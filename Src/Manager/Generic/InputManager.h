#pragma once
#include<unordered_map>	//対応表のため
#include<vector>		//デバイスを追加するため
#include<string>		//対応キーのため
#include<map>
#include<functional>
#include <Dxlib.h>
#include "../../Common/Vector2.h"
#include"SceneManager.h"

class InputManager
{

public:
	static constexpr int ANALOG_STHICK_THRESHOLD = 10000;	//スティック閾値
	static constexpr int ANALOG_TRIGGER_THRESHOLD = 128;	//トリガー閾値
	static constexpr int KEY_ALL = 256;	//キー種総数

	/// <summary>
	/// 周辺機器種別
	/// </summary>
	enum class PERIPHERAL_TYPE {
		//キーマウ操作
		KEYBOARD,
		MOUSE,
		//PAD操作
		GAMEPAD,
		X_ANALOG,
		MAX
	};

	/// <summary>
	/// アナログ入力種別
	/// </summary>
	enum class ANALOG_INPUT_TYPE {
		LS_UP,		//左スティックの上
		LS_DOWN,	//左スティックの下
		LS_RIGHT,	//左スティックの右
		LS_LEFT,	//左スティックの左
		LT,			//左トリガー
		RS_UP,		//右スティックの上
		RS_DOWN,	//右スティックの下
		RS_RIGHT,	//右スティックの右
		RS_LEFT,	//右スティックの左
		RT,			//右トリガー
		end
	};

	enum class MOUSE_INPUT {
		L_CLICK,//左クリック
		R_CLICK,//右クリック
		M_CLICK,//ホイールクリック
		UP,		//上移動
		DOWN,	//下移動
		LEFT,	//左移動
		RIGHT,	//右移動
		MAX
	};

	enum class INPUT_RECORD {
		CURRENT,
		LAST
	};


	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096,	// キー入力
		MAX
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// リソースの破棄
	void Destroy(void);

private:
	//キー対応初期化
	void ResetInput(void);
	//アナログキーの入力判別の関数定義
	void AnalogInputFuncInit(void);

	//マウスホイールの入力判別の関数定義
	void MouseInputFuncInit(void);

	//コードの現在又は１フレーム前の入力を渡す
	const bool IsInputRecord(const std::string& _eventCode,const INPUT_RECORD& _record, const bool _isDistinguish);

public:
	/// <summary>
	/// キーのダウントリガ
	/// </summary>
	/// <param name="_eventCode">登録名</param>
	/// <returns></returns>
	bool IsTrigerrDown(const std::string& _eventCode, bool _isDistinguish = true);

	/// <summary>
	/// キーのアップトリガ
	/// </summary>
	/// <param name="_eventCode">登録名</param>
	/// <returns></returns>
	bool IsTrigerrUp(const std::string& _eventCode, bool _isDistinguish = true);

	bool IsPressed(const std::string& _eventCode, bool _isDistinguish = true);

private:
	static InputManager* instance_;


	/// <summary>
	/// 入力紐づけ(機器・対応コード)
	/// </summary>
	struct InputCode
	{
		PERIPHERAL_TYPE type;//周辺機器
		uint32_t code;		//入力コード(汎用)
	};

	//<登録名、対応キー>(キーボード・PADに対応)
	using InputTable_t = std::unordered_map<std::string, std::vector<InputCode>>;
	InputTable_t inputTable_;	//イベントと入力の対応表

	std::vector<std::string>inputListForDisplay_;	//キーコンフィグ用(前期実装未定)

	//スティック関係は少し別種なのでこちらで扱う(基本的にRスティックやトリガー用)
	using AnalogInputTable_t = std::unordered_map<ANALOG_INPUT_TYPE, std::function<bool(const XINPUT_STATE&)>>;
	AnalogInputTable_t analpgInputTable_;

	//<登録名,押下状態>
	using InputData_t = std::unordered_map<std::string, bool>;
	//InputData_t currentInput_;	//イベントに対応するボタンが押されているか
	//InputData_t lastInput_;		//イベントに対応するボタンが押されているか(１フレーム前)

	//押されたとき何で押されたかを覚えておく
	using InputPeriTypeData_t = std::unordered_map<std::string, std::vector<PERIPHERAL_TYPE>>;
	InputPeriTypeData_t currentInptuPeri_;
	InputPeriTypeData_t lastInptuPeri_;


	//マウスホイールに関して
	int mouseState_;		//マウスの入力状態
	Vector2 mousePos_;		//マウス位置
	Vector2 centerMousePos_;	//マウス位置(１フレーム前)

	using MouseInputTable_t = std::unordered_map<MOUSE_INPUT, std::function<bool(void)>>;
	MouseInputTable_t mouseInputTable_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager(void);
	InputManager(const InputManager& manager) = default;
	~InputManager(void);
};