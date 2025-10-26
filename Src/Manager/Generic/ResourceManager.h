#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include"SceneManager.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		//�摜
		TITLE_LOGO,
		SHADOW_LOGO,
		PAD_IMG,
		KEYBOARD_IMG,
		GAMEOVER_LOGO,
		GAMECLAR_LOGO,
		TITLE_BACK_BTN,
		BACK_GAME_BTN,
		MENU_BTN,
		START_GAME_BTN,
		STOP_GAME_BTN,
		CLICK_STR_IMG,
		BACK_BTN,
		SWITCH_BTN,

		DESCRIPTION_PAD,
		DESCRIPTION_KEY,

		CONFIRM_SWITCH,
		FINISH_SWITCH,

		FOCUS_IMG,
		//UI
		ARROW_DOWN_IMG,
		HEART_IMG,
		HEART_BROKEN_IMG,
		HEART_EMPTY_IMG,
		SUSPECT_IMG,
		FIND_IMG,
		EXIT_IMG,
		LOCKON_IMG,
		ANNOUNCE_LOCKON_IMG,
		PLATE_IMG,
		SKELTON_IMG,
		GOLEM_IMG,
		NUMBER_IMGS,
		WARNING_IMG,
		ATK_ALLERT_IMG,
		RETRY_IMG,
		CHECK_CONFIG_IMG,
		SWITCH_OPERATOR_IMG,
		

		//���f��
		STAGE_MDL,
		OBJECTS_MDL,
		PLAYER_MDL,
		ENEMY_MDL,
		BOSS_MDL,
		SKY_DOME,

		//��
		TITLE_BGM,
		GAME_BGM,
		BATTLE_BGM,
		BOSS_BGM,
		WARNING_BGM,

		GAMECLEAR_BGM,
		GAMEOVER_BGM,

		CANCEL_SE,
		MOVE_CURSUR_SE,
		ENTER_CNTL_SE,
		WALK_SE,
		RUN_SE,
		SWING_SWORD_SE,
		LOCK_ON_SE,
		FIND_PLAYER_SE,
		DAMAGE_SE,
		BOSS_IMPACT_SE,
		BOSS_SHOUT_SE,

		ATK_ALLERT_SE,
		DODGE_SE,
		JUST_DODGE_SE,
		

		//�G�t�F�N�g
		SWORD_EFC,
		DAMAGE_EFC,

		TEST_MDL,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(SceneManager::SCENE_ID _scene= SceneManager::SCENE_ID::NONE);

	
private:
	//�V�[�����ƂɃf�[�^��ǂݍ��ނ��Ƃɂ���
	void InitTitle(void);
	void InitGame(void);
	void InitClear(void);
	void InitGameOver(void);
	void InitPouse(void);
	void InitKeyConfig(void);
	void InitSwitchCntl(void);

	//��܂��ȃ��\�[�X�̂܂Ƃ܂�
	void ResorceDeviceIcon(void);
	void ResorceStage(void);

public:
	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

