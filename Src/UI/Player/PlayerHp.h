#pragma once
#include<DxLib.h>
#include<vector>
#include "../UIBase.h"

class PlayerHp :
	public UIBase
{
public:
	static constexpr float SCALE_DOWN = 1.0f / 8.0f;

	static constexpr float SIZE_X = 500.0f * SCALE_DOWN;
	static constexpr float SIZE_Y = 428.0f * SCALE_DOWN;

	static constexpr float ALPHA_DEG = 2.0f;
	static constexpr float FALL_ACC = 0.5f;

	static constexpr float HP_EX = 0.2f;

	enum class STATE {
		NOMAL,
		BROKEN,
		EMPTY,
		MAX
	};

	PlayerHp(VECTOR& _followPos,const int _hp);
	~PlayerHp(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	//èÛë‘ïœçX
	//void ChangeState(const STATE _state) { state_ = _state; }

	void SetPos(const VECTOR& _pos)override;

	void Reset(void)override;

	void Damage(void);

private:
	//UIâÊëú
	std::string nomalStr_;
	std::string emptyStr_;
	std::string brokenStr_;

	std::vector<STATE> states_;		//èÛë‘Çï\Ç∑

};

