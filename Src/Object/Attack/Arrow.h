#pragma once
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Manager/GameSystem/AttackManager.h"

class Quaternion;

class Arrow
{
public:
	//定数
	static constexpr VECTOR ARROW_LOCAL_POS = { 15.0f,30.0f,50.0f };	//打ち始めに違和感がないように調整
	static constexpr float GRAVITY = 0.5f;			//重力
	static constexpr float START_UP_ANGLE = 10.0f;	//最初の上アングル
	static constexpr float MOVE_SPEED = 5.0f;

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	/// <summary>
	/// 弓矢の生成
	/// </summary>
	/// <param name="_master">攻撃者陣営</param>
	/// <param name="_mdlId">モデル</param>
	/// <param name="_pow">攻撃力</param>
	/// <param name="_qua">回転</param>
	Arrow(const AttackManager::ATTACK_MASTER& _master,const int _mdlId,const VECTOR _pos,const float _pow,const Quaternion& _qua);
	~Arrow(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release();
	void Destroy(void);

	const VECTOR GetPos(void)const { return pos_; }
	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

	//状況変位
	void ChangeState(const STATE _state) { state_ = _state; }

private:
	void UpdateRotQuat(void);
	//移動処理
	void Move(void);
	//生存フラグ設定
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }

	// 各種方向を取得
	VECTOR GetForward(void) const;
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& _vec) const;

	//位置情報
	int modelId_;	//モデルID
	VECTOR pos_;	//座標
	VECTOR scl_;	//モデル大きさ
	VECTOR rot_;	//回転情報(XYZ)

	//各情報の行列か
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ローカル回転
	Quaternion quaRotLocal_;

	//発生者
	AttackManager::ATTACK_MASTER master_;
	//状態
	STATE state_;
	//攻撃力
	float atkPow_;
	//生存判定
	bool isAlive_;
};

