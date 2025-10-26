#pragma once
#include"../UIBase.h"

class EnemyHp :
	public UIBase
{
public:
	EnemyHp(VECTOR& _followPos,float& _now, float _max);
	~EnemyHp(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	float& nowHp_;
	float hpMax_;
};

