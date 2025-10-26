#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Enemy/Boss.h"
#include"../../Object/Character/Enemy/Skelton.h"
#include"../../Utility/Utility.h"
#include"../../Application.h"
#include"../../UI/Enemy/EnemyCount.h"
#include"../Generic/ResourceManager.h"
#include"../../Scene/Main/Game.h"
#include "EnemyManager.h"

const std::string EnemyManager::ATTACK_NOMAL = "EnemyAttack";

EnemyManager::EnemyManager(Game& _scene):gameScene_(_scene)
{
	enemyCnt_ = -1;
	numImg_ = nullptr;
	platePos_ = Utility::VECTOR_INIT;
	preBattle_ = false;
	createBoss_ = false;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	//デバッグ用
	VECTOR initPos[4] = { INIT_1 ,INIT_2 ,INIT_3 ,INIT_4 };

	enemyCnt_ = ENEMY_NUM;
	numImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBER_IMGS).handleIds_;

	for (int i = 0; i < ENEMY_NUM; i++) {
		std::shared_ptr enemy = std::make_shared<Skelton>(initPos[i]);
		enemy->Init(i);
		characters_.push_back(std::move(enemy));
	}

	platePos_ = VECTOR{ Application::SCREEN_SIZE_X - 300.0f,150.0f,0.0f };
	counterUI_ = std::make_unique<EnemyCount>(platePos_);
	counterUI_->Init("Manager");
	//残りカウントのの設定
	counterUI_->SetNumImg(numImg_[enemyCnt_]);

	preBattle_ = false;
}

void EnemyManager::Update(const VECTOR& _playerPos, AttackManager& _atkMng)
{
	preBattle_ = IsBattleEnemy();

	//いなかったら処理しない
	if (characters_.empty())return;

	counterUI_->Update();

	//死亡したキャラクターの配列番号保存とそのカウンター
	std::vector<int>dethEnemy = {};
	int counter = 0;
	//敵の個体分回す
	for (auto& chara : characters_) {
		//更新をかける
		chara->Update(_playerPos, _atkMng);

		//死亡していたら
		if (!chara->IsAlive()) {
			//死亡リストに追加
			dethEnemy.push_back(counter);
		}
		//個体カウンター増加
		counter++;
	}

	//死亡リスト分回す
	int dethCnt = 0;
	for (auto& idx : dethEnemy) {
		//該当の敵を消去
		characters_.erase(characters_.begin()+(idx-dethCnt));
		enemyCnt_--;

		dethCnt++;
	}
	if (dethCnt != 0) {
		//残りカウントのの設定
		counterUI_->SetNumImg(numImg_[enemyCnt_]);
		if (enemyCnt_ <= 0 && !createBoss_) {
			//シーンにボス出現を伝える
			gameScene_.StartBossFaze();
		}
	}
}

void EnemyManager::UpdateAnim(void)
{
	for (auto& chara : characters_) {
		//更新をかける
		chara->UpdateAnimOnly();
	}
}

void EnemyManager::Draw(void)
{
	//いなかったら処理しない
	if (characters_.empty())return;

	for (auto& chara : characters_) {
		chara->Draw();
	}
	counterUI_->Draw();
}

void EnemyManager::Release(void)
{
	//いなかったら処理しない
	if (characters_.empty())return;
	for (auto& chara : characters_) {
		chara->Release();
	}
}

std::vector<std::weak_ptr<EnemyBase>> EnemyManager::GetEnemys(void)
{
	std::vector<std::weak_ptr<EnemyBase>>retVector;

	for (auto& chara : characters_) {
		retVector.push_back(chara);
	}
	return retVector;
}

const VECTOR EnemyManager::GetPos(const int _num)
{
	return characters_[_num]->GetPos();
}

const Quaternion EnemyManager::GetQua(const int _num)
{
	return characters_[_num]->GetQua();
}


int EnemyManager::GetNearEnemyNum(const VECTOR _pPos)
{
	int nearNum = -1;
	VECTOR distance = Utility::VECTOR_ZERO;
	//テキトーなでかい値
	double min = 100000.0;

	for (int i = 0; i < characters_.size(); i++) {
		//画面内にいないときはつぎへ
		VECTOR myPos = characters_[i]->GetPos();
		distance = VSub(characters_[i]->GetPos(), _pPos);

		float disMag = Utility::MagnitudeF(distance);

		if (/*!InsideScreen(myPos)||*/ disMag > TARGETTING_PERMISSION_DISTANCE)continue;

		//距離比較
		
		if (min > disMag) {
			nearNum = i;
			min = disMag;
		}
	}

	//デバッグ用
	if (nearNum != -1)characters_[nearNum]->SetColor(0xffffff);

	return nearNum;
}

bool EnemyManager::InsideScreen(const VECTOR _pos)
{
	//ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(_pos);
	//画面内にいるか
	if (screenPos.x > 0.0f && screenPos.x < Application::SCREEN_SIZE_X &&
		screenPos.y>0.0f && screenPos.y < Application::SCREEN_SIZE_Y) 
	{
		return true;
	}
	return false;
}

bool EnemyManager::IsBattleEnemy(void)
{
	for (auto& chara : characters_) {
		//戦闘状態の敵が居たら
		if (chara->GetNowState() == EnemyBase::ENEMY_STATE::BATTLE) {
			return true;
		}
	}
	return false;
}

bool EnemyManager::IsSwitchBattleOrNomalEnemyTrg(void)
{
	//更新前の状態と更新後の物を比較
	if (preBattle_ != IsBattleEnemy()) {
		return true;
	}
	return false;
}

void EnemyManager::SetAnimSpeedRate(const float _percent)
{
	for (auto chara : characters_) {
		chara->SetAnimSpeedRate(_percent);
	}
}

void EnemyManager::SetTargetEnemy(const int _num)
{
	//引数以外の物を解除しつつ設定
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//設定
		characters_[i]->SetIsLockTraget(setFlag);
	}
}

void EnemyManager::NoTargetEnemy(void)
{
	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		//設定
		characters_[i]->SetIsLockTraget(false);
		characters_[i]->SetIsLocked(false);
	}
}

void EnemyManager::LokedOn(const int _num)
{
	//引数以外の物を解除しつつ設定
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//設定
		characters_[i]->SetIsLocked(setFlag);
	}
}

void EnemyManager::CreateBoss(void)
{
	VECTOR pos = INIT_1;
	auto boss = std::make_shared<Boss>(pos);
	boss->Init(0);
	characters_.push_back(boss);
	createBoss_ = true;

	enemyCnt_++;

	counterUI_->SetNumImg(numImg_[enemyCnt_]);
	counterUI_->SetIconImg();
}

void EnemyManager::BossShout(void)
{
	for (auto& chara : characters_) {
		chara->Shout();
	}
}

void EnemyManager::DrawDebug(void)
{
	//いなかったら処理しない
	if (characters_.empty())return;

	VECTOR pos = ConvWorldPosToScreenPos(characters_[0]->GetPos());
	DrawFormatString(0, 80, 0xffffff, "SCPOS={%.1f,%.1f}", pos.x, pos.y);
	for (auto& chara : characters_) {
		chara->DrawDebug();
	}
}
