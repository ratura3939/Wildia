#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"

#include "KeyConfigScene.h"

//ローカル定数
namespace {
#pragma region UI登録名
	const std::string BACK_BTN = "BackBtn_KeyScene";
	const std::string CONFIG_IMG = "Config_img";
#pragma endregion
}

KeyConfigScene::KeyConfigScene(void)
{
}

KeyConfigScene::~KeyConfigScene(void)
{
}

void KeyConfigScene::Init(void)
{
	//頻出するシーンでもないので演出用マネージャは使用しない

	auto cntl = SceneManager::GetInstance().GetController();
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR screenSize = { static_cast<float>(Application::SCREEN_SIZE_X),static_cast<float>(Application::SCREEN_SIZE_Y),0.0f };

	//操作説明の画像
	if (cntl == SceneManager::CNTL::PAD) {
		//PADの場合
		uiM.Add(CONFIG_IMG, rsM.Load(ResourceManager::SRC::DESCRIPTION_PAD).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	}
	else {
		//キーボードの場合
		uiM.Add(CONFIG_IMG, rsM.Load(ResourceManager::SRC::DESCRIPTION_KEY).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	}
	uiM.SetUIInfo(CONFIG_IMG, VECTOR{ screenSize.x / 2.0f,screenSize.y / 2.0f,0.0f }, IMG_EX_SB_SCENE);

	//戻るボタン
	uiM.Add(BACK_BTN, rsM.Load(ResourceManager::SRC::BACK_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_BTN, VECTOR{ static_cast<float>(BTN_DIFF_X_SB_SCENE),static_cast<float>(screenSize.y - BTN_DIFF_Y_SB_SCENE),0.0f }, BTN_EX_MAX_SB_SCENE);
	uiM.SetUIDirectionPram(BACK_BTN, UI_GROUP::ZOOM, BTN_EX_ACC_SB_SCENE, BTN_EX_MAX_SB_SCENE, BTN_EX_MIN_SB_SCENE);
}

void KeyConfigScene::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//決定ボタンが押されたとき
	if (inpM.IsTrigerrDown("action",false)) {
		//シーンをポップ
		SceneManager::GetInstance().PopScene();
	}

	//UI更新
	uiM.Update(BACK_BTN);
}

void KeyConfigScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//その他ボタン描画
	uiM.Draw({ CONFIG_IMG,BACK_BTN });
}

void KeyConfigScene::Release(void)
{
	UIManager2d::GetInstance().DeleteUI({ CONFIG_IMG,BACK_BTN });
}

void KeyConfigScene::Reset(void)
{
}

void KeyConfigScene::InitSound(void)
{
	//随時追加
}

void KeyConfigScene::InitEffect(void)
{
	//随時追加
}
