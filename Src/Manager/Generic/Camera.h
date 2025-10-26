#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../../Common/Quaternion.h"

class Transform;

class Camera
{

public:
	//カメラの描画域(Near,Far)関連の定数------------------------------------------------------
	
	static constexpr float SPEED = 10.0f;			//カメラスピード：NEAR

	
	static constexpr float CAMERA_NEAR = 40.0f;		//カメラクリップ：NEAR

	
	static constexpr float CAMERA_FAR = 15000.0f;	//カメラクリップ：NEAR

	//カメラ座標関連の定数---------------------------------------------------------------------
	
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 700.0f, -700.0f };			//カメラの初期座標

	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };			//カメラ位置から注視点までの相対座標

	
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 700.0f, -600.0f };	//追従対象からカメラ位置までの相対座標(完全追従)

	//static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };	//追従対象からカメラ位置までの相対座標(ばね付き)

	static constexpr float ROCK_DISTANCE_MIN = 500.0f;		//ロックオン時に最低限離れておく距離

	static constexpr float ROCK_MAGNIFICATION_Y = 0.25f;	//ロックオン時のY座標調整用の倍率
	
	//カメラ移動関連の定数---------------------------------------------------------------------
	
	static constexpr float MAX_MOVE_SPEED = 5.0f;	//移動速度の最大値
	static constexpr float MAX_ROT_SPEED = 0.025f;	//移動速度の最大値

	//カメラ揺らし関連の定数--------------------------------------------------------------------

	static constexpr float TIME_SHAKE = 0.5f;		//時間

	static constexpr float WIDTH_SHAKE = 5.0f;		//幅

	static constexpr float SPEED_SHAKE = 40.0f;		//スピード

	//リセット関係
	static constexpr float RESET_TIME = 1.0f;
	static constexpr float RESET_STEP = 0.05f;

	//距離最低制限
	static constexpr float UNDER_LIMIT_Y = 280.0f;

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 45.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = -25.0f * (DX_PI_F / 180.0f);

	//線形補完
	static constexpr float NO_LERP = 1.0f;
	static constexpr float LERP_SPEED = 0.05f;

	//カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//定点カメラ
		FREE,			//フリーモード
		FOLLOW,			//追従モード
		FOLLOW_SPRING,	//ばね付き追従モード
		SHAKE,			//カメラ揺らし
		LOCKON,			//ロックオン
		RESET,			//カメラ位置リセット用
		AUTO_MOVE,		//目標位置まで自動的に移動
	};

	struct FOR_FOLLOW_INFO
	{
		VECTOR pos;
		Quaternion quaRot;
	};

	//コンストラクタ
	Camera(void);

	//デストラクタ
	~Camera(void);

	//初期化処理
	void Init(void);

	//更新処理
	void Update(void);

	//カメラの描画モード関連------------------
	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//定点カメラ
	void SetBeforeDrawFree(void);			//フリーカメラ
	void SetBeforeDrawFollow(void);			//追従カメラ
	void SetBeforeDrawLockOn(void);			//ロックオンカメラ
	void SetBeforeDrawShake(void);			//カメラシェイク
	void SetBeforeDrawReset(void);			//カメラリセット
	void SetBeforeDrawAutoMove(void);			//カメラリセット

	//----------------------------------------
	// 描画処理
	void Draw(void);

	//解放処理
	void Release(void);

	//座標取得
	const VECTOR GetPos(void) const;
	const VECTOR GetRockPos(void)const;
	const VECTOR GetGoalPos(void)const { return goalPos_; }

	//回転取得
	const Quaternion GetRot(void)const;
	const VECTOR GetAngle(void)const;
	//回転スピード
	const float GetRotSpeed(void)const;
	void SetRotSpeed(const float _speed);

	//カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	void SetFollow(const VECTOR _pos,const Quaternion _qua);


	//座標設定
	void SetPos(const VECTOR& pos,const VECTOR& focus);
	void SetPos(const VECTOR& pos);
	void SetFocusPos(const VECTOR& _focus);
	void SetRockPos(const VECTOR& _rock);
	void SetGoalPos(const VECTOR& _goal);

	//カメラを初期位置に戻す
	void SetDefault(void);


	const MODE GetMode(void);
	const bool IsFinishShake(void) { return finishShake_; }

	void DrawDebug(void);
private:

	//追従対象
	FOR_FOLLOW_INFO followObject_;
	//開始・目標位置(リセット時などに使用)
	FOR_FOLLOW_INFO start_;
	FOR_FOLLOW_INFO goal_;
	float stepReset_;
	bool isReset_;

	//カメラモード
	MODE mode_;			
	MODE currentMode_;	//Shake・Resetに遷移する際に現在の物を保存する

	//カメラの位置
	VECTOR pos_;

	//ロックオン対象の位置
	VECTOR lockPos_;
	VECTOR prevGoalPos_;
	VECTOR lockOnGoalPos_;
	float lockOnLerpStep_;

	//カメラの注視点
	VECTOR focusPos_;
	VECTOR goalFocusPos_;
	
	//移動目標位置
	VECTOR goalPos_;

	//カメラの上方向
	VECTOR cameraUp_;

	//カメラの回転
	Quaternion rot_;

	//回転スピード
	float rotSpeed_;

	// カメラ角度(rad)
	VECTOR angles_;
	// X軸回転が無い角度
	Quaternion rotOutX_;

	//画面揺らし用
	float stepShake_;

	//画面揺れが終わったか
	bool finishShake_;
	//演出前位置
	VECTOR defaultPos_;
	//揺れ方向
	VECTOR shakeDir_;
	//補完スピード
	float lerpSpeed_;

	//回転
	void Rotation(void);

};

