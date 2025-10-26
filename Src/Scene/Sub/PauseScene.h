#pragma once
#include "../SceneBase.h"

#include<vector>
#include<string>

class PauseScene :
    public SceneBase
{
public:
	/// <summary>
	/// メニュー時の項目
	/// </summary>
	enum class MENU_ITEM {
		NONE,
		BACK_GAME,	//ゲームに戻る
		CONFIG,		//操作方法
		SWITCH_CNTL,//操作切り換え
		BACK_TITLE,	//ゲームやめる(タイトルに戻る)
	};

	PauseScene(void);
	~PauseScene(void)override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	//入力受付
	void InputUser(void);

	//矢印を描画すべき位置を取得
	const VECTOR GetDrawPosOfArrow(void)const;

	std::vector<std::string>drawBtnList_;	//描画されるボタンを上から順に
	int selectIdx_;	//カーソル位置
};

