#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyFind.h"

namespace {
	const float FIND_EX = 80.0f;
}

EnemyFind::EnemyFind(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state):UIBase(_followPos),eState_(_state)
{
	findUICnt_ = 0.0f;
}

EnemyFind::~EnemyFind(void)
{
}

bool EnemyFind::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	suspectStr_ = _master + "Suspect";
	findStr_ = _master + "Find";

	uiM.Add(suspectStr_, rsM.Load(ResourceManager::SRC::SUSPECT_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::ZOOM_IN, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(suspectStr_, drawFollowPos_);
	uiM.SetUIDirectionPram(suspectStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, SUSPECT_EXT_ACC, SUSPECT_EXT_MAX, 1.0f);

	uiM.Add(findStr_, rsM.Load(ResourceManager::SRC::FIND_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(findStr_, drawFollowPos_, FIND_EX);

	return true;
}

bool EnemyFind::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	if (eState_==EnemyBase::ENEMY_STATE::SEARCH) {
		//Œ©‚Â‚©‚Á‚Ä‚¢‚È‚¢‚Æ‚«
		uiM.SetPos(suspectStr_, drawFollowPos_);
		uiM.Update(suspectStr_);
	}
	if(eState_ == EnemyBase::ENEMY_STATE::BATTLE&& findUICnt_<= FIND_UI_DRAW_TIME) {
		uiM.SetPos(findStr_, drawFollowPos_);
		uiM.Update(findStr_);
		findUICnt_++;
	}
	
	return true;
}

void EnemyFind::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	if (eState_ == EnemyBase::ENEMY_STATE::SEARCH) {
		//Œ©‚Â‚©‚Á‚Ä‚¢‚È‚¢‚Æ‚«
		uiM.Draw(suspectStr_);
	}
	if (eState_ == EnemyBase::ENEMY_STATE::BATTLE && findUICnt_ <= FIND_UI_DRAW_TIME) {
		uiM.Draw(findStr_);
	}
}

void EnemyFind::Reset(void)
{
	findUICnt_ = 0.0f;
	UIManager2d::GetInstance().ResetUpdate(suspectStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM);
}
