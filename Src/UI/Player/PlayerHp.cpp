#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "PlayerHp.h"

namespace {
	constexpr float HP_DISTANCE = 100.0f;
}

PlayerHp::PlayerHp(VECTOR& _followPos, const int _hp):UIBase(_followPos)
{
	for (int num = 0; num < _hp; num++) {
		states_.push_back(STATE::NOMAL);
	}
}

PlayerHp::~PlayerHp(void)
{
}

bool PlayerHp::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR pos = drawFollowPos_;

	nomalStr_ = _master + "NomalHp";
	emptyStr_ = _master + "EmptyHp";
	brokenStr_ = _master + "BrokenHp";

	for (int i = 0; i < states_.size(); i++) {
		//ノーマルHP
		std::string nomal = nomalStr_.c_str() + i;
		std::string empty = emptyStr_.c_str() + i;
		std::string broken = brokenStr_.c_str() + i;

		uiM.Add(nomal, rsM.Load(ResourceManager::SRC::HEART_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
		uiM.SetUIInfo(nomal, pos, HP_EX);
		//空のHP
		uiM.Add(empty, rsM.Load(ResourceManager::SRC::HEART_EMPTY_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
		uiM.SetUIInfo(empty, pos, HP_EX);
		//ひび割れHP(落ちてうっすら消えていく)
		uiM.Add(broken, rsM.Load(ResourceManager::SRC::HEART_BROKEN_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::GRAD_DISAP, UI_DIMENSION::DIMENSION_2);
		uiM.SetUIInfo(broken, pos, HP_EX);
		uiM.SetUIDirectionPram(broken, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, 3.0f, 255.0f, 0.0f);

		uiM.PushUIDirection(broken, UIManager2d::UI_DIRECTION_2D::MOVE_DOWN);
		uiM.SetUIDirectionPram(broken, UIManager2d::UI_DIRECTION_GROUP::MOVE, 1.0f, 50.0f, 0.0f);

		pos.x += HP_DISTANCE;
	}

	//正常終了
	return true;
}

bool PlayerHp::Update(void)
{
	//ダメージ位状態のHPがあるなら更新をかける
	int cnt = 0;

	for (auto& state : states_) {
		if (state == STATE::BROKEN) {
			UIManager2d::GetInstance().Update(brokenStr_.c_str() + cnt);
			if (UIManager2d::GetInstance().IsFinishDirection(brokenStr_.c_str() + cnt, UIManager2d::UI_DIRECTION_GROUP::MOVE)) {
				state = STATE::EMPTY;
			}
			break;
		}
		cnt++;
	}

	////正常終了
	return true;
}

void PlayerHp::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	VECTOR pos = drawPos_;
	int cnt = 0;

	for (auto& state : states_) {
		
		if (state == STATE::NOMAL) {
			uiM.Draw(nomalStr_.c_str() + cnt);
		}
		else {
			uiM.Draw(emptyStr_.c_str() + cnt);
			if (state == STATE::BROKEN) {
				uiM.Draw(brokenStr_.c_str() + cnt);
			}
		}
		cnt++;
	}
}

void PlayerHp::SetPos(const VECTOR& _pos)
{
	drawPos_ = _pos;
}

void PlayerHp::Reset(void)
{
}

void PlayerHp::Damage(void)
{
	for (int cnt = static_cast<int>(states_.size()) - 1; cnt >= 0; cnt--) {
		if (states_[cnt] == STATE::NOMAL) {
			states_[cnt] = STATE::BROKEN;
			break;
		}
	}
}
