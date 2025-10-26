#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Scene/Main/Title.h"
#include "../../Scene/Sub/KeyConfigScene.h"
#include "../../Scene/Sub/SwitchControllerScene.h"
#include"../../Utility/Utility.h"
#include "PauseScene.h"

//ローカル定数
namespace {
#pragma region UI登録名
	std::string BACK_TITLE_BTN = "BackTitle_PauseScene";
	std::string BACK_GAME_BTN = "BackGame_PauseScene";
	std::string CHECK_CONFIG_BTN = "Config_PauseScene";
	std::string SWITCH_OPE_BTN = "SwitchCntl_PauseScene";
	std::string RIGHT_ARROW = "Arrow_PauseScene";
#pragma endregion

#pragma region 画像調整用
	const float BTN_SIZE = 256.0f;			//元画像大きさ
	const float BTN_DRAW_SIZE = 0.6f;		//ボタン(ゲームに戻る・タイトルに戻る)を描画するときの大きさ倍率
	const float BIG_BTN_DRAW_SIZE = 0.5f;	//ボタン(操作方法・操作切り替え)を描画するときの大きさ倍率

	const float ARRW_DIFF_X = -300.0f;		//矢印の中央描画位置からの差分（X軸）
	const float ARROW_LOCAL_ROT = -270.0f;	//矢印の画像回転用
	const float ARROW_ACC = -3.0f;			//矢印の移動量
	const float ARROW_MOVE_MAX = 0;			//移動量(上限)
	const float ARROW_MOVE_MIN = -60;		//移動量(下限)
#pragma endregion

	const int MENU_LIST_NONE_DIFFER = 1;	//列挙にNONEが入っているのでそれの差分用
}

PauseScene::PauseScene(void)
{
	drawBtnList_ = {};
	selectIdx_ = 0;
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);

	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR screenSize = { static_cast<float>(Application::SCREEN_SIZE_X),static_cast<float>(Application::SCREEN_SIZE_Y),0.0f };

	//ゲームに戻る
	uiM.Add(BACK_GAME_BTN, resM.Load(ResourceManager::SRC::BACK_GAME_BTN).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_GAME_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) - BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//操作方法
	uiM.Add(CHECK_CONFIG_BTN, resM.Load(ResourceManager::SRC::CHECK_CONFIG_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(CHECK_CONFIG_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) - BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//操作切り替え
	uiM.Add(SWITCH_OPE_BTN, resM.Load(ResourceManager::SRC::SWITCH_OPERATOR_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_OPE_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) + BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//タイトルに戻る
	uiM.Add(BACK_TITLE_BTN, resM.Load(ResourceManager::SRC::STOP_GAME_BTN).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_TITLE_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) + BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//リスト制作
	drawBtnList_ = { BACK_GAME_BTN, CHECK_CONFIG_BTN, SWITCH_OPE_BTN, BACK_TITLE_BTN };

	//矢印
	uiM.Add(RIGHT_ARROW, resM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::LEFT_RIGHT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(RIGHT_ARROW, GetDrawPosOfArrow(), BTN_DRAW_SIZE, ARROW_LOCAL_ROT);
	uiM.SetUIDirectionPram(RIGHT_ARROW, UI_GROUP::MOVE, ARROW_ACC, ARROW_MOVE_MAX, ARROW_MOVE_MIN);
	
	//演出初期化
	InitSound();
	InitEffect();
}

void PauseScene::InitSound(void)
{
	//随時追加
}

void PauseScene::InitEffect(void)
{
	//随時追加
}

void PauseScene::Update(void)
{
	//入力受付
	InputUser();

	//UI更新
	UIManager2d::GetInstance().Update(RIGHT_ARROW);
}

void PauseScene::InputUser(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//ポーズボタンを押されたら
	if (inpM.IsTrigerrDown("pause")) {
		//シーン移動(1つ前のシーン＝ゲームシーンに戻る)
		scM.PopScene();
	}

	//決定入力
	if (inpM.IsTrigerrDown("action",false)) {

		//矢印位置から処理を選択
		switch (static_cast<MENU_ITEM>(selectIdx_ + MENU_LIST_NONE_DIFFER)) {
		case MENU_ITEM::BACK_GAME:
			//シーンをポップ
			scM.PopScene();
			break;

		case MENU_ITEM::CONFIG:
			//操作方法シーンの追加
			scM.PushScene(std::make_shared<KeyConfigScene>());
			break;

		case MENU_ITEM::SWITCH_CNTL:
			//操作切り換えシーンの追加
			scM.PushScene(std::make_shared<SwitchControllerScene>());
			break;

		case MENU_ITEM::BACK_TITLE:
			//タイトルシーンに変更
			scM.ChangeScene(std::make_shared<Title>());
			break;
		}
	}

	//上入力
	if (inpM.IsTrigerrDown("up",false)) {
		//Idxの減少。範囲外にならないよう調整
		selectIdx_--;
		selectIdx_ = (selectIdx_ + drawBtnList_.size()) % static_cast<int>(drawBtnList_.size());

		//矢印の描画位置設定
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetDrawPosOfArrow());
	}
	//下入力
	else if (inpM.IsTrigerrDown("down",false)) {
		//Idxの増加。範囲外にならないよう調整
		selectIdx_++;
		selectIdx_ = selectIdx_ % static_cast<int>(drawBtnList_.size());

		//矢印の描画位置設定
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetDrawPosOfArrow());
	}
}

void PauseScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ボタンの描画
	uiM.Draw(drawBtnList_);

	//→矢印の描画
	uiM.Draw(RIGHT_ARROW);
}

void PauseScene::Release(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void PauseScene::Reset(void)
{
	//ポーズシーンではBGMを流さない
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 0);	//前シーンで流れているBGMの音量を０に
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);
}

const VECTOR PauseScene::GetDrawPosOfArrow(void) const
{
	//選択されているボタンの位置
	VECTOR arrowPos = UIManager2d::GetInstance().GetDrawPos(drawBtnList_[selectIdx_]);
	//描画位置はボタンの左側(差分を足す)
	arrowPos.x += ARRW_DIFF_X;
	return arrowPos;
}
