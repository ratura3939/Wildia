#include "UIBase.h"

UIBase::UIBase(VECTOR& _followPos):drawFollowPos_(_followPos)
{
}

UIBase::~UIBase(void)
{
}

void UIBase::SetPos(const VECTOR& _pos)
{
	drawPos_ = _pos;
}
