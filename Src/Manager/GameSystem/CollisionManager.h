#pragma once
#include<vector>
#include<memory>
#include"AttackManager.h"

class PlayerChara;
class EnemyBase;
class AttackBase;

class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);

	/// <summary>
	/// �����蔻�葍��
	/// </summary>
	/// <param name="_player">�v���C���[���</param>
	/// <param name="_enemys">�G���</param>
	/// <param name="_atks">�U�����</param>
	/// <returns>true=�X���[���o������/false=�ʏ�</returns>
	const bool Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);

private:
	/// <summary>
	/// �v���C���[�̓����蔻��֌W�܂Ƃ�
	/// </summary>
	/// <param name="_player">�v���C���[���</param>
	/// <param name="_atk">�U�����</param>
	void CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks);

	/// <summary>
	/// �G�̓����蔻��֌W�܂Ƃ�
	/// </summary>
	/// <param name="_enemy">�G���</param>
	/// <param name="_atk">�U�����</param>
	void CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);

	bool isSlow_;	//�X���[���o���邩
};

