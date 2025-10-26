#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyCount.h"

//ローカル定位数
namespace {
	const float SCALE_SKEL = 0.2f;
	const float SCALE_PALTE_NUM = 0.4f;
	const float POS_AJUST = 100.0f;
}

EnemyCount::EnemyCount(VECTOR& _followPos):UIBase(_followPos)
{
	update_ = &EnemyCount::NomalUpdate;
	golemIcon_ = -1;
}

EnemyCount::~EnemyCount(void)
{
}

bool EnemyCount::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	plateStr_ = _master + "CounterPlate";
	numberStr_ = _master + "UseNumber";
	SkeltonConterStr_ = _master + "SkeltonIcon";

	//プレート
	uiM.Add(plateStr_, rsM.Load(ResourceManager::SRC::PLATE_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(plateStr_, drawFollowPos_, SCALE_PALTE_NUM);
	//数字
	auto numPos = drawFollowPos_;
	numPos.x += POS_AJUST;
	uiM.Add(numberStr_, -1, UIManager2d::UI_DIRECTION_2D::UP_DOWN, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(numberStr_, numPos, SCALE_PALTE_NUM);
	uiM.SetUIDirectionPram(numberStr_, UIManager2d::UI_DIRECTION_GROUP::MOVE, 2.0f, 20.0f, -20.0f);			//詳細設定

	//アイコン×
	auto iconPos = drawFollowPos_;
	iconPos.x -= POS_AJUST/2;
	uiM.Add(SkeltonConterStr_, rsM.Load(ResourceManager::SRC::SKELTON_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SkeltonConterStr_, iconPos, SCALE_SKEL);

	golemIcon_ = rsM.Load(ResourceManager::SRC::GOLEM_IMG).handleId_;

	return true;
}

bool EnemyCount::Update(void)
{
	(this->*update_)();
	return false;
}

void EnemyCount::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//uiM.Draw({ plateStr_, SkeltonConterStr_,numberStr_ });
	uiM.Draw(plateStr_);
	uiM.Draw(SkeltonConterStr_);
	uiM.Draw(numberStr_);
}

void EnemyCount::Reset(void)
{
}

void EnemyCount::SetNumImg(int _img)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetImage(numberStr_, _img);
}

void EnemyCount::SetIconImg(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetImage(SkeltonConterStr_, golemIcon_);
}

void EnemyCount::NomalUpdate(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//表示項目を常にぴょこぴょこさせる
	uiM.Update(numberStr_);
}
