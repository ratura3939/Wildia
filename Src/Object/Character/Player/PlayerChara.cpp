#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Utility/Utility.h"
#include "PlayerChara.h"

//ローカル定数
namespace {
#pragma region ステータス定数
	const int PALYER_HP = 5;		//HP
	const float MOVE_POW = 5.0f;	//移動量
	const float DUSH_POW = 20.0f;	//移動量(ダッシュ)
	const float CHARA_SCALE = 0.7f;	//サイズ
	const VECTOR FOCUS_NOMAL = { 0.0f,0.0f,50.0f };      //注視点(通常)
	//static constexpr VECTOR FOCUS_BOW = { 0.0f,20.0f,150.0f };    //注視点(弓矢)

	const int ALLERT_TIME = 30;  //警戒UI描画時間    
#pragma endregion

#pragma region アニメーション関連定数
	const float SPEED_ANIM = 1.0f;
	const int ANIM_IDLE = 36;            //待機
	//攻撃
	const int ANIM_ATTACK_FIRST = 8;     //攻撃一段目
	const int ANIM_ATTACK_SECOND = 9;    //攻撃二段目
	const int ANIM_ATTACK_SPIN = 10;     //回転切り
	//防御
	const int ANIM_GUARD_START = 18;     //防御開始
	const int ANIM_GUARD_ATTACK = 19;    //ガードアタック
	const int ANIM_GUARD_HIT = 20;       //ガードヒット
	const int ANIM_GUARD_SUSTANABLE = 21;//ガード持続

	//その他アクション
	const int ANIM_WALK = 72;            //歩き
	const int ANIM_DUSH_FORWARD = 48;    //ダッシュ(前方)
	const int ANIM_DUSH_LEFT = 50;       //ダッシュ(左)<ロックオン時のみ>
	const int ANIM_DUSH_RIGHT = 51;      //ダッシュ(右)<ロックオン時のみ>

	const int ANIM_JUMP = 39;            //ジャンプ
	const int ANIM_DODGE_LEFT = 29;      //回避(左)
	const int ANIM_DODGE_RIGHT = 30;     //回避(右)
	const int ANIM_DODGE_BACK = 27;      //回避(後)

	const int ANIM_PICK_UP = 47;         //拾う

	//演出
	const int ANIM_DAMAGE = 35;          //被ダメ
	const int ANIM_DETH_START = 25;      //死亡開始
	const int ANIM_DETH_SUSTANABLE = 26; //死亡持続
#pragma endregion
}


PlayerChara::PlayerChara(void)
{
	speciesName_ = "Player";
	focusPoint_ = Utility::VECTOR_ZERO;
	rState_ = ROCK_STATE::MAX;
	state_ = STATE::NOMAL;
	isDush_ = false;
}

PlayerChara::~PlayerChara(void)
{
}

const bool PlayerChara::Init(const int _num)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_MDL).handleId_;
	if (modelId_ == -1)return false;

	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT),0.0f);

	rState_ = ROCK_STATE::NOMAL;

	//当たり判定大きさ
	colRadius_ = CHARACTER_RADIUS;

	//個体名登録
	speciesName_ += std::to_string(_num);

	//注視点の設定
	focusPoint_ = FOCUS_NOMAL;

	UpdateRotQuat();
	hp_ = PALYER_HP;

	//アニメ初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	//初期アニメ
	animController_->Play("idle", SPEED_ANIM);

	//UI初期化
	InitUI();

	return true;
}

void PlayerChara::Update(void)
{
	prePos_ = pos_;
	uiPos_ = pos_;
	uiPos_.y += 200.0f;
	allertTime_++;
	//ほかにアクション行動していないときのみ
	if (state_ == STATE::NOMAL || rState_ == ROCK_STATE::LOCKON) {
		Move();
		Rotation();
		if (allertTime_ > ALLERT_TIME) {
			uiCntl_->ChangeAllert(false);
		}
	}

	UpdateRotQuat();
	animController_->Update();
	uiCntl_->Update();
}

const VECTOR PlayerChara::GetFocusPoint(void) const
{
	return VAdd(pos_, focusPoint_);
}

void PlayerChara::ChangeRockState(const bool _state)
{
	if (_state) {
		rState_ = ROCK_STATE::LOCKON;
		//プレイヤーの角度を強制的に敵に向ける
		float deg = GetToLockDeg();
		SetGoalRot(deg);
		characterRotY_ = goalQua_;
	}
	else rState_ = ROCK_STATE::NOMAL;
}

const PlayerChara::STATE PlayerChara::GetState(void) const
{
	return state_;
}

void PlayerChara::SetState(const STATE& _state)
{
	state_ = _state;
}

void PlayerChara::PlayAnim(const std::string _anim)
{
	//アニメーション
	animController_->Play(_anim, SPEED_ANIM);
}

const bool PlayerChara::IsRock(void)
{
	return rState_==ROCK_STATE::LOCKON;
}

void PlayerChara::Damage(const float _pow)
{
	//攻撃力分減らす
	hp_ -= static_cast<int>(_pow);

	uiCntl_->Damage();
	//0以下のとき
	if (hp_ <= 0) {
		//死亡処理
		Deth();
	}
}

void PlayerChara::DrawDebug(void)
{
	DrawFormatString(0, 40, 0xffffff, "pPos={%.1f,%.1f,%.1f}\npRot={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	DrawFormatString(0, 120, 0xffffff, "GoalRot={%.1f,%.1f,%.1f}", goalQua_.x, goalQua_.y, goalQua_.z);
	VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();
	float deg = static_cast<float>(Utility::AngleDeg(pos_, VSub(rockPos, pos_)));
	if (pos_.x > rockPos.x)deg = 180.0f + (180.0f - deg);
	DrawFormatString(0, 140, 0xffffff, "RockDeg={%.1f}", deg);

	switch (state_)
	{
	case PlayerChara::STATE::NOMAL:
		DrawString(0, 160,  "NOMAL", 0xffffff);
		break;
	case PlayerChara::STATE::GUARD:
		DrawString(0, 160, "GUARD", 0xffffff);
		break;
	case PlayerChara::STATE::DODGE:
		DrawString(0, 160, "DODGE", 0xffffff);
		break;
	case PlayerChara::STATE::ATTACK:
		DrawString(0, 160, "ATTACK", 0xffffff);
		break;
	default:
		break;
	}

	DrawCupcel();
}

float PlayerChara::GetToLockDeg(void)
{//ロックオン特有の角度設定
	VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();			//ロックオン対象位置	
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度

	//自分から対象へのベクトル
	auto diff = VSub(rockPos, pos_);
	//角度求める
	return atan2(diff.x, diff.z) - cameraRot.y;
}

void PlayerChara::SetAtkAllert(void)
{
	uiCntl_->ChangeAllert(true);
	allertTime_ = 0;
}


void PlayerChara::InitAnim(void)
{
	animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
	//攻撃
	animController_->Add("atkFirst", ANIM_ATTACK_FIRST, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("atkSecond", ANIM_ATTACK_SECOND, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("atkSpin", ANIM_ATTACK_SPIN, AnimationController::PLAY_TYPE::NOMAL);
	//防御
	animController_->Add("defStart", ANIM_GUARD_START, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defAtk", ANIM_GUARD_ATTACK, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defHit", ANIM_GUARD_HIT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defSus", ANIM_GUARD_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
	//その他アクション
	animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("walkB", ANIM_WALK, AnimationController::PLAY_TYPE::RETURN);
	animController_->Add("dushF", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dushL", ANIM_DUSH_LEFT, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dushR", ANIM_DUSH_RIGHT, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("jump", ANIM_JUMP, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodL", ANIM_DODGE_LEFT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodR", ANIM_DODGE_RIGHT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodB", ANIM_DODGE_BACK, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("pick", ANIM_PICK_UP, AnimationController::PLAY_TYPE::NOMAL);
	//演出
	animController_->Add("damage", ANIM_DAMAGE, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void PlayerChara::InitUI(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	uiPos_ = pos_;
	uiPos_.y += 200.0f;
	uiCntl_ = std::make_unique<PlayerUIController>(uiPos_, PALYER_HP);
	uiCntl_->Init(speciesName_);
}

void PlayerChara::DrawUI(void)
{
	uiCntl_->Draw();
}

void PlayerChara::Move(void)
{
	//移動を行わないとき
	if (moveDir_ == MOVE_DIR::NONE || state_ == STATE::ATTACK) {
		//通常なら
		if (state_ == STATE::NOMAL) {
			//待機アニメーション
			animController_->Play("idle", SPEED_ANIM);
		}
		return;
	}

	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetRot();
	VECTOR dir = Utility::VECTOR_ZERO;
	std::string seName = "Walk";

	float afterDeg = 0.0f;

	//移動方向
	if (moveDir_ == MOVE_DIR::FORWARD) {
		dir = cameraRot.GetForward();
		afterDeg = Utility::Deg2RadF(DEG_FORWARD);
	}
	if (moveDir_ == MOVE_DIR::LEFT) {
		dir = cameraRot.GetLeft();
		afterDeg = Utility::Deg2RadF(DEG_LEFT);
	}
	if (moveDir_ == MOVE_DIR::BACK) {
		dir = cameraRot.GetBack();
		afterDeg = Utility::Deg2RadF(DEG_BACK);
	}
	if (moveDir_ == MOVE_DIR::RIGHT) {
		dir = cameraRot.GetRight();
		afterDeg = Utility::Deg2RadF(DEG_RIGHT);
	}

	//速度設定
	float speed = MOVE_POW;
	//ダッシュのとき
	if (isDush_) {
		speed = DUSH_POW;
		seName = "Dush";
	}
	//ロックオンの時
	if (rState_ == ROCK_STATE::LOCKON)speed = MOVE_POW;

	//移動処理
	pos_ = VAdd(pos_, VScale(dir, speed));
	//上下の移動が起きない様に
	//ゆくゆくは重力とステージの当たり判定で処理する
	pos_.y = 0.0f;

	//ロックオンのとき
	if (rState_ == ROCK_STATE::LOCKON) {
		//敵との角度差を設定
		afterDeg = GetToLockDeg();
	}
	//目標角度設定
	SetGoalRot(afterDeg);

	//アニメーション
	//回避中は回避アニメーションを再生しているため他はしない
	if (state_ != STATE::DODGE) {
		animController_->Play(DecideAnim(moveDir_), SPEED_ANIM);
		SoundManager::GetInstance().Play(seName);
	}
}

const std::string PlayerChara::DecideAnim(const MOVE_DIR _dir) const
{
	std::string retAnim = "walk";
	if(isDush_)retAnim = "dushF";

	//ロックオンのとき
	if (rState_ == ROCK_STATE::LOCKON) {
		if (_dir == MOVE_DIR::LEFT) {
			retAnim = "dushL";
		}
		else if (_dir == MOVE_DIR::RIGHT) {
			retAnim = "dushR";
		}
		else if (_dir == MOVE_DIR::BACK) {
			retAnim = "walkB";
		}
	}
	
	return retAnim;
}
