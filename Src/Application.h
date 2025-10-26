#pragma once
#include <string>

class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;

	static constexpr float FRAME_RATE = 1000.0f / 60.0f;
	static constexpr float DEFAULT_FPS = 60.0f;

	// �f�[�^�p�X�֘A
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_UI;
	static const std::string PATH_MODEL;
	static const std::string PATH_CHARACTER;
	static const std::string PATH_STAGE;
	static const std::string PATH_EFFECT;
	static const std::string PATH_BGM;
	static const std::string PATH_SE;
	static const std::string PATH_SHADER;
	//-------------------------------------------

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

	//�X�N���[���T�C�Y�擾
	const int GetWindowWidth(void) { return width_; }
	const int GetWindowHeight(void) { return height_; }

private:

	//�t���[���Œ�p
	int currentFrame_;	//���݂̃t���[����ۑ�
	int lastFrame_;		//�Ō�Ɏ��s�����t���[����ۑ�

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	int width_;
	int height_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);

	// �R�s�[�R���X�g���N�^�����l
	Application(const Application&);

	// �f�X�g���N�^�����l
	~Application(void) = default;

};