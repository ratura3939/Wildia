#pragma once

//��`
//�V�[���J��::���C���V�[���̐؂芷��
//�V�[���ړ�::�T�u�V�[���Ƃ̈ړ��̂����

class SceneBase
{

public:
#pragma region �T�u�V�[�����ʒ萔
	//�ꖇ�G
	static constexpr float IMG_EX_SB_SCENE = 0.8f;			//�摜�g�嗦

	//�{�^��
	//�g��k��
	static constexpr float BTN_EX_MAX_SB_SCENE = 0.3f;		//���
	static constexpr float BTN_EX_MIN_SB_SCENE = 0.2f;		//����
	static constexpr float BTN_EX_ACC_SB_SCENE = 0.01f;	//�X�s�[�h
	//�ʒu����
	static constexpr float BTN_DIFF_X_SB_SCENE = 250.0f;		//X
	static constexpr float BTN_DIFF_Y_SB_SCENE = 100.0f;		//Y
#pragma endregion

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void) = 0;

	// ����������
	virtual void Init(void) = 0;
	virtual void InitSound(void) = 0;
	virtual void InitEffect(void) = 0;

	// �X�V�X�e�b�v
	virtual void Update(void) = 0;

	// �`�揈��
	virtual void Draw(void) = 0;

	// �������
	virtual void Release(void) = 0;

	//�󋵉�A(�V�[���ړ���̏󋵗p)
	virtual void Reset(void) = 0;
};
