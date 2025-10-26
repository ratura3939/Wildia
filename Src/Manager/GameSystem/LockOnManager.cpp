#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../GameSystem/EnemyManager.h"
#include"../GameSystem/PlayerManager.h"
#include"../../Scene/Main/Game.h"
#include "LockOnManager.h"

LockOnManager::LockOnManager(Game& _gameScene, PlayerManager& _player, EnemyManager& _enemyInfo) :scene_(_gameScene), master_(_player), targets_(_enemyInfo)
{
	nearTargetNum_ = -1;
	preNearTargetNum_ = -1;
	isLockOn_ = false;
}

LockOnManager::~LockOnManager(void)
{
}

void LockOnManager::Update(void)
{
	//�Ώێ҂̑I��
	preNearTargetNum_ = nearTargetNum_;	//�ۑ�
	nearTargetNum_ = DecideRockEnemy();	//����

	//�񃍃b�N�I����
	if (!isLockOn_) {
		//���b�N�I���Ώۂ��ς������
		if (preNearTargetNum_ != nearTargetNum_) {
			//�Ώۂ̍X�V
			targets_.SetTargetEnemy(nearTargetNum_);
		}
		//�ΏۂƂȂ�G�����Ȃ��Ƃ�
		if (nearTargetNum_ < 0) {
			targets_.NoTargetEnemy();
		}
	}
	else {
		//���b�N�I�����ΏۂƂȂ�G�����Ȃ�������
		if (nearTargetNum_ < 0) {
			LockOff();
		}
	}
}

const bool LockOnManager::CanLockOn(void)
{
	return targets_.GetNearEnemyNum(master_.GetPos()) >= 0;
}

void LockOnManager::LockOn(void)
{
	//���ʂȏ��������Ȃ�����
	if (isLockOn_)return;

	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.SetRockPos(targets_.GetPos(nearTargetNum_));	//���b�N�I���Ώۂ̐ݒ�
	master_.RedyLockOn();
	targets_.LokedOn(nearTargetNum_);
	camera.ChangeMode(Camera::MODE::LOCKON);

	isLockOn_ = true;
}

void LockOnManager::LockOff(void)
{
	//���ʂȏ��������Ȃ�����
	if (!isLockOn_)return;

	Camera& camera = SceneManager::GetInstance().GetCamera();
	master_.RedyLockOff();
	targets_.NoTargetEnemy();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	//�]�[���𑱂����Ȃ����߂�
	scene_.ChangeActionDirec(Game::ACTION_DIRECTION::NOMAL);
	scene_.EndSlow();
	//�Ώۂ��L�����Z�������Ƃ݂Ȃ�����������
	nearTargetNum_ = -1;

	isLockOn_ = false;
}

const int LockOnManager::DecideRockEnemy(void)
{
	return targets_.GetNearEnemyNum(master_.GetPos());
}
