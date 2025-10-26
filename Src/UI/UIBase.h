#pragma once
#include<DxLib.h>
#include<string>

class UIBase
{
public:
	UIBase(VECTOR& _followPos);
	virtual ~UIBase(void);

	virtual bool Init(const std::string& _master) = 0;
	virtual bool Update(void) = 0;
	virtual void Draw(void) = 0;

	virtual void SetPos(const VECTOR& _pos);

	virtual void Reset(void) = 0;	//変数リセット用

protected:
	VECTOR drawPos_;	//描画位置
	VECTOR& drawFollowPos_;	//追従型のUIに使用する
};

