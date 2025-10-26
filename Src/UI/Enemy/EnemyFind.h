#pragma once
#include<string>
#include "../UIBase.h"
#include"../../Object/Character/Enemy/EnemyBase.h"

class EnemyFind :
    public UIBase
{
public:
	//「？」拡大用
	static constexpr float SUSPECT_EXT_MAX = 100.0f;//拡大率最大値
	static constexpr float SUSPECT_EXT_ACC = 2.0f;	//拡大スピード
	//「！」描画用
	static constexpr float FIND_UI_DRAW_TIME = 30.0f;	//描画時間
	static constexpr float FIND_UI_DRAW_SIZE = 50.0f;	//描画サイズ

	EnemyFind(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state);
	~EnemyFind(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string suspectStr_;
	std::string findStr_;

	float findUICnt_;   //発見を表示し続けるカウンター

	EnemyBase::ENEMY_STATE& eState_;
};

