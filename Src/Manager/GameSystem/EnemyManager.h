#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;
class EnemyCount;
class Game;
class AttackManager;

class EnemyManager
{
public:
	static constexpr int ENEMY_NUM = 4;			//敵総数

	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;

	static constexpr float TARGETTING_PERMISSION_DISTANCE = 1000.0f;	//ターゲティングを許可する範囲

	//デバッグ用
	static constexpr int ATTACK_TIME = 100;
	static constexpr int ATTACK_TIME_START = 20;
	static constexpr int ATTACK_TIME_END = 90;

	static constexpr VECTOR INIT_1 = { 0.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_2 = { 500.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_3 = { 0.0f,0.0f,-1000.0f };
	static constexpr VECTOR INIT_4 = { -500.0f,0.0f,-1000.0f };

	EnemyManager(Game& _scene);
	~EnemyManager(void);

	void Init(void);
	void Update(const VECTOR& _playerPos, AttackManager& _atkMng);
	void UpdateAnim(void);
	void Draw(void);
	void Release(void);

	//敵たちの取得
	std::vector<std::weak_ptr<EnemyBase>> GetEnemys(void);

	//位置・回転取得
	const VECTOR GetPos(const int _num);//座標
	const Quaternion GetQua(const int _num);//回転

	//一番近い敵の配列番号を返す
	int GetNearEnemyNum(const VECTOR _pPos);
	//画面内であるかどうか
	bool InsideScreen(const VECTOR _pos);

	/// <summary>
	/// 戦闘状態の敵が居るかどうか
	/// </summary>
	/// <returns>true=いる/false=いない</returns>
	bool IsBattleEnemy(void);

	/// <summary>
	/// IsBattleEnemyのONまたはOFFのトリガー
	/// </summary>
	/// <returns>true=トリガ起動/false=変化なし</returns>
	bool IsSwitchBattleOrNomalEnemyTrg(void);

	//アニメのスピードの設定
	void SetAnimSpeedRate(const float _percent);

	/// <summary>
	/// ロックオンされる可能性のある敵をセット
	/// </summary>
	/// <param name="_num">敵番号</param>
	void SetTargetEnemy(const int _num);

	/// <summary>
	/// ロックオン対象になるような敵がいないときに起こる処理
	/// </summary>
	/// <param name=""></param>
	void NoTargetEnemy(void);

	//ロックオンされた処理
	void LokedOn(const int _num);

	void CreateBoss(void);
	void BossShout(void);

	void DrawDebug(void);

private:
	std::vector<std::shared_ptr<EnemyBase>> characters_;
	bool preBattle_;	//位置フレーム前が戦闘状態であるかどうか

	int enemyCnt_;		//敵カウント
	int* numImg_;		//番号
	VECTOR platePos_;	//敵数表示　プレート

	bool createBoss_;	//ボスを生成した形跡

	std::unique_ptr<EnemyCount>counterUI_;	//敵残数カウンター

	Game& gameScene_;
};

