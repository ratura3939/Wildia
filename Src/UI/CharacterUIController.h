#pragma once
#include<string>
#include <DxLib.h>
class CharacterUIController
{
public:
	CharacterUIController(VECTOR& _followPos);
	virtual ~CharacterUIController(void);

	virtual void Init(const std::string& _master) = 0;
	virtual void Update(void) = 0;
	virtual void Release(void) = 0;

	virtual void SetDrawPos(const VECTOR _pos) = 0;

protected:
	VECTOR& followUIPos_;
};

