#pragma once

class Game;
class EnemyManager;
class PlayerManager;

class LockOnManager {
public:
	LockOnManager(Game& _gameScene, PlayerManager& _player, EnemyManager& _enemyInfo);
	~LockOnManager(void);

	void Update(void);

	//���b�N�I�����\���ǂ�����Ԃ�
	const bool CanLockOn(void);

	//���b�N�I���E��������
	void LockOn(void);
	void LockOff(void);
	
private:
	/// <summary>
	/// ���b�N�I���̑Ώی���
	/// </summary>
	/// <param name=""></param>
	const int DecideRockEnemy(void);

	Game& scene_;
	PlayerManager& master_;	//���b�N�I�����s����
	EnemyManager& targets_;	//���b�N�I����������

	bool isLockOn_;	//���b�N�I�������Ă��邩

	int nearTargetNum_;		//��ԋ߂��^�[�Q�b�g(���b�N�I���Ώێ�)
	int preNearTargetNum_;	//1�t���[���O�̈�ԋ߂��^�[�Q�b�g(���b�N�I���Ώێ�)
};