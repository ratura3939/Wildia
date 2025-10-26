#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class Game;
class EnemyManager;
class PlayerChara;
class AttackManager;
class LockOnManager;

class PlayerManager
{
public:
	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;
	//デバッグ用
	static constexpr int ATTACK_TIME = 50;

	//状態上限時間
	static constexpr int LIMIT_AVOID_STATE = 30;	//回避

	PlayerManager(Game& _gameScene, EnemyManager& _enemy);
	~PlayerManager(void);

	void Init(void);
	void Update(AttackManager& _atk);
	void Draw(void);
	void Release(void);

	//キャラクター取得
	std::weak_ptr<PlayerChara> GetPlayer(void);

	//位置・回転取得
	const VECTOR GetPos(void);			//座標
	const Quaternion GetQua(void);		//回転
	const VECTOR GetFocusPoint(void);	//注視点

	//生存しているか
	const bool IsAlive(void)const;

	//ロックオン・オフ時に必要な処理
	void RedyLockOn(void);
	void RedyLockOff(void);
	
	//デバッグ
	void DrawDebug(void);

private:
	/// <summary>
	/// プレイヤーからの入力受付関係
	/// </summary>
	void UserInput(AttackManager& _atk);

	/// <summary>
	/// 回避可能方向に移動入力があるか
	/// </summary>
	const bool IsDudgeMove(void)const;
	void DoDudge(void);

	void RedyStateCount(const int _limit);

	Game& scene_;	//ゲームクラス参照
	std::shared_ptr<PlayerChara> character_;//キャラクター
	std::unique_ptr<LockOnManager>lockOn_;	//ロックオン関係

	int stateCnt_;		//キャラクター状態管理用カウンター
	int stateLimit_;	//状態をの時間上限
};

