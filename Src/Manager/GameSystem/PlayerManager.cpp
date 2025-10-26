#include"../Generic/InputManager.h"
#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../GameSystem/AttackManager.h"
#include"../GameSystem/LockOnManager.h"
#include"../Decoration/SoundManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Utility/Utility.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include "PlayerManager.h"

const std::string PlayerManager::ATTACK_NOMAL = "PlayerAttack";

namespace {
	float AtkScl = 70.0f;
	float AtkPow = 30.0f;
	int playerNum = 0;
	VECTOR ATK_LOCAL_POS = { 0.0f, 75.0f, 100.0f };	//攻撃相対座標
}

PlayerManager::PlayerManager(Game& _gameScene, EnemyManager& _enemy):scene_(_gameScene)
{
	lockOn_ = std::make_unique<LockOnManager>(_gameScene, *this, _enemy);
	stateCnt_ = 0;
	stateLimit_ = 0;
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//キャラクター生成
	character_ = std::make_shared<PlayerChara>();
	character_->Init(playerNum);
}

void PlayerManager::Update(AttackManager& _atk)
{
	//状態管理
	//通常じゃないとき
	if (character_->GetState() != PlayerChara::STATE::NOMAL) {
		//カウンタが状態の上限時間を上回っていたら
		if (stateCnt_ > stateLimit_) {
			//通常に戻す
			character_->SetState(PlayerChara::STATE::NOMAL);
		}
		stateCnt_++;
	}

	//プレイヤーからの入力
	UserInput(_atk);
	//キャラクター更新
	character_->Update();
	//ロックオン更新
	lockOn_->Update();
}

void PlayerManager::Draw(void)
{
	character_->Draw();
}

void PlayerManager::Release(void)
{
	character_->Release();
}

const VECTOR PlayerManager::GetPos(void)
{
	return character_->GetPos();
}

const Quaternion PlayerManager::GetQua(void)
{
	return character_->GetQua();
}

std::weak_ptr<PlayerChara> PlayerManager::GetPlayer(void)
{
	return character_;
}

const VECTOR PlayerManager::GetFocusPoint(void)
{
	return character_->GetFocusPoint();
}


void PlayerManager::RedyLockOn(void)
{
	character_->ChangeRockState(true);
}

void PlayerManager::RedyLockOff(void)
{
	character_->ChangeRockState(false);
}

void PlayerManager::UserInput(AttackManager& _atk)
{
	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();

	//攻撃中は入力を受け付けない
	if (character_->GetState() == PlayerChara::STATE::ATTACK)return;

	//攻撃の生成
	if (ins.IsTrigerrDown("attack")) {
		//攻撃の生成および状態の設定
		_atk.Attack("Player",ATTACK_NOMAL, AtkPow, VAdd(character_->GetPos(), character_->GetQua().PosAxis(ATK_LOCAL_POS)), character_->GetQua(), AttackManager::ATTACK_MASTER::PLAYER, AtkScl, "SwingSword");
		character_->SetState(PlayerChara::STATE::ATTACK);
		//対応するアニメーション
		character_->PlayAnim("atkFirst");
		//時間の設定
		RedyStateCount(static_cast<int>(_atk.GetTotalTime(ATTACK_NOMAL)));
	}

	//回避入力があったとき(ロックオン状態でしか作動しない)
	if (IsDudgeMove() && ins.IsTrigerrDown("jump") && character_->IsRock()) {
		//回避処理
		DoDudge();
	}

	//移動
	//入力
	//入力があったら対応した移動方向をセット
	if (ins.IsPressed("up")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::FORWARD);
	}
	else if (ins.IsPressed("left")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::LEFT);
	}
	else if (ins.IsPressed("down")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::BACK);
	}
	else if (ins.IsPressed("right")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::RIGHT);
	}
	//移動していないとき
	else {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::NONE);
	}
	//ダッシュ
	character_->InputDash(ins.IsPressed("dash"));

	//ロックオン
	if (ins.IsPressed("rock") && lockOn_->CanLockOn()) {
		lockOn_->LockOn();
	}

	if (ins.IsTrigerrUp("rock")) {
		lockOn_->LockOff();
	}
}


void PlayerManager::RedyStateCount(const int _limit)
{
	//カウンターの初期化
	stateCnt_ = 0;
	//上限時間の設定
	stateLimit_ = _limit;
}

const bool PlayerManager::IsDudgeMove(void) const
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsPressed("right") || ins.IsPressed("left") || ins.IsPressed("down")|| ins.IsPressed("up");
}

void PlayerManager::DoDudge(void)
{
	//回避状態に
	character_->SetState(PlayerChara::STATE::DODGE);
	//回避音出す
	SoundManager::GetInstance().Play("Dodge");

	//カメラとキャラクターの前方同士の内積
	auto cFor = SceneManager::GetInstance().GetCamera().GetRot().GetForward();
	auto pFor = character_->GetForward();
	bool isReverse = false;

	float CtoP = Utility::DotF(cFor, pFor);
	if (CtoP < 0.0f) {
		//キャラクターの向きが反転している。
		isReverse = true;
	}

	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();


	if (!isReverse) {
		//反転していない場合
		if (ins.IsPressed("left")) {
			//左回避
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("right")) {
			//右回避
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("down")) {
			//後ろ回避
			character_->PlayAnim("dodB");
		}
	}
	else {
		if (ins.IsPressed("left")) {
			//右回避
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("right")) {
			//左回避
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("up")) {
			//後ろ回避
			character_->PlayAnim("dodB");
		}
	}

	//時間の設定
	RedyStateCount(LIMIT_AVOID_STATE);
}

const bool PlayerManager::IsAlive(void) const
{
	return character_->IsAlive();
}

void PlayerManager::DrawDebug(void)
{
	character_->DrawDebug();
}
