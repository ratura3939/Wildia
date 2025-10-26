#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include"SceneManager.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		//画像
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
		

		//モデル
		STAGE_MDL,
		OBJECTS_MDL,
		PLAYER_MDL,
		ENEMY_MDL,
		BOSS_MDL,
		SKY_DOME,

		//音
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
		

		//エフェクト
		SWORD_EFC,
		DAMAGE_EFC,

		TEST_MDL,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(SceneManager::SCENE_ID _scene= SceneManager::SCENE_ID::NONE);

	
private:
	//シーンごとにデータを読み込むことにする
	void InitTitle(void);
	void InitGame(void);
	void InitClear(void);
	void InitGameOver(void);
	void InitPouse(void);
	void InitKeyConfig(void);
	void InitSwitchCntl(void);

	//大まかなリソースのまとまり
	void ResorceDeviceIcon(void);
	void ResorceStage(void);

public:
	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

