#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"Title.h"
#include"../../Object/Stage/Stage.h"
#include "GameOver.h"

//���[�J���萔
namespace {
	std::string GAME_OVER_LOGO = "OverLogo";
	std::string BACK_TITLE_LOGO = "BackTitle";
}

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	auto& uiM = UIManager2d::GetInstance();
	auto& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//���\�[�X����
	rsM.GetInstance().Init(SceneManager::SCENE_ID::GAMEOVER);

	//�߂�A�C�R��
	uiM.Add(GAME_OVER_LOGO, rsM.Load(ResourceManager::SRC::GAMEOVER_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(GAME_OVER_LOGO, VECTOR{ Application::SCREEN_SIZE_X / 2.0f,Application::SCREEN_SIZE_Y / 2.0f - 300.0f,0.0f }, 0.6f);

	//�^�C�g���߂�
	uiM.Add(BACK_TITLE_LOGO, rsM.Load(ResourceManager::SRC::STOP_GAME_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_TITLE_LOGO, VECTOR(Application::SCREEN_SIZE_X / 2.0f, Application::SCREEN_SIZE_Y / 2.0f + 50.0f, 0.0f), 0.6f);											//��b�ݒ�
	uiM.SetUIDirectionPram(BACK_TITLE_LOGO, UI_GROUP::ZOOM, 0.01f, 0.7f, 0.55f);//�ڍאݒ�

	//�X�e�[�W
	stage_ = std::make_unique<Stage>(true);
	stage_->Init();

	InitSound();

	//�J�������Œ��
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetDefault();
}

void GameOver::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::GAMEOVER_BGM).handleId_);
	//BGM�Đ�
	sndM.Play("NomalBgm");

	//����
	sndM.Add(SoundManager::TYPE::SE, "Enter",
		rsM.Load(ResourceManager::SRC::ENTER_CNTL_SE).handleId_);
}

void GameOver::InitEffect(void)
{
}

void GameOver::Update(void)
{
	InputManager& ins = InputManager::GetInstance();

	//���肪�����ꂽ�Ƃ�
	if (ins.IsTrigerrDown("action")){
		//�V�[���J��
		SoundManager::GetInstance().Stop("NomalBgm");
		SceneManager::GetInstance().ChangeScene(std::make_shared<Title>());
	}

	UIManager2d::GetInstance().Update(BACK_TITLE_LOGO);
}

void GameOver::Draw(void)
{
	auto& uiM = UIManager2d::GetInstance();

	stage_->Draw();
	uiM.Draw({ GAME_OVER_LOGO,BACK_TITLE_LOGO });
}

void GameOver::Release(void)
{
	
}

void GameOver::Reset(void)
{
}
