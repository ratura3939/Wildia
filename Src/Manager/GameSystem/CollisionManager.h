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
	/// 当たり判定総括
	/// </summary>
	/// <param name="_player">プレイヤー情報</param>
	/// <param name="_enemys">敵情報</param>
	/// <param name="_atks">攻撃情報</param>
	/// <returns>true=スロー演出が入る/false=通常</returns>
	const bool Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);

private:
	/// <summary>
	/// プレイヤーの当たり判定関係まとめ
	/// </summary>
	/// <param name="_player">プレイヤー情報</param>
	/// <param name="_atk">攻撃情報</param>
	void CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks);

	/// <summary>
	/// 敵の当たり判定関係まとめ
	/// </summary>
	/// <param name="_enemy">敵情報</param>
	/// <param name="_atk">攻撃情報</param>
	void CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);

	bool isSlow_;	//スロー演出するか
};

