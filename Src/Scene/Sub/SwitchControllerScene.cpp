#include<string>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"

#include "SwitchControllerScene.h"

//ローカル定数
namespace {
#pragma region UI登録名
	const std::string SWITCH_IMG = "switchImg";
	const std::string SWITCH_BTN = "switchBtn";
	const std::string BACK_BTN = "BackBtn_swScene";
	const std::string FINISH_SW = "Finish_swScene";
#pragma endregion

#pragma region 画像調整用
	//「切り換え完了」
	const float STRING_EX = 1.4f;	//サイズ
	const int IMG_SW_DIFF_Y = 100;	//Y位置調整用
#pragma endregion

	const int COOL_TIME_MAX = 100;	//クールタイム上限
}

SwitchControllerScene::SwitchControllerScene(void)
{
	coolTimeCnt_ = 0;
	isSwCoolTime_ = false;
	isSwBtn_ = false;
}

SwitchControllerScene::~SwitchControllerScene(void)
{
}

void SwitchControllerScene::Init(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR screenSize = { static_cast<float>(Application::SCREEN_SIZE_X),static_cast<float>(Application::SCREEN_SIZE_Y),0.0f };

	//切り替え確認画像
	uiM.Add(SWITCH_IMG, rsM.Load(ResourceManager::SRC::CONFIRM_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_IMG, VECTOR{ screenSize.x / 2.0f,screenSize.y / 2.0f - IMG_SW_DIFF_Y,0.0f }, IMG_EX_SB_SCENE);

	//切り替えるボタン
	uiM.Add(SWITCH_BTN, rsM.Load(ResourceManager::SRC::SWITCH_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_BTN, VECTOR{ screenSize.x / 2.0f,screenSize.y - BTN_DIFF_Y_SB_SCENE,0.0f }, BTN_EX_MAX_SB_SCENE);
	uiM.SetUIDirectionPram(SWITCH_BTN, UI_GROUP::ZOOM, BTN_EX_ACC_SB_SCENE, BTN_EX_MAX_SB_SCENE, BTN_EX_MIN_SB_SCENE);

	//戻るボタン
	uiM.Add(BACK_BTN, rsM.Load(ResourceManager::SRC::BACK_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_BTN, VECTOR{ BTN_DIFF_X_SB_SCENE,screenSize.y - BTN_DIFF_Y_SB_SCENE,0.0f }, BTN_EX_MAX_SB_SCENE);
	uiM.SetUIDirectionPram(BACK_BTN, UI_GROUP::ZOOM, BTN_EX_ACC_SB_SCENE, BTN_EX_MAX_SB_SCENE, BTN_EX_MIN_SB_SCENE);

	//切り替え完了通知
	uiM.Add(FINISH_SW, rsM.Load(ResourceManager::SRC::FINISH_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(FINISH_SW, VECTOR{ screenSize.x - BTN_DIFF_X_SB_SCENE,Application::SCREEN_SIZE_Y - BTN_DIFF_Y_SB_SCENE,0.0f }, STRING_EX);
}

void SwitchControllerScene::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//切り替え後
	if (isSwCoolTime_) {
		//カウンタ加算
		coolTimeCnt_++;

		if (coolTimeCnt_ > COOL_TIME_MAX) {
			//切り換えクールタイム解除
			isSwCoolTime_ = false;
			coolTimeCnt_ = 0;
		}
	}

	//決定の入力
	if (inpM.IsTrigerrDown("action",false)) {
		if (isSwBtn_) {
			//切り換え処理・クールタイム発生
			SceneManager::GetInstance().SwitchController();
			isSwCoolTime_ = true;
		}
		else {
			//シーンをポップ
			SceneManager::GetInstance().PopScene();
			return;
		}
	}

	//移動入力
	if (inpM.IsTrigerrDown("right",false)) {
		//「切り換え」ボタンの位置になるためtrueに
		isSwBtn_ = true;
	}
	else if (inpM.IsTrigerrDown("left",false)) {
		//「戻る」ボタンの位置になるためfalseに
		isSwBtn_ = false;
	}

	//ボタン拡大・縮小(現在選択しているボタンのみ)
	if (isSwBtn_) {
		uiM.Update(SWITCH_BTN);
	}
	else {
		uiM.Update(BACK_BTN);
	}
}

void SwitchControllerScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//UI描画
	uiM.Draw({ SWITCH_IMG,SWITCH_BTN,BACK_BTN });

	//クールタイム時
	if (isSwCoolTime_) {
		//「完了しました」という文章表示
		uiM.Draw(FINISH_SW);
	}
}

void SwitchControllerScene::Release(void)
{
	//UIの削除
	UIManager2d::GetInstance().DeleteUI({ SWITCH_IMG,SWITCH_BTN,BACK_BTN });
}

void SwitchControllerScene::Reset(void)
{
}

void SwitchControllerScene::InitSound(void)
{
}

void SwitchControllerScene::InitEffect(void)
{
}
