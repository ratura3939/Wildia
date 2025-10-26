#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../../Common/Quaternion.h"

class Transform;

class Camera
{

public:
	//�J�����̕`���(Near,Far)�֘A�̒萔------------------------------------------------------
	
	static constexpr float SPEED = 10.0f;			//�J�����X�s�[�h�FNEAR

	
	static constexpr float CAMERA_NEAR = 40.0f;		//�J�����N���b�v�FNEAR

	
	static constexpr float CAMERA_FAR = 15000.0f;	//�J�����N���b�v�FNEAR

	//�J�������W�֘A�̒萔---------------------------------------------------------------------
	
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 700.0f, -700.0f };			//�J�����̏������W

	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };			//�J�����ʒu���璍���_�܂ł̑��΍��W

	
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 700.0f, -600.0f };	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])

	//static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�΂˕t��)

	static constexpr float ROCK_DISTANCE_MIN = 500.0f;		//���b�N�I�����ɍŒ������Ă�������

	static constexpr float ROCK_MAGNIFICATION_Y = 0.25f;	//���b�N�I������Y���W�����p�̔{��
	
	//�J�����ړ��֘A�̒萔---------------------------------------------------------------------
	
	static constexpr float MAX_MOVE_SPEED = 5.0f;	//�ړ����x�̍ő�l
	static constexpr float MAX_ROT_SPEED = 0.025f;	//�ړ����x�̍ő�l

	//�J�����h�炵�֘A�̒萔--------------------------------------------------------------------

	static constexpr float TIME_SHAKE = 0.5f;		//����

	static constexpr float WIDTH_SHAKE = 5.0f;		//��

	static constexpr float SPEED_SHAKE = 40.0f;		//�X�s�[�h

	//���Z�b�g�֌W
	static constexpr float RESET_TIME = 1.0f;
	static constexpr float RESET_STEP = 0.05f;

	//�����Œᐧ��
	static constexpr float UNDER_LIMIT_Y = 280.0f;

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 45.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = -25.0f * (DX_PI_F / 180.0f);

	//���`�⊮
	static constexpr float NO_LERP = 1.0f;
	static constexpr float LERP_SPEED = 0.05f;

	//�J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		FOLLOW_SPRING,	//�΂˕t���Ǐ]���[�h
		SHAKE,			//�J�����h�炵
		LOCKON,			//���b�N�I��
		RESET,			//�J�����ʒu���Z�b�g�p
		AUTO_MOVE,		//�ڕW�ʒu�܂Ŏ����I�Ɉړ�
	};

	struct FOR_FOLLOW_INFO
	{
		VECTOR pos;
		Quaternion quaRot;
	};

	//�R���X�g���N�^
	Camera(void);

	//�f�X�g���N�^
	~Camera(void);

	//����������
	void Init(void);

	//�X�V����
	void Update(void);

	//�J�����̕`�惂�[�h�֘A------------------
	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//��_�J����
	void SetBeforeDrawFree(void);			//�t���[�J����
	void SetBeforeDrawFollow(void);			//�Ǐ]�J����
	void SetBeforeDrawLockOn(void);			//���b�N�I���J����
	void SetBeforeDrawShake(void);			//�J�����V�F�C�N
	void SetBeforeDrawReset(void);			//�J�������Z�b�g
	void SetBeforeDrawAutoMove(void);			//�J�������Z�b�g

	//----------------------------------------
	// �`�揈��
	void Draw(void);

	//�������
	void Release(void);

	//���W�擾
	const VECTOR GetPos(void) const;
	const VECTOR GetRockPos(void)const;
	const VECTOR GetGoalPos(void)const { return goalPos_; }

	//��]�擾
	const Quaternion GetRot(void)const;
	const VECTOR GetAngle(void)const;
	//��]�X�s�[�h
	const float GetRotSpeed(void)const;
	void SetRotSpeed(const float _speed);

	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const VECTOR _pos,const Quaternion _qua);


	//���W�ݒ�
	void SetPos(const VECTOR& pos,const VECTOR& focus);
	void SetPos(const VECTOR& pos);
	void SetFocusPos(const VECTOR& _focus);
	void SetRockPos(const VECTOR& _rock);
	void SetGoalPos(const VECTOR& _goal);

	//�J�����������ʒu�ɖ߂�
	void SetDefault(void);


	const MODE GetMode(void);
	const bool IsFinishShake(void) { return finishShake_; }

	void DrawDebug(void);
private:

	//�Ǐ]�Ώ�
	FOR_FOLLOW_INFO followObject_;
	//�J�n�E�ڕW�ʒu(���Z�b�g���ȂǂɎg�p)
	FOR_FOLLOW_INFO start_;
	FOR_FOLLOW_INFO goal_;
	float stepReset_;
	bool isReset_;

	//�J�������[�h
	MODE mode_;			
	MODE currentMode_;	//Shake�EReset�ɑJ�ڂ���ۂɌ��݂̕���ۑ�����

	//�J�����̈ʒu
	VECTOR pos_;

	//���b�N�I���Ώۂ̈ʒu
	VECTOR lockPos_;
	VECTOR prevGoalPos_;
	VECTOR lockOnGoalPos_;
	float lockOnLerpStep_;

	//�J�����̒����_
	VECTOR focusPos_;
	VECTOR goalFocusPos_;
	
	//�ړ��ڕW�ʒu
	VECTOR goalPos_;

	//�J�����̏����
	VECTOR cameraUp_;

	//�J�����̉�]
	Quaternion rot_;

	//��]�X�s�[�h
	float rotSpeed_;

	// �J�����p�x(rad)
	VECTOR angles_;
	// X����]�������p�x
	Quaternion rotOutX_;

	//��ʗh�炵�p
	float stepShake_;

	//��ʗh�ꂪ�I�������
	bool finishShake_;
	//���o�O�ʒu
	VECTOR defaultPos_;
	//�h�����
	VECTOR shakeDir_;
	//�⊮�X�s�[�h
	float lerpSpeed_;

	//��]
	void Rotation(void);

};

