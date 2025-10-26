#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../UI/EnemyUIController.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include "EnemyBase.h"

//ローカル定数
namespace {
	constexpr VECTOR forward={0.0f,0.0f,100.0f};

	//デバッグ用
	constexpr int serchDebugCol = 0x6495ef;
	constexpr int alertDebugCol = 0xff0000;

	constexpr int serchDebugCol2 = 0xffff00;
	constexpr int alertDebugCol2 = 0xffdd88;
}

EnemyBase::EnemyBase(VECTOR& _pos)
{
	speciesName_ = "Enemy";
	serchCol_ = serchDebugCol;
	alertCol_ = serchDebugCol2;
	color_ = 0xffffff;

	update_ = &EnemyBase::UpdateNomal;
	move_ = &EnemyBase::MoveNomal;

	moveOneTime_ = -1.0f;

	debugRot_ = -1.0;

	pos_ = _pos;
	preStayPos_ = Utility::VECTOR_INIT;
	uiPos_ = Utility::VECTOR_INIT;

	//行先設定のため初期はステイ状態にする
	isStay_ = true;
	stayCnt_ = STAY_TIME;
	stopTime_ = -1.0f; 
	intervalCnt_ = INTERVAL_ATTACK_NOMAL;

	searchRestartCnt_ = 0.0f;
	searchCnt_ = 0.0f;

	hp_ = ENEMY_HP;
	moveSped_ = MOVE_POW;

	isAlive_ = true;
	state_ = ENEMY_STATE::END;

	isLockTarget_ = false;
}

EnemyBase::~EnemyBase(void)
{
}

const bool EnemyBase::Init(const int _num)
{
	//個体名登録
	speciesName_ += std::to_string(_num);

	SetPram();

	renderer_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	return true;
}


void EnemyBase::Update(const VECTOR _pPos, AttackManager& _atk)
{
	prePos_ = pos_;

	//行動更新
	(this->*update_)(_pPos,_atk);
	//共通更新
	Rotation();
	UpdateRotQuat();

	animController_->Update();
	//位置設定
	uiPos_ = pos_;
	//頭位置
	uiPos_.y = uiDeviationY_;

	uiCntl_->Update();
}

void EnemyBase::InitAnim(void)
{
	
}

void EnemyBase::InitUI(void)
{
	//UIコントローラー初期化
	uiCntl_ = std::make_unique<EnemyUIController>(uiPos_,state_);
	uiCntl_->Init(speciesName_);
	uiCntl_->CreateUI(speciesName_, hp_, maxHp_);
}




void EnemyBase::UpdateNomal(const VECTOR& _pPos, AttackManager& _atk)
{
	//移動処理
	(this->*move_)(_pPos);

	//カウンタ増加
	searchRestartCnt_++;

	//サーチ完了時間になったら
	if (searchRestartCnt_ > SEARCH_RESTART_TIME) {
		//判定
		auto deg = Utility::AngleDeg(GetForward(), VSub(_pPos, pos_));

		//索敵可能範囲内に入ったら
		if (Utility::MagnitudeF(VSub(_pPos, pos_)) <= ALERT_DISTANCE &&
			deg <= FIELD_VISION_DEG_HALF) {
			//索敵状態に
			ChangeState(ENEMY_STATE::SEARCH);
		}
	}
	
}

void EnemyBase::UpdateSearch(const VECTOR& _pPos, AttackManager& _atk)
{
	//移動処理
	(this->*move_)(_pPos);
	
	//判定
	auto deg = Utility::AngleDeg(GetForward(), VSub(_pPos, pos_));
	auto distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//視界内なら
	if (deg <= FIELD_VISION_DEG_HALF &&
		distance <= ALERT_DISTANCE) {
		//一定時間いたら
		if (searchCnt_ >= SEARCH_CNT_MAX) {
			//戦闘状態に
			SoundManager::GetInstance().Play("FindPlayer");
			ChangeState(ENEMY_STATE::BATTLE);
		}
		else {
			//カウンタ増加
			searchCnt_++;
		}
	}
	//プレイヤーが索敵範囲外に出たら
	if (distance > ALERT_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//デバッグ
	debugRot_ = deg;
}

void EnemyBase::UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)
{
	//この内容は初期キャラ用。攻撃時には止まって攻撃する
	//強いキャラクターは移動攻撃も想定するのでここの処理とは少し違っていくる
	//プレイヤーとの距離
	float distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//移動処理
	if (distance >= ATTACK_DISTANCE) {
		(this->*move_)(_pPos);
	}
	else {
		OderGoalRot(_pPos);	//回転の設定だけは行う
	}
	
	//カウンタ増加(ゲーム更新スピード)
	intervalCnt_+=SceneManager::GetInstance().GetUpdateSpeedRate_();	

	//判定
	//プレイヤーが戦闘状態範囲度外にでたら
	if (distance > BATTLE_FINISH_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//プレイヤーが攻撃範囲内かつ攻撃可能な間隔を開けているのなら
	if (distance <= ATTACK_DISTANCE && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		//攻撃する
		_atk.Attack(speciesName_,EnemyManager::ATTACK_NOMAL, POW_ATTACK_NOMAL, VAdd(pos_, characterRotY_.PosAxis(atkRelative_)), characterRotY_, AttackManager::ATTACK_MASTER::ENEMY, atkScale_, "SwingSword");
		animController_->Play("attack", SPEED_ANIM);
		stopTime_ = _atk.GetTotalTime(EnemyManager::ATTACK_NOMAL);
		intervalCnt_ = 0.0f;
	}
}

void EnemyBase::UpdateDeth(const VECTOR& _pPos, AttackManager& _atk)
{
	scl_ = VSub(scl_, SCALE_DOWN);
	if (Utility::LessThanVZero(scl_)) {
		ChangeState(ENEMY_STATE::END);
	}
}

void EnemyBase::MoveNomal(const VECTOR& _pPos)
{
	//ステイ状態のとき
	if (isStay_) {
		//待機アニメーション
		animController_->Play("idle", SPEED_ANIM);

		//一定の時間が過ぎていたら
		if (stayCnt_ >= STAY_TIME) {
			//行先の再設定
			//行先の角度設定(characterRotYに変更すべし)
			VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
			float degRand = static_cast<float>(GetRand(static_cast<int>(Utility::CIRCLE_DEG)));
			float radRand = Utility::Deg2RadF(degRand) - cameraRot.y;
			//クォータニオンに変換
			Quaternion quaGoal= Quaternion::AngleAxis((double)cameraRot.y + radRand, Utility::AXIS_Y);
			//キャラクター回転も設定
			SetGoalRot(radRand);
			characterRotY_ = quaGoal;

			//移動量を範囲付きのランダムで生成
			moveOneTime_ = static_cast<float>(GetRand(static_cast<int>(MOVE_RANDOM_MAX))) + MOVE_RANDOM_MIN;
			//ステイ状態の解除
			isStay_ = false;
			//前回停止位置の更新
			preStayPos_ = pos_;
		}
		else {
			//引き続きステイ
			//カウンタ増加(ゲーム更新スピード)
			stayCnt_+= SceneManager::GetInstance().GetUpdateSpeedRate_();
			return;
		}
	}
	
	//移動(前方方向)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSped_* SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("walk", SPEED_ANIM);

	//判定
	//停止した位置からどれだけ離れているか
	float diffNow2Pre = Utility::MagnitudeF(VSub(pos_, preStayPos_));

	//既定の移動量以上の値の時
	if (diffNow2Pre >=moveOneTime_) {
		//ステイ状態に
		isStay_ = true;
		stayCnt_ = 0;
	}

	
}

void EnemyBase::MoveSearch(const VECTOR& _pPos)
{
	//実装予定
	//＜敵のひきつけ＞
	//ほかのオブジェクトの使用による効果なので初期では作らない
}

void EnemyBase::MoveBattle(const VECTOR& _pPos)
{
	//停止時間がまだある場合
	if (stopTime_ > 0) {

		stopTime_--;
		return;
	}
	//移動(前方方向)
	pos_=VAdd(pos_, VScale(GetForward(), moveSped_* SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("dush", SPEED_ANIM);

	OderGoalRot(_pPos);
}


void EnemyBase::OderGoalRot(const VECTOR _pPos) {
	//回転
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
	//自分から対象へのベクトル
	auto diff = VSub(_pPos, pos_);
	//角度求める
	auto rad = atan2(diff.x, diff.z);

	//方向の設定
	SetGoalRot(static_cast<float>(rad) - cameraRot.y);

	debugRot_ = Utility::Deg2RadF(rad);
}

void EnemyBase::ChangeState(const ENEMY_STATE _state)
{
	state_ = _state;

	switch (state_)
	{
	case ENEMY_STATE::NOMAL:
		update_ = &EnemyBase::UpdateNomal;
		move_ = &EnemyBase::MoveNomal;
		moveSped_ = MOVE_POW;
		uiCntl_->FindReset();
		searchCnt_ = 0.0f;
		searchRestartCnt_ = 0.0f;

		serchCol_ = serchDebugCol;
		alertCol_ = serchDebugCol2;
		break;

	case ENEMY_STATE::SEARCH:
		update_ = &EnemyBase::UpdateSearch;
		move_ = &EnemyBase::MoveSearch;
		moveSped_ = MOVE_POW;
		//待機アニメーション
		animController_->Play("idle", SPEED_ANIM);

		serchCol_ = serchDebugCol;
		alertCol_ = alertDebugCol2;
		break;

	case ENEMY_STATE::BATTLE:
		update_ = &EnemyBase::UpdateBattle;
		move_ = &EnemyBase::MoveBattle;
		moveSped_ = MOVE_POW_FIND;
		

		serchCol_ = alertDebugCol;
		break;
	case ENEMY_STATE::DETH:
		update_ = &EnemyBase::UpdateDeth;
		//死亡アニメーション
		animController_->Play("dethStart", SPEED_ANIM,{"dethSus"});

		break;
	case ENEMY_STATE::END:
		//削除可能に
		isAlive_ = false;
		break;
	default:
		break;
	}
}

void EnemyBase::Draw(void)
{
	renderer_->Draw();
	DrawUI();
}

void EnemyBase::DrawUI(void)
{
	//発見マーク
	if (state_ == ENEMY_STATE::SEARCH|| state_ == ENEMY_STATE::BATTLE) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::FIND);
	}

	//HPボックス表示
	if (hp_ >= 0) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::HP);
	}

	//ロックオン関係UI
	//自身がロックオン対象だったら
	if (isLockTarget_) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::TARGETTING);
	}
}

void EnemyBase::SetColor(int _color)
{
	color_ = _color;
}

void EnemyBase::SetPos(VECTOR _pos)
{
	pos_ = _pos;
}

void EnemyBase::DrawDebug(void)
{
	//VECTOR forwardDif = { 0,0,100 };
	////前方向明示
	//VECTOR fowardPos = VAdd(pos_, characterRotY_.PosAxis(forwardDif));
	//DrawSphere3D(fowardPos, 8, 8, 0xff5500, 0xff0000, false);
	////視野角
	//auto conePos = VAdd(pos_, VECTOR{ 0.0f,-1.0f,0.0f });

	//DrawCone3D(conePos, pos_, ALERT_DISTANCE, 30, alertCol_, 0x000000, true);

	//DrawCone3D(conePos, pos_, FIELD_VISION_DISTANCE, 30, serchCol_, 0x000000, true);

	/*VECTOR fowardDir = VAdd(pos_, characterRotY_.PosAxis(forward));
	DrawFormatString(0, 0, 0xffffff, "EPOS={%.1f,%.1f,%.1f}\nDEG={%.1f}\nForward={%.1f,%.1f,%.1f}",
		pos_.x, pos_.y, pos_.z,
		static_cast<float>(debugRot_),
		fowardDir.x, fowardDir.y, fowardDir.z);*/

	DrawCupcel();
}

const bool EnemyBase::IsAlive(void) const
{
	return isAlive_;
}

const void EnemyBase::SetAnimSpeedRate(const float _percent)
{
	animController_->ChangeSpeedRate(_percent);
}

void EnemyBase::SetIsLocked(const bool _flag)
{
	uiCntl_->ChangeTargetUI(_flag);
}

void EnemyBase::Damage(const float _pow)
{
	//攻撃力分減らす
	hp_ -= static_cast<int>(_pow);
	//戦闘状態ではなかったら
	if (state_ != ENEMY_STATE::BATTLE) {
		//戦闘状態に
		SoundManager::GetInstance().Play("FindPlayer");
		ChangeState(ENEMY_STATE::BATTLE);
	}
	//0以下のとき
	if (hp_ <= 0) {
		//死亡処理
		Deth();
	}
}

void EnemyBase::Deth(void)
{
	ChangeState(ENEMY_STATE::DETH);
}

void EnemyBase::Shout(void)
{
	//ボス専用
}
