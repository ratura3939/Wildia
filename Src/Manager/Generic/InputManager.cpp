#include <DxLib.h>
#include"../../Application.h"
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *instance_;
}

void InputManager::Init(void)
{
	ResetInput();			//入力紐づけ
	AnalogInputFuncInit();	//アナログ入力関数定義
	MouseInputFuncInit();	//マウス入力関数定義
}

void InputManager::Update(void)
{
	//PAD関係は１Pの事しか見ていない
	//複数人を想定するのなら要改良
	//キーボード関係とPad関係で分けるのがよさそう？

	//lastInput_ = currentInput_;
	lastInptuPeri_ = currentInptuPeri_;
	
	//キーボード
	char keystate[KEY_ALL] = {};
	GetHitKeyStateAll(keystate);
	//マウス
	mouseState_ = GetMouseInput();
	//マウス位置
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	//パッド
	int padstate = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//アナログ
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//項目分回す
	for (const auto& keyvalue : inputTable_) {
		bool pressed = false;	//押されているかどうかのフラグ
		std::vector<PERIPHERAL_TYPE> inputTypes = {};
		//中身の動的配列をfor文で回す(キーボード→PADの順で見ている)
		for (auto input : keyvalue.second) {
			//キーボードのとき
			if (input.type == PERIPHERAL_TYPE::KEYBOARD) {
				if (keystate[input.code] != 0) {
					pressed = keystate[input.code];
					//入力が行われていたらこの危機から入力があったと記録する
					if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::KEYBOARD);
				}
			}
			else if (input.type == PERIPHERAL_TYPE::GAMEPAD) {
				//パッドに何かしらの入力がありそれがコードだったとき
				pressed = padstate & input.code;
				if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::GAMEPAD);
			}
			else if (input.type == PERIPHERAL_TYPE::MOUSE) {
				//マウスに何かしらの入力がありそれがコードだったとき
				pressed = mouseInputTable_[static_cast<MOUSE_INPUT>(input.code)]();
				if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::MOUSE);
			}
			else if (input.type == PERIPHERAL_TYPE::X_ANALOG) {
				pressed = analpgInputTable_[static_cast<ANALOG_INPUT_TYPE>(input.code)](xinputState);
				if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::X_ANALOG);
			}
		}
		currentInptuPeri_[keyvalue.first] = inputTypes;
		/*if (!currentInptuPeri_[keyvalue.first].empty())
		{
			pressed = true;
		}
		currentInput_[keyvalue.first] = pressed;*/
		
	}

	//マウス位置初期化
	mousePos_ = centerMousePos_;
	SetMousePoint(mousePos_.x, mousePos_.y);
}

void InputManager::Destroy(void)
{
	delete instance_;
}

void InputManager::ResetInput(void)
{
	// ゲームで使用したいキーとその名前を、
	// 事前にここで登録しておいてください

	//移動関係<WASD・左スティック>
	inputTable_["up"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_W },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_UP) } };
	inputTable_["down"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_S },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_DOWN) } };
	inputTable_["left"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_A },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_LEFT) } };
	inputTable_["right"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_D },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_RIGHT) } };
	//移動入力(サブ)<Rスティック・方向キー>
	inputTable_["subUp"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::UP)},{PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_UP)}};
	inputTable_["subDown"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::DOWN) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_DOWN) } };
	inputTable_["subLeft"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::LEFT) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_LEFT) } };
	inputTable_["subRight"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::RIGHT) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_RIGHT) } };

	//各コマンド<PADは複数個所で兼用あり>
	inputTable_["action"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_RETURN } ,{ PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::L_CLICK)},{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_B } };//Bボタン(Aボタン：任天堂)
	inputTable_["dash"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_LSHIFT },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_A } };		//Aボタン(Bボタン：任天堂)
	inputTable_["cancel"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_Q },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_A } };			//Aボタン(Bボタン：任天堂)
	inputTable_["attack"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::L_CLICK) },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_C } };		//Xボタン(Yボタン：任天堂)
	inputTable_["jump"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_SPACE },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_X } };		//Yボタン(Xボタン：任天堂)
	inputTable_["crouch"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_LCONTROL },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_START } };//LS
	inputTable_["rock"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_R },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LT) } };		//LT
	inputTable_["arrow"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::R_CLICK) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RT) } };	//RT



	//ポーズ
	inputTable_["pause"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_TAB },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_R } };
}

void InputManager::AnalogInputFuncInit(void)
{
	analpgInputTable_[ANALOG_INPUT_TYPE::LS_UP] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLY > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::LS_DOWN] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLY < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::LS_RIGHT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLX > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::LS_LEFT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLX < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::RS_UP] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRY > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::RS_DOWN] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRY < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::RS_RIGHT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRX > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::RS_LEFT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRX < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::LT] = [](const XINPUT_STATE& _state) {
		return _state.LeftTrigger > ANALOG_TRIGGER_THRESHOLD;
	};
	analpgInputTable_[ANALOG_INPUT_TYPE::RT] = [](const XINPUT_STATE& _state) {
		return _state.RightTrigger > ANALOG_TRIGGER_THRESHOLD;
	};
}

void InputManager::MouseInputFuncInit(void)
{
	mouseInputTable_[MOUSE_INPUT::L_CLICK] = [this]() {
		return mouseState_ & MOUSE_INPUT_LEFT;
	};
	mouseInputTable_[MOUSE_INPUT::R_CLICK] = [this]() {
		return mouseState_ & MOUSE_INPUT_RIGHT;
	};
	mouseInputTable_[MOUSE_INPUT::M_CLICK] = [this]() {
		return mouseState_ & MOUSE_INPUT_MIDDLE;
	};
	mouseInputTable_[MOUSE_INPUT::UP] = [this]() {
		return mousePos_.y < centerMousePos_.y;
	};
	mouseInputTable_[MOUSE_INPUT::DOWN] = [this]() {
		return mousePos_.y > centerMousePos_.y;
	};
	mouseInputTable_[MOUSE_INPUT::LEFT] = [this]() {
		return mousePos_.x < centerMousePos_.x;
	};
	mouseInputTable_[MOUSE_INPUT::RIGHT] = [this]() {
		return mousePos_.x > centerMousePos_.x;
	};
}

const bool InputManager::IsInputRecord(const std::string& _eventCode, const INPUT_RECORD& _record,const bool _isDistinguish)
{
	//シーンマネージャが管理するコントローラ設定で
	//対応する入力方法のみ受け付ける
	//KEY＝キーボード・マウス
	//PAD＝PAD・アナログ

	
	//引数に応じて現在か１フレ前かを渡す
	std::vector<PERIPHERAL_TYPE> inputRecord = {};
	if (_record == INPUT_RECORD::CURRENT) {
		inputRecord = currentInptuPeri_[_eventCode];
	}
	else {
		inputRecord = lastInptuPeri_[_eventCode];
	}

	auto cntl = SceneManager::GetInstance().GetController();
	bool currentFlag = false;	//何かしら入力があったか

	//指定のコードで入力があった機種の経歴分回す
	for (auto& periType : inputRecord) {


		//キーボード操作の時
		if (cntl == SceneManager::CNTL::KEY) {
			//キーボードとマウスを受け付ける
			if (periType == PERIPHERAL_TYPE::KEYBOARD || periType == PERIPHERAL_TYPE::MOUSE)currentFlag = true;
		}
		//パッドの時
		else if (cntl == SceneManager::CNTL::PAD) {
			//PADとアナログを受け付ける
			if (periType == PERIPHERAL_TYPE::GAMEPAD || periType == PERIPHERAL_TYPE::X_ANALOG) currentFlag = true;
		}
		//NONEのとき
		else {
			//このルーぷに入っている時点で入力があったということ
			currentFlag = true;
		}

		//識別が必要ないなら問答無用でOK
		if (!_isDistinguish) {
			currentFlag = true;
		}
	}
	return currentFlag;
}


bool InputManager::IsTrigerrDown(const std::string& _eventCode, bool _isDistinguish)
{
	//先に要素がない場合の予防線をはる
// 反応しないだけという状態を作りたいから
//containd()=引数がキーとなる要素がないとき
	if (!currentInptuPeri_.contains(_eventCode)) {
		//参照できないので
		return false;
	}

	//[]の形で中身を見ようとすると勝手に中身が空のキーの場所が生成されてしまう
	//なのでmap型のat()関数はキー検索であり読み取り専用を使用することで中身を変えずに参照が可能になるｂ
	return IsInputRecord(_eventCode, INPUT_RECORD::CURRENT, _isDistinguish) && !IsInputRecord(_eventCode, INPUT_RECORD::LAST, _isDistinguish);
}

bool InputManager::IsTrigerrUp(const std::string& _eventCode, bool _isDistinguish)
{
	//先に要素がない場合の予防線をはる
	// 反応しないだけという状態を作りたいから
	//containd()=引数がキーとなる要素がないとき
	if (!currentInptuPeri_.contains(_eventCode)) {
		//参照できないので
		return false;
	}

	//[]の形で中身を見ようとすると勝手に中身が空のキーの場所が生成されてしまう
	//なのでmap型のat()関数はキー検索であり読み取り専用を使用することで中身を変えずに参照が可能になるｂ
	return !IsInputRecord(_eventCode, INPUT_RECORD::CURRENT, _isDistinguish) && IsInputRecord(_eventCode, INPUT_RECORD::LAST, _isDistinguish);
}

bool InputManager::IsPressed(const std::string& _eventCode, bool _isDistinguish)
{
	return IsInputRecord(_eventCode, INPUT_RECORD::CURRENT,_isDistinguish);
}

InputManager::InputManager(void)
{
	centerMousePos_ = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };
	mousePos_ = centerMousePos_;
	mouseState_ = -1;
}

InputManager::~InputManager(void)
{
}


