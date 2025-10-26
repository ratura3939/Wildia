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
	//�f�o�b�O�p
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
	//�c��J�E���g�̂̐ݒ�
	counterUI_->SetNumImg(numImg_[enemyCnt_]);

	preBattle_ = false;
}

void EnemyManager::Update(const VECTOR& _playerPos, AttackManager& _atkMng)
{
	preBattle_ = IsBattleEnemy();

	//���Ȃ������珈�����Ȃ�
	if (characters_.empty())return;

	counterUI_->Update();

	//���S�����L�����N�^�[�̔z��ԍ��ۑ��Ƃ��̃J�E���^�[
	std::vector<int>dethEnemy = {};
	int counter = 0;
	//�G�̌̕���
	for (auto& chara : characters_) {
		//�X�V��������
		chara->Update(_playerPos, _atkMng);

		//���S���Ă�����
		if (!chara->IsAlive()) {
			//���S���X�g�ɒǉ�
			dethEnemy.push_back(counter);
		}
		//�̃J�E���^�[����
		counter++;
	}

	//���S���X�g����
	int dethCnt = 0;
	for (auto& idx : dethEnemy) {
		//�Y���̓G������
		characters_.erase(characters_.begin()+(idx-dethCnt));
		enemyCnt_--;

		dethCnt++;
	}
	if (dethCnt != 0) {
		//�c��J�E���g�̂̐ݒ�
		counterUI_->SetNumImg(numImg_[enemyCnt_]);
		if (enemyCnt_ <= 0 && !createBoss_) {
			//�V�[���Ƀ{�X�o����`����
			gameScene_.StartBossFaze();
		}
	}
}

void EnemyManager::UpdateAnim(void)
{
	for (auto& chara : characters_) {
		//�X�V��������
		chara->UpdateAnimOnly();
	}
}

void EnemyManager::Draw(void)
{
	//���Ȃ������珈�����Ȃ�
	if (characters_.empty())return;

	for (auto& chara : characters_) {
		chara->Draw();
	}
	counterUI_->Draw();
}

void EnemyManager::Release(void)
{
	//���Ȃ������珈�����Ȃ�
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
	//�e�L�g�[�Ȃł����l
	double min = 100000.0;

	for (int i = 0; i < characters_.size(); i++) {
		//��ʓ��ɂ��Ȃ��Ƃ��͂���
		VECTOR myPos = characters_[i]->GetPos();
		distance = VSub(characters_[i]->GetPos(), _pPos);

		float disMag = Utility::MagnitudeF(distance);

		if (/*!InsideScreen(myPos)||*/ disMag > TARGETTING_PERMISSION_DISTANCE)continue;

		//������r
		
		if (min > disMag) {
			nearNum = i;
			min = disMag;
		}
	}

	//�f�o�b�O�p
	if (nearNum != -1)characters_[nearNum]->SetColor(0xffffff);

	return nearNum;
}

bool EnemyManager::InsideScreen(const VECTOR _pos)
{
	//���[���h���W���X�N���[�����W�ɕϊ�
	VECTOR screenPos = ConvWorldPosToScreenPos(_pos);
	//��ʓ��ɂ��邩
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
		//�퓬��Ԃ̓G��������
		if (chara->GetNowState() == EnemyBase::ENEMY_STATE::BATTLE) {
			return true;
		}
	}
	return false;
}

bool EnemyManager::IsSwitchBattleOrNomalEnemyTrg(void)
{
	//�X�V�O�̏�ԂƍX�V��̕����r
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
	//�����ȊO�̕����������ݒ�
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//�ݒ�
		characters_[i]->SetIsLockTraget(setFlag);
	}
}

void EnemyManager::NoTargetEnemy(void)
{
	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		//�ݒ�
		characters_[i]->SetIsLockTraget(false);
		characters_[i]->SetIsLocked(false);
	}
}

void EnemyManager::LokedOn(const int _num)
{
	//�����ȊO�̕����������ݒ�
	bool setFlag = false;

	for (int i = 0; i < static_cast<int>(characters_.size()); i++) {
		setFlag = false;
		if (i == _num)setFlag = true;
		//�ݒ�
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
	//���Ȃ������珈�����Ȃ�
	if (characters_.empty())return;

	VECTOR pos = ConvWorldPosToScreenPos(characters_[0]->GetPos());
	DrawFormatString(0, 80, 0xffffff, "SCPOS={%.1f,%.1f}", pos.x, pos.y);
	for (auto& chara : characters_) {
		chara->DrawDebug();
	}
}
