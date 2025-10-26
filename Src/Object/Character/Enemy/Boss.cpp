#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Utility/Utility.h"
#include"../../../UI/EnemyUIController.h"
#include"../../../Renderer/ModelMaterial.h"
#include "Boss.h"

//���[�J���萔
namespace {
#pragma region �A�j���[�V�����֘A
	static constexpr int BOSS_IDLE = 1;
	static constexpr int BOSS_WALK = 2;
	static constexpr int BOSS_PRE_PUNCH = 5;
	static constexpr int BOSS_PUNCH = 6;
	static constexpr int BOSS_PRE_SHOUT = 9;
	static constexpr int BOSS_SHOUT = 10;
	static constexpr int BOSS_DETH = 12;

	static constexpr float BOSS_HP = 300.0f;
	static constexpr float BOSS_RADIUS = 400.0f;
#pragma endregion
}

Boss::Boss(VECTOR& _pos):EnemyBase(_pos)
{
}

Boss::~Boss(void)
{
}

void Boss::Shout(void)
{
	animController_->Play("preShout", 0.8f, { "shout" });
	SoundManager::GetInstance().Play("shout");
}

void Boss::InitAnim(void)
{
	animController_->Add("idle", BOSS_IDLE, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("preAttack", BOSS_PRE_PUNCH, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("attack", BOSS_PUNCH, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("preShout", BOSS_PRE_SHOUT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("shout", BOSS_SHOUT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("walk", BOSS_WALK, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dethStart", BOSS_DETH, AnimationController::PLAY_TYPE::NOMAL);
	
	//���̏ꂵ�̂��ł����ɖ��������
	SoundManager::GetInstance().Add(SoundManager::TYPE::SE, "shout", ResourceManager::GetInstance().Load(ResourceManager::SRC::BOSS_SHOUT_SE).handleId_);
}

void Boss::SetPram(void)
{
	speciesName_ = "Boss";

	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::BOSS_MDL);

	if (modelId_ == -1) {
		return;
	}
	//�p�����[�^�֌W
	const float SCALE = 0.02f;
	scl_ = { SCALE,SCALE ,SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//�������p�Ɉ����s
	UpdateRotQuat();

	//�����蔻��傫��
	colRadius_ = BOSS_RADIUS;

	//�U���\����
	atkDistance_ = 500.0f;

	//�U���̔����ʒu(���΍��W)
	atkRelative_ = VECTOR{ 0.0f,75.0f,atkDistance_ };
	//�U���̑傫��
	atkScale_ = 500.0f;

	//�A�j���[�V����������
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	hp_ = BOSS_HP;
	maxHp_ = BOSS_HP;

	uiDeviationY_ = 600.0f;

	//�ʒu�ݒ�
	uiPos_ = pos_;
	//���ʒu
	uiPos_.y += uiDeviationY_;

	//UI������
	InitUI();

	//���f���`��N���X����
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS

	intervalCnt_ = 0.0f;
	//PS
	//�e�F�̋���(�g�U��)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//�u���[�̋���(�ŏ��̍��ڂ̂݊֌W����)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//�T���v����(�ŏ��̍��ڂ̂݊֌W����)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });

	//��Ԃ�ʏ��
	ChangeState(ENEMY_STATE::BATTLE);
}

void Boss::UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)
{
	//���̓��e�͏����L�����p�B�U�����ɂ͎~�܂��čU������
	//�����L�����N�^�[�͈ړ��U�����z�肷��̂ł����̏����Ƃ͏�������Ă�����
	//�v���C���[�Ƃ̋���
	float distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//�ړ�����
	//�ړ�����
	if (distance >= atkDistance_) {
		(this->*move_)(_pPos);
	}
	else {
		OderGoalRot(_pPos);	//��]�̐ݒ肾���͍s��
	}

	//�J�E���^����(�Q�[���X�V�X�s�[�h)
	intervalCnt_ += SceneManager::GetInstance().GetUpdateSpeedRate_();

	//����
	//�v���C���[���U���͈͓����U���\�ȊԊu���J���Ă���̂Ȃ�
	if (distance <= atkDistance_ && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		//�U���̏�������

		//�������Ԃ��I�������U������
		_atk.Attack(speciesName_, EnemyManager::ATTACK_NOMAL, POW_ATTACK_NOMAL, VAdd(pos_, characterRotY_.PosAxis(atkRelative_)), characterRotY_, AttackManager::ATTACK_MASTER::ENEMY, atkScale_, "SwingSword");
		animController_->Play("attack", SPEED_ANIM);
		stopTime_ = _atk.GetTotalTime(EnemyManager::ATTACK_NOMAL);
		intervalCnt_ = 0.0f;
	}
}

void Boss::MoveBattle(const VECTOR& _pPos)
{
	//��~���Ԃ��܂�����ꍇ
	if (stopTime_ > 0) {

		stopTime_--;
		return;
	}
	//�ړ�(�O������)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSped_ * SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("walk", SPEED_ANIM);

	//��]
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//�J�����p�x
	//��������Ώۂւ̃x�N�g��
	auto diff = VSub(_pPos, pos_);
	//�p�x���߂�
	auto rad = atan2(diff.x, diff.z);

	//�����̐ݒ�
	SetGoalRot(static_cast<float>(rad) - cameraRot.y);

	debugRot_ = Utility::Deg2RadF(rad);
}

void Boss::DrawUI(void)
{
	//HP�{�b�N�X�\��
	if (hp_ >= 0) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::HP);
	}

	//���b�N�I���֌WUI
	//���g�����b�N�I���Ώۂ�������
	if (isLockTarget_) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::TARGETTING);
	}
}

