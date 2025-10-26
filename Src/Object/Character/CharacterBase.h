#pragma once
#include<DxLib.h>
#include<memory>
#include "../../Common/Quaternion.h"
#include"../../Manager/GameSystem/AnimationController.h"

//class AnimationController;
class AttackManager;
class ModelMaterial;
class ModelRenderer;
class Game;

class CharacterBase
{
public:
	//初期化用
	static constexpr float INIT_MODEL_ROT = 180.0f;	//Unity形式のモデルの形を合わせる用

	//回転作業
	static constexpr float PER_ROT = 0.1f;			//フレームごとの回転(球面補間における時間の増加量を表す)
	static constexpr float THRESHOLD_ROT = 0.1f;	//回転のしきい値を表す

	//回転量
	static constexpr float DEG_FORWARD = 0.0f;	//前方
	static constexpr float DEG_BACK = 180.0f;	//後方
	static constexpr float DEG_LEFT = 270.0f;	//左方
	static constexpr float DEG_RIGHT = 90.0f;	//右方

	static constexpr VECTOR CHARACTER_HEIGHT = { 0.0f,150.0f,0.0f };	//キャラクターの身長
	static constexpr float CHARACTER_RADIUS = 80.0f;					//キャラクターの判定の大きさ

	//デバッグ用
	static constexpr int COLOR = 0xffffff;

	CharacterBase(void);
	virtual ~CharacterBase(void);

	virtual const bool Init(const int _num) = 0;
	virtual void Update(void);
	virtual void Update(const VECTOR _pPos, AttackManager& _atk);	//敵用(敵はキャラクター自身が行動を決めるのでそれ用に分けている)
	void UpdateAnimOnly(void);
	virtual void Draw(void);
	virtual const bool Release(void);

	// 各種方向を取得
	VECTOR GetForward(void) const;
	VECTOR GetBack(void) const;
	VECTOR GetRight(void) const;
	VECTOR GetLeft(void) const;
	VECTOR GetUp(void) const;
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& _vec) const;

	//座標取得
	const VECTOR GetPos(void)const;				//足元
	const VECTOR GetHeight(void)const;			//頭辺り

	//当たり判定の半径
	const float GetCollisionRadius(void)const;

	void SetPrePos(void);

	//回転情報の取得
	const Quaternion GetQua(void)const;

	//個体名取得
	const std::string GetSpeciesName(void)const;

	//判定
	virtual const bool IsAlive(void)const;

	//ダメージを与える
	virtual void Damage(const float _pow) = 0;

	//死亡させる
	virtual void Deth(void);

	//デバッグ
	virtual void DrawDebug(void);
	void DrawCupcel(void);

protected:
	//必ず更新・初期化処理に入れること
	//************************************************
	virtual void InitAnim(void) = 0;
	virtual void InitUI(void) = 0;
	void UpdateRotQuat(void);
	//**********************************************
	//UI描画
	virtual void DrawUI(void) = 0;

	//回転目標角度
	void SetGoalRot(const float _rad);
	//回転
	void Rotation(void);

#pragma region モデル基礎制御
	int modelId_;	//モデルID

	VECTOR pos_;	//座標
	VECTOR prePos_;	//座標
	VECTOR uiPos_; //UI表示位置
	VECTOR scl_;	//モデル大きさ
	VECTOR rot_;	//回転情報(XYZ)
	Quaternion characterRotY_;	//Y軸回転用

	Quaternion goalQua_;	//目標の回転量
	float stepRotation_;	//回転のカウンター

	//各情報の行列か
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ローカル回転
	Quaternion quaRotLocal_;
#pragma endregion

	

#pragma region 描画関連
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
#pragma endregion

	//個体名
	std::string speciesName_;

	//アニメーション
	std::unique_ptr<AnimationController> animController_;

	//ステータスなど
	float hp_;		
	float colRadius_;	//当たり判定の大きさ
};

