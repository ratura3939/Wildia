#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "SceneManager.h"
#include "InputManager.h"
//#include "../Object/Character/PlayableChara/PlayerBase.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	currentMode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	focusPos_ = { 0.0f, 0.0f, 0.0f };
	goalFocusPos_ = { 0.0f, 0.0f, 0.0f };
	lockPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
	rotSpeed_ = MAX_ROT_SPEED;

	stepReset_ = 0.0f;
	isReset_ = true;

	followObject_.pos = { 0.0f, 0.0f, 0.0f };
	followObject_.quaRot = Quaternion::Identity();
	start_.pos = { 0.0f, 0.0f, 0.0f };
	start_.quaRot = Quaternion::Identity();
	goal_.pos = { 0.0f, 0.0f, 0.0f };
	goal_.quaRot = Quaternion::Identity();

	angles_.x = Utility::Deg2RadF(0.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	lerpSpeed_ = NO_LERP;
	finishShake_ = false;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	//カメラの初期設定
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);


	switch (mode_)
	{
	case MODE::NONE:
		SetBeforeDrawFollow();
		break;

	case MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case MODE::FREE:
		SetBeforeDrawFree();
		break;
	
	case MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case MODE::LOCKON:
		SetBeforeDrawLockOn();
		break;

	case MODE::FOLLOW_SPRING:
		break;

	case MODE::SHAKE:
		SetBeforeDrawShake();
		break;

	case MODE::RESET:
		SetBeforeDrawReset();
		break;
	case MODE::AUTO_MOVE:
		SetBeforeDrawAutoMove();
		break;
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		focusPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	Rotation();
}

void Camera::SetBeforeDrawFollow(void)
{
	//回転
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	auto& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("rock")) {
		ChangeMode(MODE::RESET);
		return;
	}

	//追従対象までの距離ベクトルを回転させ相対座標を生成
	VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//カメラ位置の更新(追従対象位置から相対座標を足す)
	VECTOR gPos = VAdd(followPos, relativeCPos);

	if (fabs(Utility::MagnitudeF(gPos) - Utility::MagnitudeF(pos_)) <= 10.0f) {
		lerpSpeed_ = NO_LERP;
	}

	pos_ = Utility::Lerp(pos_, gPos, lerpSpeed_);

	//注視点までの距離ベクトルを回転させ相対座標を生成
	VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	focusPos_ = VAdd(followPos, relativeTPos);

	//カメラの上方向
	cameraUp_ = rot_.GetUp();

}

void Camera::SetBeforeDrawLockOn(void)
{
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;
	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	//ロックオン対象と追従対象の離れている距離
	VECTOR distance = VSub(lockPos_, followPos);

	//離れる距離を数値化
	float disMag = Utility::MagnitudeF(distance);
	//最低限の値を下回っていたら
	if (disMag <= ROCK_DISTANCE_MIN) {
		//最低限の値を入れる
		disMag = ROCK_DISTANCE_MIN;
	}

	//カメラ位置調整(カメラは後方位置に。Y方向は距離に応じて高さを変える。)
	VECTOR relative = { 0.0f,disMag * ROCK_MAGNIFICATION_Y,-disMag };
	//カメラの回転情報をもとに相対座標を回転させる
	VECTOR relativeCPos = rot_.PosAxis(relative);

	//初動時のみに発動する
	//カメラの初期ゴールを計算結果で算出した場所にする
	if (!isReset_) {
		ChangeMode(MODE::RESET);
		goal_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(relative));
		goal_.quaRot = followObject_.quaRot;
		return;
	}

	//注視点の更新
	//ロックオン中の注視点は追従対象とロックオン対象の中間地点にある。
	//focusPos_ = VAdd(followPos,VScale(distance, 0.5f));
	goalFocusPos_ = VAdd(followPos, VScale(distance, 0.5f));
	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, 0.2f);

	//カメラ位置の更新
	prevGoalPos_ = lockOnGoalPos_;
	lockOnGoalPos_ = VAdd(focusPos_, relativeCPos);
	if (!Utility::Equals(lockOnGoalPos_, prevGoalPos_)) {
		lockOnLerpStep_ = 0.0f;
	}

	lockOnLerpStep_ += RESET_STEP;
	if (lockOnLerpStep_ > 1.0f)lockOnLerpStep_ = 1.0f;

	pos_ = Utility::Lerp(pos_, lockOnGoalPos_, lockOnLerpStep_);
	//pos_ = VAdd(focusPos_, relativeCPos);

	//ある程度の高さは保つ
	if (pos_.y < UNDER_LIMIT_Y)pos_.y = UNDER_LIMIT_Y;

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawShake(void)
{
	// 一定時間カメラを揺らす
	//stepShake_ -= SceneManager::GetInstance().GetDeltaTime();

	stepShake_ -= 0.01f;

	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::FIXED_POINT);
		finishShake_ = true;
		return;
	}

	// -1.0f～1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);

	// -1000.0f～1000.0f
	f *= 1000.0f;

	// -1000 or 1000
	int d = static_cast<int>(f);

	// 0 or 1
	int shake = d % 2;

	// 0 or 2
	shake *= 2;

	// -1 or 1
	shake -= 1;

	// 移動量
	VECTOR velocity = VScale(shakeDir_, (float)(shake)*WIDTH_SHAKE);

	// 移動先座標
	 pos_ = VAdd(defaultPos_, velocity);

	//float pow = WIDTH_SHAKE * sinf(stepShake_ * SPEED_SHAKE);
	//VECTOR velocity = VScale(shakeDir_, pow);
	//VECTOR newPos = VAdd(defaultPos_, velocity);
	//pos_ = newPos;

}

void Camera::SetBeforeDrawReset(void)
{
	//angleを逆算させる方法を考える

	stepReset_ += RESET_STEP;
	//終了条件
	if (stepReset_ >= RESET_TIME) {
		ChangeMode(currentMode_);
		isReset_ = true;
		angles_ = Utility::VECTOR_ZERO;

		VECTOR axY = { 0.0f,1.0f,0.0f };

		rot_.ToAngleAxis(&angles_.y, &axY);

		return;
	}

	//球面補間
	rot_ = Quaternion::Slerp(start_.quaRot, goal_.quaRot, stepReset_);
	//pos_ = Utility::Lerp(start_.pos, goal_.pos, stepReset_);
	pos_ = VAdd(followObject_.pos, rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW));

	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, 0.8f);

	VECTOR axY = { 0.0f,1.0f,0.0f };

	rot_.ToAngleAxis(&angles_.y, &axY);

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawAutoMove(void)
{
	//目標位置まで移動する
	//終了の判定は呼び出した側で行う
	pos_ = Utility::Lerp(pos_, goalPos_, 0.01f);

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

const VECTOR Camera::GetPos(void) const
{
	return pos_;
}

const Quaternion Camera::GetRot(void) const
{
	return rot_;
}

const VECTOR Camera::GetAngle(void) const
{
	return angles_;
}

const float Camera::GetRotSpeed(void) const
{
	return rotSpeed_;
}

void Camera::SetRotSpeed(const float _speed)
{
	rotSpeed_ = _speed;
}

void Camera::ChangeMode(MODE mode)
{

	//カメラの初期設定
	//カメラを揺らす前の位置で揺れるようにしたいため外している
	//SetDefault();
	
	if (mode == MODE::RESET)currentMode_ = mode_;

	//カメラモードの変更
  	mode_ = mode;

	isReset_ = false;

	//変更時の初期化処理
	switch (mode_)
	{
	case MODE::FIXED_POINT:
		break;
	case MODE::FREE:
		break;
	case MODE::FOLLOW:
		lerpSpeed_ = LERP_SPEED;
		break;
	case MODE::FOLLOW_SPRING:
		break;
	case MODE::SHAKE:
		finishShake_ = false;
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm({ 0.7f, 0.7f ,0.0f });
		defaultPos_ = pos_;
		break;
	case MODE::RESET:
		stepReset_ = 0.0f;
		start_.pos = pos_;
		start_.quaRot = rot_;
		goal_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(RELATIVE_F2C_POS_FOLLOW));
		goal_.quaRot = followObject_.quaRot;
		break;
	}

}

void Camera::SetFollow(const VECTOR _pos, const Quaternion _qua)
{
	followObject_.pos = _pos;
	followObject_.quaRot = _qua;
}

void Camera::SetPos(const VECTOR& pos, const VECTOR& target)
{
	pos_ = pos;
	focusPos_ = target;
}

void Camera::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void Camera::SetFocusPos(const VECTOR& _focus)
{
	//focusPos_ = _focus;
	goalFocusPos_ = _focus;
}

void Camera::SetRockPos(const VECTOR& _rock)
{
	lockPos_ = _rock;
}

void Camera::SetGoalPos(const VECTOR& _goal)
{
	goalPos_ = _goal;
}

const VECTOR Camera::GetRockPos(void) const
{
	return lockPos_;
}

const Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

void Camera::DrawDebug(void)
{
	//DrawFormatString(0, 0, 0xffffff, "cPOS={%.1f,%.1f,%.1f}\ncROT={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	//DrawFormatString(0, 100, 0xffffff, "FCPOS={%.1f,%.1f,%.1f}", focusPos_.x, focusPos_.y, focusPos_.z);
	DrawSphere3D(focusPos_, 8, 10, 0x00ff00, 0x00ff00, false);
}

void Camera::SetDefault(void)
{

	//カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	//注視点
	focusPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//カメラはX軸に傾いているが、
	//この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();

}

void Camera::Rotation(void)
{

	InputManager& ins = InputManager::GetInstance();

	if (ins.IsPressed("subUp"))
	{
		angles_.x -= rotSpeed_;
		if (angles_.x <= LIMIT_X_DW_RAD)
			angles_.x = LIMIT_X_DW_RAD;
	}
	if (ins.IsPressed("subDown"))
	{
		angles_.x += rotSpeed_;
		if (angles_.x >= LIMIT_X_UP_RAD)
			angles_.x = LIMIT_X_UP_RAD;
	}
	if (ins.IsPressed("subLeft"))
	{
		angles_.y -= rotSpeed_;
	}
	if (ins.IsPressed("subRight"))
	{
		angles_.y += rotSpeed_;
	}

	//カメラ座標を中心として、注視点を回転させる
	if (!Utility::EqualsVZero(angles_))
	{

		// 正面から設定されたY軸分、回転させる
		rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

		// 正面から設定されたX軸分、回転させる
		rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
		// カメラの上方向
		cameraUp_ = rot_.GetUp();
	}
}

