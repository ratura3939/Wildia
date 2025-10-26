#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyTargetting.h"

//ローカル定数
namespace {
	const float ACC_ALPHA = 10.0f;
	const float MAX_ALPHA = 255.0f;
	const float MIN_ALPHA = 00.0f;

	const float ACC_MOVE = 3.0f;
	const float MAX_MOVE = 40.0f;
	const float MIN_MOVE = 0.0f;
}

EnemyTargetting::EnemyTargetting(VECTOR& _followPos):UIBase(_followPos)
{
	isLocked_ = false;
}

EnemyTargetting::~EnemyTargetting(void)
{
}

bool EnemyTargetting::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	noticeStr_ = _master + "LockNotice";
	lockStr_ = _master + "Locked";

	uiM.Add(noticeStr_, rsM.Load(ResourceManager::SRC::ANNOUNCE_LOCKON_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::FLASHING, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(noticeStr_, drawFollowPos_, 100.0f);
	uiM.SetUIDirectionPram(noticeStr_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, ACC_ALPHA, MAX_ALPHA, MIN_ALPHA);


	uiM.Add(lockStr_, rsM.Load(ResourceManager::SRC::LOCKON_IMG ).handleId_, UIManager2d::UI_DIRECTION_2D::UP_DOWN, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(lockStr_, drawFollowPos_,100.0f);
	uiM.SetUIDirectionPram(lockStr_, UIManager2d::UI_DIRECTION_GROUP::MOVE, ACC_MOVE, MAX_MOVE, MIN_MOVE);

	return true;
}

bool EnemyTargetting::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//ロックオンされていたら
	if (isLocked_) {
		uiM.SetPos(lockStr_, drawFollowPos_);
		uiM.Update(lockStr_);
	}
	else {
		uiM.SetPos(noticeStr_, drawFollowPos_);
		uiM.Update(noticeStr_);
	}
	return true;
}

void EnemyTargetting::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//ロックオンされていたら
	if (isLocked_) {
		uiM.Draw(lockStr_);
	}
	else {
		uiM.Draw(noticeStr_);
	}
}

void EnemyTargetting::Reset(void)
{
}
