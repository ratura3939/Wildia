#pragma once

class Game;
class EnemyManager;
class PlayerManager;

class LockOnManager {
public:
	LockOnManager(Game& _gameScene, PlayerManager& _player, EnemyManager& _enemyInfo);
	~LockOnManager(void);

	void Update(void);

	//ロックオンが可能かどうかを返す
	const bool CanLockOn(void);

	//ロックオン・解除処理
	void LockOn(void);
	void LockOff(void);
	
private:
	/// <summary>
	/// ロックオンの対象決め
	/// </summary>
	/// <param name=""></param>
	const int DecideRockEnemy(void);

	Game& scene_;
	PlayerManager& master_;	//ロックオンを行う者
	EnemyManager& targets_;	//ロックオンをされる者

	bool isLockOn_;	//ロックオンをしているか

	int nearTargetNum_;		//一番近いターゲット(ロックオン対象者)
	int preNearTargetNum_;	//1フレーム前の一番近いターゲット(ロックオン対象者)
};