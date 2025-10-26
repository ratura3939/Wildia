#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "NoticeDodge.h"

NoticeDodge::NoticeDodge(VECTOR& _followPos) :UIBase(_followPos)
{
}

NoticeDodge::~NoticeDodge(void)
{
}

bool NoticeDodge::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	noticeStr_ = _master + "AtkNotice";

	uiM.Add(noticeStr_, rsM.Load(ResourceManager::SRC::ATK_ALLERT_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(noticeStr_, drawFollowPos_);
	//uiM.SetUIDirectionPram(noticeStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, SUSPECT_EXT_ACC, SUSPECT_EXT_MAX, 1.0f);
	return false;
}

bool NoticeDodge::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetPos(noticeStr_, drawFollowPos_);
	return false;
}

void NoticeDodge::Draw(void)
{
	UIManager2d::GetInstance().Draw(noticeStr_);
}

void NoticeDodge::Reset(void)
{
}
