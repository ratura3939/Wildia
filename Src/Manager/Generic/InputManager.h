#pragma once
#include<unordered_map>	//�Ή��\�̂���
#include<vector>		//�f�o�C�X��ǉ����邽��
#include<string>		//�Ή��L�[�̂���
#include<map>
#include<functional>
#include <Dxlib.h>
#include "../../Common/Vector2.h"
#include"SceneManager.h"

class InputManager
{

public:
	static constexpr int ANALOG_STHICK_THRESHOLD = 10000;	//�X�e�B�b�N臒l
	static constexpr int ANALOG_TRIGGER_THRESHOLD = 128;	//�g���K�[臒l
	static constexpr int KEY_ALL = 256;	//�L�[�푍��

	/// <summary>
	/// ���Ӌ@����
	/// </summary>
	enum class PERIPHERAL_TYPE {
		//�L�[�}�E����
		KEYBOARD,
		MOUSE,
		//PAD����
		GAMEPAD,
		X_ANALOG,
		MAX
	};

	/// <summary>
	/// �A�i���O���͎��
	/// </summary>
	enum class ANALOG_INPUT_TYPE {
		LS_UP,		//���X�e�B�b�N�̏�
		LS_DOWN,	//���X�e�B�b�N�̉�
		LS_RIGHT,	//���X�e�B�b�N�̉E
		LS_LEFT,	//���X�e�B�b�N�̍�
		LT,			//���g���K�[
		RS_UP,		//�E�X�e�B�b�N�̏�
		RS_DOWN,	//�E�X�e�B�b�N�̉�
		RS_RIGHT,	//�E�X�e�B�b�N�̉E
		RS_LEFT,	//�E�X�e�B�b�N�̍�
		RT,			//�E�g���K�[
		end
	};

	enum class MOUSE_INPUT {
		L_CLICK,//���N���b�N
		R_CLICK,//�E�N���b�N
		M_CLICK,//�z�C�[���N���b�N
		UP,		//��ړ�
		DOWN,	//���ړ�
		LEFT,	//���ړ�
		RIGHT,	//�E�ړ�
		MAX
	};

	enum class INPUT_RECORD {
		CURRENT,
		LAST
	};


	// �Q�[���R���g���[���[�̔F���ԍ�
	// DxLib�萔�ADX_INPUT_PAD1���ɑΉ�
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// �L�[���͂ƃp�b�h�P����
		PAD1,				// �p�b�h�P����
		PAD2,				// �p�b�h�Q����
		PAD3,				// �p�b�h�R����
		PAD4,				// �p�b�h�S����
		INPUT_KEY = 4096,	// �L�[����
		MAX
	};

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// ���\�[�X�̔j��
	void Destroy(void);

private:
	//�L�[�Ή�������
	void ResetInput(void);
	//�A�i���O�L�[�̓��͔��ʂ̊֐���`
	void AnalogInputFuncInit(void);

	//�}�E�X�z�C�[���̓��͔��ʂ̊֐���`
	void MouseInputFuncInit(void);

	//�R�[�h�̌��ݖ��͂P�t���[���O�̓��͂�n��
	const bool IsInputRecord(const std::string& _eventCode,const INPUT_RECORD& _record, const bool _isDistinguish);

public:
	/// <summary>
	/// �L�[�̃_�E���g���K
	/// </summary>
	/// <param name="_eventCode">�o�^��</param>
	/// <returns></returns>
	bool IsTrigerrDown(const std::string& _eventCode, bool _isDistinguish = true);

	/// <summary>
	/// �L�[�̃A�b�v�g���K
	/// </summary>
	/// <param name="_eventCode">�o�^��</param>
	/// <returns></returns>
	bool IsTrigerrUp(const std::string& _eventCode, bool _isDistinguish = true);

	bool IsPressed(const std::string& _eventCode, bool _isDistinguish = true);

private:
	static InputManager* instance_;


	/// <summary>
	/// ���͕R�Â�(�@��E�Ή��R�[�h)
	/// </summary>
	struct InputCode
	{
		PERIPHERAL_TYPE type;//���Ӌ@��
		uint32_t code;		//���̓R�[�h(�ėp)
	};

	//<�o�^���A�Ή��L�[>(�L�[�{�[�h�EPAD�ɑΉ�)
	using InputTable_t = std::unordered_map<std::string, std::vector<InputCode>>;
	InputTable_t inputTable_;	//�C�x���g�Ɠ��͂̑Ή��\

	std::vector<std::string>inputListForDisplay_;	//�L�[�R���t�B�O�p(�O����������)

	//�X�e�B�b�N�֌W�͏����ʎ�Ȃ̂ł�����ň���(��{�I��R�X�e�B�b�N��g���K�[�p)
	using AnalogInputTable_t = std::unordered_map<ANALOG_INPUT_TYPE, std::function<bool(const XINPUT_STATE&)>>;
	AnalogInputTable_t analpgInputTable_;

	//<�o�^��,�������>
	using InputData_t = std::unordered_map<std::string, bool>;
	//InputData_t currentInput_;	//�C�x���g�ɑΉ�����{�^����������Ă��邩
	//InputData_t lastInput_;		//�C�x���g�ɑΉ�����{�^����������Ă��邩(�P�t���[���O)

	//�����ꂽ�Ƃ����ŉ����ꂽ�����o���Ă���
	using InputPeriTypeData_t = std::unordered_map<std::string, std::vector<PERIPHERAL_TYPE>>;
	InputPeriTypeData_t currentInptuPeri_;
	InputPeriTypeData_t lastInptuPeri_;


	//�}�E�X�z�C�[���Ɋւ���
	int mouseState_;		//�}�E�X�̓��͏��
	Vector2 mousePos_;		//�}�E�X�ʒu
	Vector2 centerMousePos_;	//�}�E�X�ʒu(�P�t���[���O)

	using MouseInputTable_t = std::unordered_map<MOUSE_INPUT, std::function<bool(void)>>;
	MouseInputTable_t mouseInputTable_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	InputManager(void);
	InputManager(const InputManager& manager) = default;
	~InputManager(void);
};