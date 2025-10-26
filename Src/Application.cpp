#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<Windows.h>
#include "Manager/Generic/ResourceManager.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_UI = "Data/Image/UI/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_CHARACTER = "Data/Model/Character/";
const std::string Application::PATH_STAGE = "Data/Model/Stage/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SE = "Data/Sound/SE/";
const std::string Application::PATH_SHADER = "Data/Shader/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("Wildea");


	// ���s���E�B���h�E�����郂�j�^�[���擾
	HMONITOR hMonitor = MonitorFromWindow(GetMainWindowHandle(), MONITOR_DEFAULTTONEAREST);

	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	// ���j�^�[�̕��E����
	width_ = mi.rcMonitor.right - mi.rcMonitor.left;
	height_ = mi.rcMonitor.bottom - mi.rcMonitor.top;

	// �E�B���h�E�T�C�Y
	SetGraphMode(width_, height_, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}
	//�G�t�F�N�V�A������
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//�}�E�X�J�[�\����\��
	SetMouseDispFlag(FALSE);

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

	//FPS�p������
	currentFrame_ = 0;
	lastFrame_ = 0;

}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Sleep(1);	//�V�X�e���ɏ�����Ԃ�
		currentFrame_ = GetNowCount();	//���݂̃t���[�������l��

		//���݂̃t���[���ƍŌ�̎��s�t���[���̍��������l�𒴂�����X�V�������s���B
		if (currentFrame_ - lastFrame_ >= FRAME_RATE)
		{
			lastFrame_ = currentFrame_;	//�t���[���̍X�V
			inputManager.Update();
			sceneManager.Update();	//�X�V
		}

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	
	// Effekseer���I������B
	Effkseer_End();
	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
