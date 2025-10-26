#include <chrono>
#include<EffekseerForDXLib.h>
#include "../../Common/Fader.h"
#include "../../Application.h"
#include "../../Scene/Main/Title.h"
#include"../Decoration/EffectManager.h"
#include"../Decoration/SoundManager.h"
#include"../Decoration/UIManager2d.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//�G�t�F�N�g�E�T�E���h�̐���
	SoundManager::CreateInstance();
	EffectManager::CreateInstance();

	//UI�}�l�[�W���̐���
	UIManager2d::CreateInstance();

	fader_ = new Fader();
	fader_->Init();

	// �J����
	camera_ = std::make_shared<Camera>();
	camera_->Init();

	isSceneChanging_ = false;
	nextScene_ = nullptr;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	SetInitScene(std::make_shared<Title>());

	// ���C���X�N���[��
	mainScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void SceneManager::Init3D(void)
{

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);

	// ���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir(LIGHT_DIR);

}

void SceneManager::Update(void)
{

	if (scenes_.empty())
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	auto checkDelta = nowTime - preTime_;

	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);



	totalTime_ += deltaTime_;
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		//�ŐV�̃V�[���������X�V
  		scenes_.back()->Update();
		SoundManager::GetInstance().Update();
		EffectManager::GetInstance().Update();
	}

	// �J�����X�V
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(mainScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	//�J����
	camera_->SetBeforeDraw();

	//�G�t�F�N�V�A�X�V
	UpdateEffekseer3D();

	//�V�[���̉��w���珇�ɕ`��
	for (auto& scene : scenes_) {
		scene->Draw();
	}

	//�G�t�F�N�V�A�`��
	DrawEffekseer3D();

	// �Ó]�E���]
	fader_->Draw();

	// �w�ʃX�N���[���Ƀ��C���X�N���[����`��
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen_, true);
}

void SceneManager::Destroy(void)
{
	//�G�t�F�N�g�E�T�E���h�̍폜
	SoundManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	UIManager2d::GetInstance().Destroy();

	scenes_.clear();

	DeleteGraph(mainScreen_);
	//delete scene_;

	delete fader_;

	camera_->Release();

	delete instance_;

}


void SceneManager::SetInitScene(std::shared_ptr<SceneBase> _scene)
{
	nextScene_ = _scene;
	DoChangeScene();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> _scene)
{
	nextScene_ = _scene;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> _scene)
{
	_scene->Init();
	scenes_.push_back(_scene);
}

void SceneManager::PopScene(void)
{
	scenes_.back()->Release();
	scenes_.pop_back();
}


float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

Camera& SceneManager::GetCamera(void) const
{
	return *camera_;
}

const SceneManager::CNTL SceneManager::GetController(void) const
{
	return cntl_;
}

void SceneManager::SetController(const CNTL _cntl)
{
	cntl_ = _cntl;
}

void SceneManager::SwitchController(void)
{
	if (cntl_ == CNTL::KEY) {
		SetController(CNTL::PAD);
	}
	else if(cntl_==CNTL::PAD) {
		SetController(CNTL::KEY);
	}
}

void SceneManager::SetUpdateSpeedRate_(const float _percent)
{
	updateSpeedRate_ = _percent / 100.0f;
}

const float SceneManager::GetUpdateSpeedRate_(void) const
{
	return updateSpeedRate_;
}

const float SceneManager::GetUpdateSpeedRatePercent_(void) const
{
	return updateSpeedRate_ * 100.0f;
}

const float SceneManager::GetScaleUpdateSpeedRate(const float _target) const
{
	return _target * updateSpeedRate_;
}

SceneManager::SceneManager(void)
{
	cntl_ = CNTL::NONE;

	mainScreen_ = -1;

	fader_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;
	totalTime_ = 0.0f;

	updateSpeedRate_ = 1.0f;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{	


	//���
	auto& resM = ResourceManager::GetInstance();
	auto& sndM = SoundManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();

	//���
	for (auto& scene : scenes_) {
		scene->Release();
	}

	// ���\�[�X�̑S���
	resM.Release();
	sndM.Release();
	uiM.Relese();


	//���̃V�[��������
	nextScene_->Init();

	
	scenes_.clear();

	//���̃V�[��������
	scenes_.push_back(nextScene_);

	ResetDeltaTime();

	nextScene_ = nullptr;
}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene();
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}