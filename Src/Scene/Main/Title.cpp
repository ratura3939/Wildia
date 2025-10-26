#include<DxLib.h>
#include<string>
#include<memory>
#include"../../Application.h"
#include"../../Utility/Utility.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"Game.h"

#include "Title.h"


//���[�J���萔
namespace {
#pragma region UI�o�^��
	std::string UI_EXIT_STR = "exit";
	std::string UI_ALLOW_STR = "allow";
	std::string UI_LOGO_STR = "Logo";
	std::string UI_SHADOWLOGO_STR = "shadowLogo";
	std::string UI_START_STR = "startBtn";
	std::string UI_CLICK_STR = "click";
#pragma endregion
	
#pragma region �摜�����p
	//�^�C�g�����S	
	const float LOGO_ALL_DIFF = 450.0f;			//�ʒu����
	const float LOGO__ALL_MARGIN_Y = 200.0f;	//����̌��Ԓ���
	const float LOGO_NOMAL_EXTEND = 0.8f;		//�g�嗦
	const float LOGO_SHADOW_EXTEND = 0.4f;		//�g�嗦(�e�G)
	const float LOGO_SHADOW_MARGIN_X = 40.0f;	//�g�嗦(�e�G)

	//�X�^�[�g�{�^��
	const float START_BTN_MARGIN_Y = 50.0f;		//�ʒu����
	const float START_BTN_EXTEND_MAX = 0.7f;	//�g�嗦(���)
	const float START_BTN_EXTEND_MIN = 0.55f;	//�g�嗦(����)
	const float START_BTN_EXTEND_ACC = 0.01f;	//�g�嗦(���Z)

	//�u���肵�Ă��������v
	const float ENTER_STR_EXTEND = 0.2f;		//�g�嗦
#pragma endregion

#pragma region �R���g���[���[�I����
	//�R���g���[���[�A�C�R��
	const int DEVICE_SIZE = 300;			//�T�C�Y(�����`)

	//���
	const int ALLOW_ICON_SIZE_X = 199;	//X�T�C�Y
	const int ALLOW_ICON_SIZE_Y = 288;	//Y�T�C�Y
	//���̉��o
	const float JUMP_POW_MAX = 0;	//������(���)
	const float JUMP_POW_MIN = -60;	//������(����)
	const float JUMP_ACC = -5;		//��󓮂��p

	//Exit
	const int EXIT_ICON_SIZE_X = 180;		//X�T�C�Y
	const int EXIT_ICON_SIZE_Y = 243;		//Y�T�C�Y
	const float EXIT_EXTEND_MAX = 1.5f;		//�g�嗦(���)
	const float EXIT_EXTEND_MIN = 0.6f;		//�g�嗦(����)
	const float EXIT_EXTEND_ACC = 0.05f;	//�g�嗦(���Z)

	//���Ԓ����p
	const float MARGIN_SIZE = 30.0f;			//���Ԃ̑傫��
	const float EXTEND_IMG = 1.5f;		//�摜�g�嗦
#pragma endregion
}

Title::Title(void)
{
	isSelectDevice_ = false;
	selectDevice_[static_cast<int>(DEVICE::KEY)] = true;
	selectDevice_[static_cast<int>(DEVICE::PAD)] = false;
	selectExit_ = false;
	allowPos_[0] = { 0.0f,0.0f,0.0f };
	allowPos_[1] = { 0.0f,0.0f,0.0f };

	deviceImgs_[0] = -1;
	deviceImgs_[1] = -1;

	update_ = &Title::NomalUpdate;
}

Title::~Title(void)
{
}

void Title::Init(void)
{
	//���\�[�X����
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::TITLE);

	//�R���g���[���[���Ή�
	SceneManager::GetInstance().SetController(SceneManager::CNTL::NONE);

	// �^�C�g�����S
	deviceImgs_[static_cast<int>(DEVICE::KEY)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEYBOARD_IMG).handleId_;
	deviceImgs_[static_cast<int>(DEVICE::PAD)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_IMG).handleId_;

	//UI������
	InitUI();

	//�����_�[�ƃ}�e���A��(�w�i�̈����L�΂��p)
	material_ = std::make_unique<PixelMaterial>("NomalTexPS.cso", 0);
	material_->AddTextureBuf(rsM.Load(ResourceManager::SRC::TITLE_BACK_BTN).handleId_);

	render_ = std::make_unique<PixelRenderer>(*material_);
	render_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });

	//���֌W������
	InitSound();

	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos({ 0.0f,0.0f,0.0f });
}

void Title::InitUI(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	//�`��ʒu
	float screenHX = static_cast<float>(Application::SCREEN_SIZE_X / 2);
	float screenHY = static_cast<float>(Application::SCREEN_SIZE_Y / 2);
	//�߂�A�C�R���`��ʒu
	VECTOR exitPos = { screenHX, screenHY + ((static_cast<float>(DEVICE_SIZE * EXTEND_IMG) / 2.0f) + (static_cast<float>(ALLOW_ICON_SIZE_Y) / 2.0f)) ,0.0f };
	VECTOR alPos = {};

	//���ʒu
	alPos.x= screenHX - (DEVICE_SIZE * EXTEND_IMG / 2) - (MARGIN_SIZE * EXTEND_IMG);
	alPos.y = screenHY - ((DEVICE_SIZE * EXTEND_IMG / 2) + (ALLOW_ICON_SIZE_Y / 2));
	alPos.z = 0.0f;
	//�L�[�{�[�h���̖��
	allowPos_[static_cast<int>(DEVICE::KEY)] = alPos;

	alPos.x = screenHX + (DEVICE_SIZE * EXTEND_IMG / 2) + (MARGIN_SIZE * EXTEND_IMG);
	//PAD���̖��
	allowPos_[static_cast<int>(DEVICE::PAD)] = alPos;

	//���A�C�R��
	uiM.Add(UI_ALLOW_STR, rsM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::UP_DOWN, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ALLOW_STR, allowPos_[static_cast<int>(DEVICE::KEY)]);
	uiM.SetUIDirectionPram(UI_ALLOW_STR, UI_GROUP::MOVE, JUMP_ACC, JUMP_POW_MAX, JUMP_POW_MIN);

	//�߂�A�C�R��
	uiM.Add(UI_EXIT_STR, rsM.Load(ResourceManager::SRC::EXIT_IMG).handleId_,UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_EXIT_STR, exitPos);	
	uiM.SetUIDirectionPram(UI_EXIT_STR, UI_GROUP::ZOOM, EXIT_EXTEND_ACC, EXIT_EXTEND_MAX, EXIT_EXTEND_MIN);

	//�^�C�g�����S
	uiM.Add(UI_LOGO_STR, rsM.Load(ResourceManager::SRC::TITLE_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_LOGO_STR, VECTOR(screenHX - LOGO_ALL_DIFF, screenHY - LOGO_ALL_DIFF + LOGO__ALL_MARGIN_Y, 0.0f), LOGO_NOMAL_EXTEND);	

	//�^�C�g�����S(�e�G)
	uiM.Add(UI_SHADOWLOGO_STR, rsM.Load(ResourceManager::SRC::SHADOW_LOGO).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_SHADOWLOGO_STR, VECTOR(screenHX + LOGO_SHADOW_MARGIN_X, screenHY - LOGO_ALL_DIFF + LOGO__ALL_MARGIN_Y, 0.0f), LOGO_SHADOW_EXTEND);

	//�X�^�[�g�{�^��
	uiM.Add(UI_START_STR, rsM.Load(ResourceManager::SRC::START_GAME_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_START_STR, VECTOR(screenHX, screenHY + START_BTN_MARGIN_Y, 0.0f), START_BTN_EXTEND_MAX);
	uiM.SetUIDirectionPram(UI_START_STR, UI_GROUP::ZOOM, START_BTN_EXTEND_ACC, START_BTN_EXTEND_MAX, START_BTN_EXTEND_MIN);


	//�N���b�N���ĉ�����
	uiM.Add(UI_CLICK_STR, rsM.Load(ResourceManager::SRC::CLICK_STR_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_CLICK_STR, VECTOR(screenHX, screenHY + DEVICE_SIZE, 0.0f), ENTER_STR_EXTEND);
}

void Title::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::TITLE_BGM).handleId_);
	//BGM�Đ�
	sndM.Play("NomalBgm");

	//SE
	//�J�[�\���ړ���
	sndM.Add(SoundManager::TYPE::SE, "Cursur",
		rsM.Load(ResourceManager::SRC::MOVE_CURSUR_SE).handleId_);
	//����
	sndM.Add(SoundManager::TYPE::SE, "Enter",
		rsM.Load(ResourceManager::SRC::ENTER_CNTL_SE).handleId_);
	//�L�����Z��
	sndM.Add(SoundManager::TYPE::SE, "Cancel",
		rsM.Load(ResourceManager::SRC::CANCEL_SE).handleId_);

}

void Title::InitEffect(void)
{
}

void Title::Update(void)
{

	(this->*update_)();
	
}

void Title::Draw(void)
{
	auto& uiM = UIManager2d::GetInstance();

	//�w�i�`��
	render_->Draw();

	//UI�`��
	uiM.Draw({ UI_SHADOWLOGO_STR,UI_LOGO_STR,UI_START_STR,UI_CLICK_STR });

	//�R���g���[���I��
	if (isSelectDevice_) {
		//�d�˂ăf�o�C�X�̕`��
		DrawDevice();
	}
}

void Title::Release(void)
{
}

void Title::Reset(void)
{
}

void Title::NomalUpdate(void)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("action"))
	{
		//�R���g���[���[�I����
		isSelectDevice_ = true;
		update_ = &Title::SelectDeviceUpdate;
		SoundManager::GetInstance().Play("Enter");
	}
	//�����̂���UI�̍X�V
	UIManager2d::GetInstance().Update(UI_START_STR);
}

void Title::SelectDeviceUpdate(void)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();

	//����
	if (ins.IsTrigerrDown("action"))
	{
		//�u�߂�v�Ȃ�
		if (selectExit_) {
			//�^�C�g���֖߂�
			isSelectDevice_ = false;
			update_ = &Title::NomalUpdate;
			SoundManager::GetInstance().Play("Cancel");
		}
		else {
			//������ʂ�Ƃ��͕K���ǂ��炩�I������Ă���Ƃ�
		
			SoundManager& sndM = SoundManager::GetInstance();
			SceneManager& scM = SceneManager::GetInstance();

			//�L�[�{�[�h���I������Ă�����
			if (selectDevice_[static_cast<int>(DEVICE::KEY)]) {
				//�L�[�{�[�h����ɐݒ�
				scM.SetController(SceneManager::CNTL::KEY);
			}
			else {
				//PAD����ɐݒ�
				scM.SetController(SceneManager::CNTL::PAD);
			}

			//�V�[���J��
			sndM.Stop("NomalBgm");
			sndM.Play("Enter");
			scM.ChangeScene(std::make_shared<Game>());
		}
	}

	//�I���֌W
	if (ins.IsTrigerrDown("right")) {
		//�J�[�\�����p�b�h��
		SetSelectDevice(DEVICE::PAD);
	}
	else if(ins.IsTrigerrDown("left")) {
		//�J�[�\�����L�[��
		SetSelectDevice(DEVICE::KEY);
	}
	else if (ins.IsTrigerrDown("down")) {
		//�J�[�\�����u�߂�v��
		SetSelectDevice(DEVICE::MAX);
	}

	//����͂́u�߂�A�C�R���v�ɃJ�[�\��������Ƃ��������肵�Ȃ�
	if(ins.IsTrigerrDown("up") && selectExit_) {
		SetSelectDevice(DEVICE::KEY);
	}

	//�u�߂�v�I��
	if (selectExit_) {
		//�߂�A�C�R���X�V
		uiM.Update(UI_EXIT_STR);
	}
	else {
		//���A�C�R���X�V
		uiM.Update(UI_ALLOW_STR);
	}

}

void Title::SetSelectDevice(const DEVICE _device)
{
	//KEY��PAD�̂Ƃ�
	if (_device != DEVICE::MAX) {
		int selectDeviceIdx = static_cast<int>(_device);	//�I�����Ă������(�z��̎w���p)
		int notSelectDeviceIdx = abs(selectDeviceIdx - 1);	//�I�����Ă��Ȃ�����(�z��̎w���p)/�v�f����݂̂Ȃ̂ŕK��0�E�P���o�͂����悤��

		//�ݒ�
		selectDevice_[selectDeviceIdx] = true;
		selectDevice_[notSelectDeviceIdx] = false;

		//UI�ʒu�ݒ�
		UIManager2d::GetInstance().SetPos(UI_ALLOW_STR, allowPos_[selectDeviceIdx]);

		//�u�߂�v�ł͂Ȃ�
		selectExit_ = false;
	}
	else {
		//�S����
		selectDevice_[static_cast<int>(DEVICE::KEY)] = false;
		selectDevice_[static_cast<int>(DEVICE::PAD)] = false;

		//�u�߂�v�ł���
		selectExit_ = true;
	}
	//�J�[�\���ړ���
	SoundManager::GetInstance().Play("Cursur");
	
}


void Title::DrawDevice(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	float screenHX = static_cast<float>(Application::SCREEN_SIZE_X / 2);
	float screenHY = static_cast<float>(Application::SCREEN_SIZE_Y / 2);

	//��������w�i��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�����͒�������
	float drawX = 0.0f, drawY = 0.0f;
	drawY = screenHY;

	//�L�[�{�[�h
	drawX = screenHX - (static_cast<float>(DEVICE_SIZE) * EXTEND_IMG / 2.0f) - (MARGIN_SIZE * EXTEND_IMG);
	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::KEY)], true);

	//PAD
	drawX = screenHX + (DEVICE_SIZE * EXTEND_IMG / 2.0f) + (MARGIN_SIZE * EXTEND_IMG);
	DrawRotaGraph(drawX, drawY, EXTEND_IMG, 0.0f, deviceImgs_[static_cast<int>(DEVICE::PAD)], true);

	//�u�߂�v���I�΂�Ă��Ȃ��Ƃ�
	if (!selectExit_) {
		//���̕`��
		uiM.Draw(UI_ALLOW_STR);
	}

	//�߂�A�C�R��
	uiM.Draw(UI_EXIT_STR);
}
