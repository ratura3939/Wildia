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

	virtual void Reset(void) = 0;	//�ϐ����Z�b�g�p

protected:
	VECTOR drawPos_;	//�`��ʒu
	VECTOR& drawFollowPos_;	//�Ǐ]�^��UI�Ɏg�p����
};

