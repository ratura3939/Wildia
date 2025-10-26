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
	ResetInput();			//���͕R�Â�
	AnalogInputFuncInit();	//�A�i���O���͊֐���`
	MouseInputFuncInit();	//�}�E�X���͊֐���`
}

void InputManager::Update(void)
{
	//PAD�֌W�͂PP�̎��������Ă��Ȃ�
	//�����l��z�肷��̂Ȃ�v����
	//�L�[�{�[�h�֌W��Pad�֌W�ŕ�����̂��悳�����H

	//lastInput_ = currentInput_;
	lastInptuPeri_ = currentInptuPeri_;
	
	//�L�[�{�[�h
	char keystate[KEY_ALL] = {};
	GetHitKeyStateAll(keystate);
	//�}�E�X
	mouseState_ = GetMouseInput();
	//�}�E�X�ʒu
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	//�p�b�h
	int padstate = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//�A�i���O
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//���ڕ���
	for (const auto& keyvalue : inputTable_) {
		bool pressed = false;	//������Ă��邩�ǂ����̃t���O
		std::vector<PERIPHERAL_TYPE> inputTypes = {};
		//���g�̓��I�z���for���ŉ�(�L�[�{�[�h��PAD�̏��Ō��Ă���)
		for (auto input : keyvalue.second) {
			//�L�[�{�[�h�̂Ƃ�
			if (input.type == PERIPHERAL_TYPE::KEYBOARD) {
				if (keystate[input.code] != 0) {
					pressed = keystate[input.code];
					//���͂��s���Ă����炱�̊�@������͂��������ƋL�^����
					if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::KEYBOARD);
				}
			}
			else if (input.type == PERIPHERAL_TYPE::GAMEPAD) {
				//�p�b�h�ɉ�������̓��͂����肻�ꂪ�R�[�h�������Ƃ�
				pressed = padstate & input.code;
				if (pressed)inputTypes.push_back(PERIPHERAL_TYPE::GAMEPAD);
			}
			else if (input.type == PERIPHERAL_TYPE::MOUSE) {
				//�}�E�X�ɉ�������̓��͂����肻�ꂪ�R�[�h�������Ƃ�
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

	//�}�E�X�ʒu������
	mousePos_ = centerMousePos_;
	SetMousePoint(mousePos_.x, mousePos_.y);
}

void InputManager::Destroy(void)
{
	delete instance_;
}

void InputManager::ResetInput(void)
{
	// �Q�[���Ŏg�p�������L�[�Ƃ��̖��O���A
	// ���O�ɂ����œo�^���Ă����Ă�������

	//�ړ��֌W<WASD�E���X�e�B�b�N>
	inputTable_["up"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_W },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_UP) } };
	inputTable_["down"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_S },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_DOWN) } };
	inputTable_["left"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_A },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_LEFT) } };
	inputTable_["right"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_D },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LS_RIGHT) } };
	//�ړ�����(�T�u)<R�X�e�B�b�N�E�����L�[>
	inputTable_["subUp"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::UP)},{PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_UP)}};
	inputTable_["subDown"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::DOWN) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_DOWN) } };
	inputTable_["subLeft"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::LEFT) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_LEFT) } };
	inputTable_["subRight"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::RIGHT) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RS_RIGHT) } };

	//�e�R�}���h<PAD�͕������Ō��p����>
	inputTable_["action"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_RETURN } ,{ PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::L_CLICK)},{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_B } };//B�{�^��(A�{�^���F�C�V��)
	inputTable_["dash"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_LSHIFT },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_A } };		//A�{�^��(B�{�^���F�C�V��)
	inputTable_["cancel"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_Q },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_A } };			//A�{�^��(B�{�^���F�C�V��)
	inputTable_["attack"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::L_CLICK) },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_C } };		//X�{�^��(Y�{�^���F�C�V��)
	inputTable_["jump"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_SPACE },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_X } };		//Y�{�^��(X�{�^���F�C�V��)
	inputTable_["crouch"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_LCONTROL },{ PERIPHERAL_TYPE::GAMEPAD,PAD_INPUT_START } };//LS
	inputTable_["rock"] = { { PERIPHERAL_TYPE::KEYBOARD,KEY_INPUT_R },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::LT) } };		//LT
	inputTable_["arrow"] = { { PERIPHERAL_TYPE::MOUSE,static_cast<int>(MOUSE_INPUT::R_CLICK) },{ PERIPHERAL_TYPE::X_ANALOG,static_cast<int>(ANALOG_INPUT_TYPE::RT) } };	//RT



	//�|�[�Y
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
	//�V�[���}�l�[�W�����Ǘ�����R���g���[���ݒ��
	//�Ή�������͕��@�̂ݎ󂯕t����
	//KEY���L�[�{�[�h�E�}�E�X
	//PAD��PAD�E�A�i���O

	
	//�����ɉ����Č��݂��P�t���O����n��
	std::vector<PERIPHERAL_TYPE> inputRecord = {};
	if (_record == INPUT_RECORD::CURRENT) {
		inputRecord = currentInptuPeri_[_eventCode];
	}
	else {
		inputRecord = lastInptuPeri_[_eventCode];
	}

	auto cntl = SceneManager::GetInstance().GetController();
	bool currentFlag = false;	//����������͂���������

	//�w��̃R�[�h�œ��͂��������@��̌o�𕪉�
	for (auto& periType : inputRecord) {


		//�L�[�{�[�h����̎�
		if (cntl == SceneManager::CNTL::KEY) {
			//�L�[�{�[�h�ƃ}�E�X���󂯕t����
			if (periType == PERIPHERAL_TYPE::KEYBOARD || periType == PERIPHERAL_TYPE::MOUSE)currentFlag = true;
		}
		//�p�b�h�̎�
		else if (cntl == SceneManager::CNTL::PAD) {
			//PAD�ƃA�i���O���󂯕t����
			if (periType == PERIPHERAL_TYPE::GAMEPAD || periType == PERIPHERAL_TYPE::X_ANALOG) currentFlag = true;
		}
		//NONE�̂Ƃ�
		else {
			//���̃��[�Ղɓ����Ă��鎞�_�œ��͂��������Ƃ�������
			currentFlag = true;
		}

		//���ʂ��K�v�Ȃ��Ȃ�ⓚ���p��OK
		if (!_isDistinguish) {
			currentFlag = true;
		}
	}
	return currentFlag;
}


bool InputManager::IsTrigerrDown(const std::string& _eventCode, bool _isDistinguish)
{
	//��ɗv�f���Ȃ��ꍇ�̗\�h�����͂�
// �������Ȃ������Ƃ�����Ԃ���肽������
//containd()=�������L�[�ƂȂ�v�f���Ȃ��Ƃ�
	if (!currentInptuPeri_.contains(_eventCode)) {
		//�Q�Ƃł��Ȃ��̂�
		return false;
	}

	//[]�̌`�Œ��g�����悤�Ƃ���Ə���ɒ��g����̃L�[�̏ꏊ����������Ă��܂�
	//�Ȃ̂�map�^��at()�֐��̓L�[�����ł���ǂݎ���p���g�p���邱�ƂŒ��g��ς����ɎQ�Ƃ��\�ɂȂ邂
	return IsInputRecord(_eventCode, INPUT_RECORD::CURRENT, _isDistinguish) && !IsInputRecord(_eventCode, INPUT_RECORD::LAST, _isDistinguish);
}

bool InputManager::IsTrigerrUp(const std::string& _eventCode, bool _isDistinguish)
{
	//��ɗv�f���Ȃ��ꍇ�̗\�h�����͂�
	// �������Ȃ������Ƃ�����Ԃ���肽������
	//containd()=�������L�[�ƂȂ�v�f���Ȃ��Ƃ�
	if (!currentInptuPeri_.contains(_eventCode)) {
		//�Q�Ƃł��Ȃ��̂�
		return false;
	}

	//[]�̌`�Œ��g�����悤�Ƃ���Ə���ɒ��g����̃L�[�̏ꏊ����������Ă��܂�
	//�Ȃ̂�map�^��at()�֐��̓L�[�����ł���ǂݎ���p���g�p���邱�ƂŒ��g��ς����ɎQ�Ƃ��\�ɂȂ邂
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


