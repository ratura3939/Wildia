#include "PlayerUIController.h"
#include"Player/PlayerHp.h"
#include"Player/NoticeDodge.h"

PlayerUIController::PlayerUIController(VECTOR& _followPos, const int _hp):CharacterUIController(_followPos)
{
	hpDrawPos_ = { 100.0f,80.0f,0.0f };
	hp_ = std::make_unique<PlayerHp>(hpDrawPos_, _hp);
	isAllert_ = false;
	dodge_ = std::make_unique<NoticeDodge>(_followPos);
}

PlayerUIController::~PlayerUIController(void)
{
}

void PlayerUIController::Init(const std::string& _master)
{
	hp_->Init(_master);
	dodge_->Init(_master);
}

void PlayerUIController::Update(void)
{
	hp_->Update();
	dodge_->Update();
}
void PlayerUIController::Draw(void)
{
	hp_->Draw();
	if (isAllert_) {
		dodge_->Draw();
	}
}


void PlayerUIController::Release(void)
{
}

void PlayerUIController::SetDrawPos(const VECTOR _pos)
{
}

void PlayerUIController::Damage(void)
{
	hp_->Damage();
}

