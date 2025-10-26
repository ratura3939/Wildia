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
	VECTOR ATK_LOCAL_POS = { 0.0f, 75.0f, 100.0f };	//�U�����΍��W
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
	//�L�����N�^�[����
	character_ = std::make_shared<PlayerChara>();
	character_->Init(playerNum);
}

void PlayerManager::Update(AttackManager& _atk)
{
	//��ԊǗ�
	//�ʏ킶��Ȃ��Ƃ�
	if (character_->GetState() != PlayerChara::STATE::NOMAL) {
		//�J�E���^����Ԃ̏�����Ԃ������Ă�����
		if (stateCnt_ > stateLimit_) {
			//�ʏ�ɖ߂�
			character_->SetState(PlayerChara::STATE::NOMAL);
		}
		stateCnt_++;
	}

	//�v���C���[����̓���
	UserInput(_atk);
	//�L�����N�^�[�X�V
	character_->Update();
	//���b�N�I���X�V
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
	//�v���C���[����̓��͑��܂Ƃ�
	InputManager& ins = InputManager::GetInstance();

	//�U�����͓��͂��󂯕t���Ȃ�
	if (character_->GetState() == PlayerChara::STATE::ATTACK)return;

	//�U���̐���
	if (ins.IsTrigerrDown("attack")) {
		//�U���̐�������я�Ԃ̐ݒ�
		_atk.Attack("Player",ATTACK_NOMAL, AtkPow, VAdd(character_->GetPos(), character_->GetQua().PosAxis(ATK_LOCAL_POS)), character_->GetQua(), AttackManager::ATTACK_MASTER::PLAYER, AtkScl, "SwingSword");
		character_->SetState(PlayerChara::STATE::ATTACK);
		//�Ή�����A�j���[�V����
		character_->PlayAnim("atkFirst");
		//���Ԃ̐ݒ�
		RedyStateCount(static_cast<int>(_atk.GetTotalTime(ATTACK_NOMAL)));
	}

	//�����͂��������Ƃ�(���b�N�I����Ԃł����쓮���Ȃ�)
	if (IsDudgeMove() && ins.IsTrigerrDown("jump") && character_->IsRock()) {
		//�������
		DoDudge();
	}

	//�ړ�
	//����
	//���͂���������Ή������ړ��������Z�b�g
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
	//�ړ����Ă��Ȃ��Ƃ�
	else {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::NONE);
	}
	//�_�b�V��
	character_->InputDash(ins.IsPressed("dash"));

	//���b�N�I��
	if (ins.IsPressed("rock") && lockOn_->CanLockOn()) {
		lockOn_->LockOn();
	}

	if (ins.IsTrigerrUp("rock")) {
		lockOn_->LockOff();
	}
}


void PlayerManager::RedyStateCount(const int _limit)
{
	//�J�E���^�[�̏�����
	stateCnt_ = 0;
	//������Ԃ̐ݒ�
	stateLimit_ = _limit;
}

const bool PlayerManager::IsDudgeMove(void) const
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsPressed("right") || ins.IsPressed("left") || ins.IsPressed("down")|| ins.IsPressed("up");
}

void PlayerManager::DoDudge(void)
{
	//�����Ԃ�
	character_->SetState(PlayerChara::STATE::DODGE);
	//������o��
	SoundManager::GetInstance().Play("Dodge");

	//�J�����ƃL�����N�^�[�̑O�����m�̓���
	auto cFor = SceneManager::GetInstance().GetCamera().GetRot().GetForward();
	auto pFor = character_->GetForward();
	bool isReverse = false;

	float CtoP = Utility::DotF(cFor, pFor);
	if (CtoP < 0.0f) {
		//�L�����N�^�[�̌��������]���Ă���B
		isReverse = true;
	}

	//�v���C���[����̓��͑��܂Ƃ�
	InputManager& ins = InputManager::GetInstance();


	if (!isReverse) {
		//���]���Ă��Ȃ��ꍇ
		if (ins.IsPressed("left")) {
			//�����
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("right")) {
			//�E���
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("down")) {
			//�����
			character_->PlayAnim("dodB");
		}
	}
	else {
		if (ins.IsPressed("left")) {
			//�E���
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("right")) {
			//�����
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("up")) {
			//�����
			character_->PlayAnim("dodB");
		}
	}

	//���Ԃ̐ݒ�
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
