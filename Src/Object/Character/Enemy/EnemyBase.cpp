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

//���[�J���萔
namespace {
	constexpr VECTOR forward={0.0f,0.0f,100.0f};

	//�f�o�b�O�p
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

	//�s��ݒ�̂��ߏ����̓X�e�C��Ԃɂ���
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
	//�̖��o�^
	speciesName_ += std::to_string(_num);

	SetPram();

	renderer_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	return true;
}


void EnemyBase::Update(const VECTOR _pPos, AttackManager& _atk)
{
	prePos_ = pos_;

	//�s���X�V
	(this->*update_)(_pPos,_atk);
	//���ʍX�V
	Rotation();
	UpdateRotQuat();

	animController_->Update();
	//�ʒu�ݒ�
	uiPos_ = pos_;
	//���ʒu
	uiPos_.y = uiDeviationY_;

	uiCntl_->Update();
}

void EnemyBase::InitAnim(void)
{
	
}

void EnemyBase::InitUI(void)
{
	//UI�R���g���[���[������
	uiCntl_ = std::make_unique<EnemyUIController>(uiPos_,state_);
	uiCntl_->Init(speciesName_);
	uiCntl_->CreateUI(speciesName_, hp_, maxHp_);
}




void EnemyBase::UpdateNomal(const VECTOR& _pPos, AttackManager& _atk)
{
	//�ړ�����
	(this->*move_)(_pPos);

	//�J�E���^����
	searchRestartCnt_++;

	//�T�[�`�������ԂɂȂ�����
	if (searchRestartCnt_ > SEARCH_RESTART_TIME) {
		//����
		auto deg = Utility::AngleDeg(GetForward(), VSub(_pPos, pos_));

		//���G�\�͈͓��ɓ�������
		if (Utility::MagnitudeF(VSub(_pPos, pos_)) <= ALERT_DISTANCE &&
			deg <= FIELD_VISION_DEG_HALF) {
			//���G��Ԃ�
			ChangeState(ENEMY_STATE::SEARCH);
		}
	}
	
}

void EnemyBase::UpdateSearch(const VECTOR& _pPos, AttackManager& _atk)
{
	//�ړ�����
	(this->*move_)(_pPos);
	
	//����
	auto deg = Utility::AngleDeg(GetForward(), VSub(_pPos, pos_));
	auto distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//���E���Ȃ�
	if (deg <= FIELD_VISION_DEG_HALF &&
		distance <= ALERT_DISTANCE) {
		//��莞�Ԃ�����
		if (searchCnt_ >= SEARCH_CNT_MAX) {
			//�퓬��Ԃ�
			SoundManager::GetInstance().Play("FindPlayer");
			ChangeState(ENEMY_STATE::BATTLE);
		}
		else {
			//�J�E���^����
			searchCnt_++;
		}
	}
	//�v���C���[�����G�͈͊O�ɏo����
	if (distance > ALERT_DISTANCE) {
		//�ʏ�ɖ߂�
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//�f�o�b�O
	debugRot_ = deg;
}

void EnemyBase::UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)
{
	//���̓��e�͏����L�����p�B�U�����ɂ͎~�܂��čU������
	//�����L�����N�^�[�͈ړ��U�����z�肷��̂ł����̏����Ƃ͏�������Ă�����
	//�v���C���[�Ƃ̋���
	float distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//�ړ�����
	if (distance >= ATTACK_DISTANCE) {
		(this->*move_)(_pPos);
	}
	else {
		OderGoalRot(_pPos);	//��]�̐ݒ肾���͍s��
	}
	
	//�J�E���^����(�Q�[���X�V�X�s�[�h)
	intervalCnt_+=SceneManager::GetInstance().GetUpdateSpeedRate_();	

	//����
	//�v���C���[���퓬��Ԕ͈͓x�O�ɂł���
	if (distance > BATTLE_FINISH_DISTANCE) {
		//�ʏ�ɖ߂�
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//�v���C���[���U���͈͓����U���\�ȊԊu���J���Ă���̂Ȃ�
	if (distance <= ATTACK_DISTANCE && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		//�U������
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
	//�X�e�C��Ԃ̂Ƃ�
	if (isStay_) {
		//�ҋ@�A�j���[�V����
		animController_->Play("idle", SPEED_ANIM);

		//���̎��Ԃ��߂��Ă�����
		if (stayCnt_ >= STAY_TIME) {
			//�s��̍Đݒ�
			//�s��̊p�x�ݒ�(characterRotY�ɕύX���ׂ�)
			VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//�J�����p�x
			float degRand = static_cast<float>(GetRand(static_cast<int>(Utility::CIRCLE_DEG)));
			float radRand = Utility::Deg2RadF(degRand) - cameraRot.y;
			//�N�H�[�^�j�I���ɕϊ�
			Quaternion quaGoal= Quaternion::AngleAxis((double)cameraRot.y + radRand, Utility::AXIS_Y);
			//�L�����N�^�[��]���ݒ�
			SetGoalRot(radRand);
			characterRotY_ = quaGoal;

			//�ړ��ʂ�͈͕t���̃����_���Ő���
			moveOneTime_ = static_cast<float>(GetRand(static_cast<int>(MOVE_RANDOM_MAX))) + MOVE_RANDOM_MIN;
			//�X�e�C��Ԃ̉���
			isStay_ = false;
			//�O���~�ʒu�̍X�V
			preStayPos_ = pos_;
		}
		else {
			//���������X�e�C
			//�J�E���^����(�Q�[���X�V�X�s�[�h)
			stayCnt_+= SceneManager::GetInstance().GetUpdateSpeedRate_();
			return;
		}
	}
	
	//�ړ�(�O������)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSped_* SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("walk", SPEED_ANIM);

	//����
	//��~�����ʒu����ǂꂾ������Ă��邩
	float diffNow2Pre = Utility::MagnitudeF(VSub(pos_, preStayPos_));

	//����̈ړ��ʈȏ�̒l�̎�
	if (diffNow2Pre >=moveOneTime_) {
		//�X�e�C��Ԃ�
		isStay_ = true;
		stayCnt_ = 0;
	}

	
}

void EnemyBase::MoveSearch(const VECTOR& _pPos)
{
	//�����\��
	//���G�̂Ђ�����
	//�ق��̃I�u�W�F�N�g�̎g�p�ɂ����ʂȂ̂ŏ����ł͍��Ȃ�
}

void EnemyBase::MoveBattle(const VECTOR& _pPos)
{
	//��~���Ԃ��܂�����ꍇ
	if (stopTime_ > 0) {

		stopTime_--;
		return;
	}
	//�ړ�(�O������)
	pos_=VAdd(pos_, VScale(GetForward(), moveSped_* SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("dush", SPEED_ANIM);

	OderGoalRot(_pPos);
}


void EnemyBase::OderGoalRot(const VECTOR _pPos) {
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
		//�ҋ@�A�j���[�V����
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
		//���S�A�j���[�V����
		animController_->Play("dethStart", SPEED_ANIM,{"dethSus"});

		break;
	case ENEMY_STATE::END:
		//�폜�\��
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
	//�����}�[�N
	if (state_ == ENEMY_STATE::SEARCH|| state_ == ENEMY_STATE::BATTLE) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::FIND);
	}

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
	////�O��������
	//VECTOR fowardPos = VAdd(pos_, characterRotY_.PosAxis(forwardDif));
	//DrawSphere3D(fowardPos, 8, 8, 0xff5500, 0xff0000, false);
	////����p
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
	//�U���͕����炷
	hp_ -= static_cast<int>(_pow);
	//�퓬��Ԃł͂Ȃ�������
	if (state_ != ENEMY_STATE::BATTLE) {
		//�퓬��Ԃ�
		SoundManager::GetInstance().Play("FindPlayer");
		ChangeState(ENEMY_STATE::BATTLE);
	}
	//0�ȉ��̂Ƃ�
	if (hp_ <= 0) {
		//���S����
		Deth();
	}
}

void EnemyBase::Deth(void)
{
	ChangeState(ENEMY_STATE::DETH);
}

void EnemyBase::Shout(void)
{
	//�{�X��p
}
