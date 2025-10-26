#pragma once
#include <chrono>
#include<vector>
#include<memory>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

//��`
//�V�[���J��::���C���V�[���̐؂芷��
//�V�[���ړ�::�T�u�V�[���Ƃ̈ړ��̂����

class SceneManager
{

public:
	static constexpr VECTOR LIGHT_DIR = { 0.00f, -1.00f, 1.00f };
	static constexpr float STICK_START_POW = 0.5f;	//�R���g���[���[�̃X�e�B�b�N臒l

	static constexpr int POP_SCENE_TYPE_NUM = 3;	//�|�b�v�\�ȃV�[���̎�ސ�

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,

		//�|�b�v�\�V�[��
		PAUSE,
		KEY_CONFIG,
		SWITCH_CNTL,
	};

	enum class CNTL
	{
		NONE,
		KEY,
		PAD,
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	//�V�[���J��
	void SetInitScene(std::shared_ptr<SceneBase>_scene);	//���������̂ݎg�p
	/// <summary>
	/// �V�[���̕ύX
	/// </summary>
	/// <param name="_scene">���C���V�[��(Scene/Main/)</param>
	void ChangeScene(std::shared_ptr<SceneBase>_scene);
	/// <summary>
	/// �ǉ��V�[���̐���
	/// </summary>
	/// <param name="_scene">�ǉ��V�[��</param>
	void PushScene(std::shared_ptr<SceneBase>_scene);
	/// <summary>
	/// �ǉ�����Ă���V�[������폜�i�ŐV�̃V�[���j
	/// </summary>
	void PopScene(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	Camera& GetCamera(void) const;

	//�����ʂ̎擾�E�ݒ�
	const CNTL GetController(void)const;
	void SetController(const CNTL _cntl);
	void SwitchController(void);	//�؂芷��

	//�X�V�����̃X�s�[�h�{��
	void SetUpdateSpeedRate_(const float _percent);	//�ݒ�
	/// <summary>
	/// �{���P�̂��擾(�C���N�������g�Ȃ炱����g�p)
	/// </summary>
	/// <returns>�{��</returns>
	const float GetUpdateSpeedRate_(void)const;
	/// <summary>
	/// �{���P��<%�\�L>���擾(�C���N�������g�Ȃ炱����g�p)
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetUpdateSpeedRatePercent_(void)const;
	/// <summary>
	/// �{�����܂߂Čv�Z�������̂�ԋp
	/// </summary>
	/// <param name="_target">�X�V�X�s�[�h</param>
	/// <returns>�X�V�X�s�[�h(�{���e���ς�)</returns>
	const float GetScaleUpdateSpeedRate(const float _target)const;

	int GetMainScreen(void)const { return mainScreen_; }
	const float GetTotalTime(void)const { return totalTime_; }

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	//�R���g���[������
	CNTL cntl_;

	std::shared_ptr<Camera> camera_;

	// ���C���X�N���[��
	int mainScreen_;

	// �t�F�[�h
	Fader* fader_;

	// �e��V�[��
	std::vector<std::shared_ptr<SceneBase>> scenes_;	//�V�[���i�[
	std::shared_ptr<SceneBase>nextScene_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	float totalTime_;

	//�X�V�J�E���^�[�̔{��
	float updateSpeedRate_;
	
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(void);

	// �t�F�[�h
	void Fade(void);
};