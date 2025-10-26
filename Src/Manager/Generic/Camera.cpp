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
	//�J�����̏����ݒ�
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//�N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
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

	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		focusPos_,
		cameraUp_
	);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	Rotation();
}

void Camera::SetBeforeDrawFollow(void)
{
	//��]
	Rotation();

	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followObject_.pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = followObject_.quaRot;

	auto& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("rock")) {
		ChangeMode(MODE::RESET);
		return;
	}

	//�Ǐ]�Ώۂ܂ł̋����x�N�g������]�������΍��W�𐶐�
	VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//�J�����ʒu�̍X�V(�Ǐ]�Ώۈʒu���瑊�΍��W�𑫂�)
	VECTOR gPos = VAdd(followPos, relativeCPos);

	if (fabs(Utility::MagnitudeF(gPos) - Utility::MagnitudeF(pos_)) <= 10.0f) {
		lerpSpeed_ = NO_LERP;
	}

	pos_ = Utility::Lerp(pos_, gPos, lerpSpeed_);

	//�����_�܂ł̋����x�N�g������]�������΍��W�𐶐�
	VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS);

	//�����_�̍X�V
	focusPos_ = VAdd(followPos, relativeTPos);

	//�J�����̏����
	cameraUp_ = rot_.GetUp();

}

void Camera::SetBeforeDrawLockOn(void)
{
	Rotation();

	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followObject_.pos;
	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = followObject_.quaRot;

	//���b�N�I���ΏۂƒǏ]�Ώۂ̗���Ă��鋗��
	VECTOR distance = VSub(lockPos_, followPos);

	//����鋗���𐔒l��
	float disMag = Utility::MagnitudeF(distance);
	//�Œ���̒l��������Ă�����
	if (disMag <= ROCK_DISTANCE_MIN) {
		//�Œ���̒l������
		disMag = ROCK_DISTANCE_MIN;
	}

	//�J�����ʒu����(�J�����͌���ʒu�ɁBY�����͋����ɉ����č�����ς���B)
	VECTOR relative = { 0.0f,disMag * ROCK_MAGNIFICATION_Y,-disMag };
	//�J�����̉�]�������Ƃɑ��΍��W����]������
	VECTOR relativeCPos = rot_.PosAxis(relative);

	//�������݂̂ɔ�������
	//�J�����̏����S�[�����v�Z���ʂŎZ�o�����ꏊ�ɂ���
	if (!isReset_) {
		ChangeMode(MODE::RESET);
		goal_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(relative));
		goal_.quaRot = followObject_.quaRot;
		return;
	}

	//�����_�̍X�V
	//���b�N�I�����̒����_�͒Ǐ]�Ώۂƃ��b�N�I���Ώۂ̒��Ԓn�_�ɂ���B
	//focusPos_ = VAdd(followPos,VScale(distance, 0.5f));
	goalFocusPos_ = VAdd(followPos, VScale(distance, 0.5f));
	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, 0.2f);

	//�J�����ʒu�̍X�V
	prevGoalPos_ = lockOnGoalPos_;
	lockOnGoalPos_ = VAdd(focusPos_, relativeCPos);
	if (!Utility::Equals(lockOnGoalPos_, prevGoalPos_)) {
		lockOnLerpStep_ = 0.0f;
	}

	lockOnLerpStep_ += RESET_STEP;
	if (lockOnLerpStep_ > 1.0f)lockOnLerpStep_ = 1.0f;

	pos_ = Utility::Lerp(pos_, lockOnGoalPos_, lockOnLerpStep_);
	//pos_ = VAdd(focusPos_, relativeCPos);

	//������x�̍����͕ۂ�
	if (pos_.y < UNDER_LIMIT_Y)pos_.y = UNDER_LIMIT_Y;

	//�J�����̏����
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawShake(void)
{
	// ��莞�ԃJ������h�炷
	//stepShake_ -= SceneManager::GetInstance().GetDeltaTime();

	stepShake_ -= 0.01f;

	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::FIXED_POINT);
		finishShake_ = true;
		return;
	}

	// -1.0f�`1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);

	// -1000.0f�`1000.0f
	f *= 1000.0f;

	// -1000 or 1000
	int d = static_cast<int>(f);

	// 0 or 1
	int shake = d % 2;

	// 0 or 2
	shake *= 2;

	// -1 or 1
	shake -= 1;

	// �ړ���
	VECTOR velocity = VScale(shakeDir_, (float)(shake)*WIDTH_SHAKE);

	// �ړ�����W
	 pos_ = VAdd(defaultPos_, velocity);

	//float pow = WIDTH_SHAKE * sinf(stepShake_ * SPEED_SHAKE);
	//VECTOR velocity = VScale(shakeDir_, pow);
	//VECTOR newPos = VAdd(defaultPos_, velocity);
	//pos_ = newPos;

}

void Camera::SetBeforeDrawReset(void)
{
	//angle���t�Z��������@���l����

	stepReset_ += RESET_STEP;
	//�I������
	if (stepReset_ >= RESET_TIME) {
		ChangeMode(currentMode_);
		isReset_ = true;
		angles_ = Utility::VECTOR_ZERO;

		VECTOR axY = { 0.0f,1.0f,0.0f };

		rot_.ToAngleAxis(&angles_.y, &axY);

		return;
	}

	//���ʕ��
	rot_ = Quaternion::Slerp(start_.quaRot, goal_.quaRot, stepReset_);
	//pos_ = Utility::Lerp(start_.pos, goal_.pos, stepReset_);
	pos_ = VAdd(followObject_.pos, rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW));

	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, 0.8f);

	VECTOR axY = { 0.0f,1.0f,0.0f };

	rot_.ToAngleAxis(&angles_.y, &axY);

	//�J�����̏����
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawAutoMove(void)
{
	//�ڕW�ʒu�܂ňړ�����
	//�I���̔���͌Ăяo�������ōs��
	pos_ = Utility::Lerp(pos_, goalPos_, 0.01f);

	//�J�����̏����
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

	//�J�����̏����ݒ�
	//�J������h�炷�O�̈ʒu�ŗh���悤�ɂ��������ߊO���Ă���
	//SetDefault();
	
	if (mode == MODE::RESET)currentMode_ = mode_;

	//�J�������[�h�̕ύX
  	mode_ = mode;

	isReset_ = false;

	//�ύX���̏���������
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

	//�J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	//�����_
	focusPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//�J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//�J������X���ɌX���Ă��邪�A
	//���̌X������Ԃ��p�x�[���A�X�������Ƃ���
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

	//�J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!Utility::EqualsVZero(angles_))
	{

		// ���ʂ���ݒ肳�ꂽY�����A��]������
		rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

		// ���ʂ���ݒ肳�ꂽX�����A��]������
		rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
		// �J�����̏����
		cameraUp_ = rot_.GetUp();
	}
}

