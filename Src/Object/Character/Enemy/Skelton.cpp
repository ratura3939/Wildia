#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include "Skelton.h"

namespace {
#pragma region �A�j���[�V�����֘A
    const int ANIM_IDLE = 42;    //�ҋ@

    //�U��(�p���悲�ƂňႤ�̂ōŏI�I�ɂ͂����ł͂Ȃ�)
    const int ANIM_ATTACK_NOMAL = 9;     //�U��

      //���̑��A�N�V����
    const int ANIM_WALK = 94;            //����
    const int ANIM_DUSH_FORWARD = 55;    //�_�b�V��(�O��)

    //���o
    const int ANIM_DETH_START = 29;      //���S�J�n
    const int ANIM_DETH_SUSTANABLE = 28; //���S����
#pragma endregion
}

Skelton::Skelton(VECTOR& _pos) :EnemyBase(_pos)
{
}

Skelton::~Skelton(void)
{
}

void Skelton::InitAnim(void)
{
    animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("attack", ANIM_ATTACK_NOMAL, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dush", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void Skelton::SetPram(void)
{
	//�e�G����
	//��XJson�������y�ɂȂ邩���H
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);

	if (modelId_ == -1) {
		return;
	}
	//�p�����[�^�֌W
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//�������p�Ɉ����s
	UpdateRotQuat();

	//�����蔻��傫��
	colRadius_ = CHARACTER_RADIUS;

	//�U���̔����ʒu(���΍��W)
	atkRelative_ = RELATIVE_ATTACK_POS;
	//�U���̑傫��
	atkScale_ = SCALE_ATTACK_NOMAL;
	//�U���\����
	atkDistance_ = ATTACK_DISTANCE;

	//�A�j���[�V����������
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	uiDeviationY_ = 250.0f;
	maxHp_ = ENEMY_HP;

	//�ʒu�ݒ�
	uiPos_ = pos_;
	//���ʒu
	uiPos_.y += uiDeviationY_;

	//UI������
	InitUI();

	//���f���`��N���X����
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS


	//PS
	//�e�F�̋���(�g�U��)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//�u���[�̋���(�ŏ��̍��ڂ̂݊֌W����)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//�T���v����(�ŏ��̍��ڂ̂݊֌W����)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });

	intervalCnt_ = 0.0f;

	//��Ԃ�ʏ��
	ChangeState(ENEMY_STATE::NOMAL);
}
