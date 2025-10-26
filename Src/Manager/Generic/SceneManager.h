#pragma once
#include <chrono>
#include<vector>
#include<memory>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

//定義
//シーン遷移::メインシーンの切り換え
//シーン移動::サブシーンとの移動のやり取り

class SceneManager
{

public:
	static constexpr VECTOR LIGHT_DIR = { 0.00f, -1.00f, 1.00f };
	static constexpr float STICK_START_POW = 0.5f;	//コントローラーのスティック閾値

	static constexpr int POP_SCENE_TYPE_NUM = 3;	//ポップ可能なシーンの種類数

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,

		//ポップ可能シーン
		PAUSE,
		KEY_CONFIG,
		SWITCH_CNTL,
	};

	enum class CNTL
	{
		NONE,
		KEY,
		PAD,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	//シーン遷移
	void SetInitScene(std::shared_ptr<SceneBase>_scene);	//初期化時のみ使用
	/// <summary>
	/// シーンの変更
	/// </summary>
	/// <param name="_scene">メインシーン(Scene/Main/)</param>
	void ChangeScene(std::shared_ptr<SceneBase>_scene);
	/// <summary>
	/// 追加シーンの生成
	/// </summary>
	/// <param name="_scene">追加シーン</param>
	void PushScene(std::shared_ptr<SceneBase>_scene);
	/// <summary>
	/// 追加されているシーンを一つ削除（最新のシーン）
	/// </summary>
	void PopScene(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera& GetCamera(void) const;

	//操作種別の取得・設定
	const CNTL GetController(void)const;
	void SetController(const CNTL _cntl);
	void SwitchController(void);	//切り換え

	//更新処理のスピード倍率
	void SetUpdateSpeedRate_(const float _percent);	//設定
	/// <summary>
	/// 倍率単体を取得(インクリメントならこれを使用)
	/// </summary>
	/// <returns>倍率</returns>
	const float GetUpdateSpeedRate_(void)const;
	/// <summary>
	/// 倍率単体<%表記>を取得(インクリメントならこれを使用)
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetUpdateSpeedRatePercent_(void)const;
	/// <summary>
	/// 倍率を含めて計算したものを返却
	/// </summary>
	/// <param name="_target">更新スピード</param>
	/// <returns>更新スピード(倍率影響済み)</returns>
	const float GetScaleUpdateSpeedRate(const float _target)const;

	int GetMainScreen(void)const { return mainScreen_; }
	const float GetTotalTime(void)const { return totalTime_; }

private:

	// 静的インスタンス
	static SceneManager* instance_;

	//コントローラ識別
	CNTL cntl_;

	std::shared_ptr<Camera> camera_;

	// メインスクリーン
	int mainScreen_;

	// フェード
	Fader* fader_;

	// 各種シーン
	std::vector<std::shared_ptr<SceneBase>> scenes_;	//シーン格納
	std::shared_ptr<SceneBase>nextScene_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	float totalTime_;

	//更新カウンターの倍率
	float updateSpeedRate_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(void);

	// フェード
	void Fade(void);
};