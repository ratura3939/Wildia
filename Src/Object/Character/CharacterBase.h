#pragma once
#include<DxLib.h>
#include<memory>
#include "../../Common/Quaternion.h"
#include"../../Manager/GameSystem/AnimationController.h"

//class AnimationController;
class AttackManager;
class ModelMaterial;
class ModelRenderer;
class Game;

class CharacterBase
{
public:
	//�������p
	static constexpr float INIT_MODEL_ROT = 180.0f;	//Unity�`���̃��f���̌`�����킹��p

	//��]���
	static constexpr float PER_ROT = 0.1f;			//�t���[�����Ƃ̉�](���ʕ�Ԃɂ����鎞�Ԃ̑����ʂ�\��)
	static constexpr float THRESHOLD_ROT = 0.1f;	//��]�̂������l��\��

	//��]��
	static constexpr float DEG_FORWARD = 0.0f;	//�O��
	static constexpr float DEG_BACK = 180.0f;	//���
	static constexpr float DEG_LEFT = 270.0f;	//����
	static constexpr float DEG_RIGHT = 90.0f;	//�E��

	static constexpr VECTOR CHARACTER_HEIGHT = { 0.0f,150.0f,0.0f };	//�L�����N�^�[�̐g��
	static constexpr float CHARACTER_RADIUS = 80.0f;					//�L�����N�^�[�̔���̑傫��

	//�f�o�b�O�p
	static constexpr int COLOR = 0xffffff;

	CharacterBase(void);
	virtual ~CharacterBase(void);

	virtual const bool Init(const int _num) = 0;
	virtual void Update(void);
	virtual void Update(const VECTOR _pPos, AttackManager& _atk);	//�G�p(�G�̓L�����N�^�[���g���s�������߂�̂ł���p�ɕ����Ă���)
	void UpdateAnimOnly(void);
	virtual void Draw(void);
	virtual const bool Release(void);

	// �e��������擾
	VECTOR GetForward(void) const;
	VECTOR GetBack(void) const;
	VECTOR GetRight(void) const;
	VECTOR GetLeft(void) const;
	VECTOR GetUp(void) const;
	VECTOR GetDown(void) const;

	// �Ώە������擾
	VECTOR GetDir(const VECTOR& _vec) const;

	//���W�擾
	const VECTOR GetPos(void)const;				//����
	const VECTOR GetHeight(void)const;			//���ӂ�

	//�����蔻��̔��a
	const float GetCollisionRadius(void)const;

	void SetPrePos(void);

	//��]���̎擾
	const Quaternion GetQua(void)const;

	//�̖��擾
	const std::string GetSpeciesName(void)const;

	//����
	virtual const bool IsAlive(void)const;

	//�_���[�W��^����
	virtual void Damage(const float _pow) = 0;

	//���S������
	virtual void Deth(void);

	//�f�o�b�O
	virtual void DrawDebug(void);
	void DrawCupcel(void);

protected:
	//�K���X�V�E�����������ɓ���邱��
	//************************************************
	virtual void InitAnim(void) = 0;
	virtual void InitUI(void) = 0;
	void UpdateRotQuat(void);
	//**********************************************
	//UI�`��
	virtual void DrawUI(void) = 0;

	//��]�ڕW�p�x
	void SetGoalRot(const float _rad);
	//��]
	void Rotation(void);

#pragma region ���f����b����
	int modelId_;	//���f��ID

	VECTOR pos_;	//���W
	VECTOR prePos_;	//���W
	VECTOR uiPos_; //UI�\���ʒu
	VECTOR scl_;	//���f���傫��
	VECTOR rot_;	//��]���(XYZ)
	Quaternion characterRotY_;	//Y����]�p

	Quaternion goalQua_;	//�ڕW�̉�]��
	float stepRotation_;	//��]�̃J�E���^�[

	//�e���̍s��
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// ��]
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ���[�J����]
	Quaternion quaRotLocal_;
#pragma endregion

	

#pragma region �`��֘A
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
#pragma endregion

	//�̖�
	std::string speciesName_;

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animController_;

	//�X�e�[�^�X�Ȃ�
	float hp_;		
	float colRadius_;	//�����蔻��̑傫��
};

