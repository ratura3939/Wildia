#include <DxLib.h>
#include<memory>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(SceneManager::SCENE_ID _scene)
{
	//�V�[�����Ƃ̃��\�[�X�Ɛ�������\��������ǉ��V�[���̃��\�[�X
	switch (_scene)
	{
	case SceneManager::SCENE_ID::TITLE:
		InitTitle();
		break;
	case SceneManager::SCENE_ID::GAME:
		InitGame();
		InitPouse();
		InitKeyConfig();
		InitSwitchCntl();
		break;
	case SceneManager::SCENE_ID::GAMEOVER:
		InitGameOver();
		break;
	case SceneManager::SCENE_ID::CLEAR:
		InitClear();
		break;
	default:
		break;
	}
}


void ResourceManager::InitTitle(void)
{
	Resource res;

	// �^�C�g��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);
	// �^�C�g��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ShadowLogo.png");
	resourcesMap_.emplace(SRC::SHADOW_LOGO, res);

	// �w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleBack.png");
	resourcesMap_.emplace(SRC::TITLE_BACK_BTN, res);

	//�X�e�[�W
	ResorceStage();

	// �f�o�C�X�A�C�R��
	ResorceDeviceIcon();

	//���
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_ArrowDown.png");
	resourcesMap_.emplace(SRC::ARROW_DOWN_IMG, res);
	//��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Exit.png");
	resourcesMap_.emplace(SRC::EXIT_IMG, res);
	//�X�^�[�g�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "StartBtn.png");
	resourcesMap_.emplace(SRC::START_GAME_BTN, res);

	//�N���b�N���Ă����������Ă�������
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "ClickStr.png");
	resourcesMap_.emplace(SRC::CLICK_STR_IMG, res);

	//��
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Title3.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);
	

}

void ResourceManager::InitGame(void)
{
	Resource res;

	//�v���C���[
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MDL, res);

	//�G
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + "EneAxe.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + "lightGolem.mv1");
	resourcesMap_.emplace(SRC::BOSS_MDL, res);

	//�X�e�[�W
	ResorceStage();

	//UI
	//���j���[�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "MenuBtn.png");
	resourcesMap_.emplace(SRC::MENU_BTN, res);

	//?�}�[�N
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Question.png");
	resourcesMap_.emplace(SRC::SUSPECT_IMG, res);
	//�I�}�[�N
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Exclamation.png");
	resourcesMap_.emplace(SRC::FIND_IMG, res);
	//HP
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "HP.png");
	resourcesMap_.emplace(SRC::HEART_IMG, res);

	//HP(���)
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "HP_BROKEN.png");
	resourcesMap_.emplace(SRC::HEART_BROKEN_IMG, res);
	//HP(��)
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "HP_EMPTY.png");
	resourcesMap_.emplace(SRC::HEART_EMPTY_IMG, res);

	//�^�[�Q�b�g�\��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_AnnounceRockOn.png");
	resourcesMap_.emplace(SRC::ANNOUNCE_LOCKON_IMG, res);
	//���b�N�I��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_RockOn.png");
	resourcesMap_.emplace(SRC::LOCKON_IMG, res);

	//�v���[�g
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "PlateWood.png");
	resourcesMap_.emplace(SRC::PLATE_IMG, res);
	//�X�P���g���~
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "SkeltonCounter.png");
	resourcesMap_.emplace(SRC::SKELTON_IMG, res);
	//�S�[�����~
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "GolemCounter.png");
	resourcesMap_.emplace(SRC::GOLEM_IMG, res);
	
	//����
	res = Resource(Resource::TYPE::IMGS, Application::PATH_UI + "PlateNumber.png", 4, 2, 240, 350);
	resourcesMap_.emplace(SRC::NUMBER_IMGS, res);

	//WARNING
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Warning.png");
	resourcesMap_.emplace(SRC::WARNING_IMG, res);

	//�W����
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FocusLine.png");
	resourcesMap_.emplace(SRC::FOCUS_IMG, res);
	//�W����
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "DudgeAllertp.png");
	resourcesMap_.emplace(SRC::ATK_ALLERT_IMG, res);

;
	//��
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game.mp3");
	resourcesMap_.emplace(SRC::GAME_BGM, res);
	//�o�g��BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Battle.mp3");
	resourcesMap_.emplace(SRC::BATTLE_BGM, res);
	//�o�g��BGM(�{�X)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Boss.mp3");
	resourcesMap_.emplace(SRC::BOSS_BGM, res);

	//�x����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Warning.mp3");
	resourcesMap_.emplace(SRC::WARNING_BGM, res);
	//����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Walk.mp3");
	resourcesMap_.emplace(SRC::WALK_SE, res);
	//����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Run.mp3");
	resourcesMap_.emplace(SRC::RUN_SE, res);
	//���U��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "SwingSword.mp3");
	resourcesMap_.emplace(SRC::SWING_SWORD_SE, res);
	//���b�N�I��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "RockOn.mp3");
	resourcesMap_.emplace(SRC::LOCK_ON_SE, res);
	//�G�������I
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "FindPlayer.mp3");
	resourcesMap_.emplace(SRC::FIND_PLAYER_SE, res);
	//�_���[�W
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Damage.mp3");
	resourcesMap_.emplace(SRC::DAMAGE_SE, res);
	//�{�X����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Impact.mp3");
	resourcesMap_.emplace(SRC::BOSS_IMPACT_SE, res);
	//�{�X����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "BossShout.mp3");
	resourcesMap_.emplace(SRC::BOSS_SHOUT_SE, res);
	

	//�U���x����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Allert.mp3");
	resourcesMap_.emplace(SRC::ATK_ALLERT_SE, res);

	//�����
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Dodge_3.mp3");
	resourcesMap_.emplace(SRC::DODGE_SE, res);

	//�W���X�g��𐬌���
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Dodge_4.mp3");
	resourcesMap_.emplace(SRC::JUST_DODGE_SE, res);

	//�G�t�F�N�g
	//��
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SwordEfc.efkefc");
	resourcesMap_.emplace(SRC::SWORD_EFC, res);
	//�_���[�W
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFC, res);
}

void ResourceManager::InitClear(void)
{
	Resource res;

	//���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameClear.png");
	resourcesMap_.emplace(SRC::GAMECLAR_LOGO, res);

	//�X�e�[�W
	ResorceStage();

	//�Q�[����߂�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "StopGameBtn.png");
	resourcesMap_.emplace(SRC::STOP_GAME_BTN, res);

	//��
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameClear.mp3");
	resourcesMap_.emplace(SRC::GAMECLEAR_BGM, res);

	//�I��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "EnterController.mp3");
	resourcesMap_.emplace(SRC::ENTER_CNTL_SE, res);

	
}

void ResourceManager::InitGameOver(void)
{
	Resource res;
	// ���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER_LOGO, res);

	//�X�e�[�W
	ResorceStage();

	//�Q�[����߂�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "StopGameBtn.png");
	resourcesMap_.emplace(SRC::STOP_GAME_BTN, res);

	//��
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameOver.mp3");
	resourcesMap_.emplace(SRC::GAMEOVER_BGM, res);
	//�I��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "EnterController.mp3");
	resourcesMap_.emplace(SRC::ENTER_CNTL_SE, res);

}

void ResourceManager::InitPouse(void)
{
	Resource res;
	//�Q�[����߂�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "StopGameBtn.png");
	resourcesMap_.emplace(SRC::STOP_GAME_BTN, res);

	//���g���C�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "RetryBtn.png");
	resourcesMap_.emplace(SRC::RETRY_IMG, res);

	//�Q�[���߂�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "BackGameBtn.png");
	resourcesMap_.emplace(SRC::BACK_GAME_BTN, res);

	//������@�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "CheckConfigBtn.png");
	resourcesMap_.emplace(SRC::CHECK_CONFIG_IMG, res);

	//����؂芷���{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "SwitchOperationBtn.png");
	resourcesMap_.emplace(SRC::SWITCH_OPERATOR_IMG, res);

	//���
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_ArrowDown.png");
	resourcesMap_.emplace(SRC::ARROW_DOWN_IMG, res);

	//�߂�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "BackBtn.png");
	resourcesMap_.emplace(SRC::BACK_BTN, res);
}

void ResourceManager::InitKeyConfig(void)
{
	Resource res;
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Description_Pad.png");
	resourcesMap_.emplace(SRC::DESCRIPTION_PAD, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Description_Key.png");
	resourcesMap_.emplace(SRC::DESCRIPTION_KEY, res);
}

void ResourceManager::InitSwitchCntl(void)
{
	Resource res;

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ConfirmSwitch.png");
	resourcesMap_.emplace(SRC::CONFIRM_SWITCH, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FinishSwitch.png");
	resourcesMap_.emplace(SRC::FINISH_SWITCH, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "SwitchBtn.png");
	resourcesMap_.emplace(SRC::SWITCH_BTN, res);
}

void ResourceManager::ResorceDeviceIcon(void)
{
	Resource res;
	// PAD
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "IconPad.png");
	resourcesMap_.emplace(SRC::PAD_IMG, res);
	//Key
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "IconKeyboard.png");
	resourcesMap_.emplace(SRC::KEYBOARD_IMG, res);

	//�L�����Z��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SE, res);
	//�J�[�\������
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "MoveCursur.mp3");
	resourcesMap_.emplace(SRC::MOVE_CURSUR_SE, res);
	//�I��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "EnterController.mp3");
	resourcesMap_.emplace(SRC::ENTER_CNTL_SE, res);
}

void ResourceManager::ResorceStage(void)
{
	Resource res;

	//�X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage1.mv1");
	resourcesMap_.emplace(SRC::OBJECTS_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "deco1.mv1");
	resourcesMap_.emplace(SRC::STAGE_MDL, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
