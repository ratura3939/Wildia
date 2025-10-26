#include<string>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"

#include "SwitchControllerScene.h"

//���[�J���萔
namespace {
#pragma region UI�o�^��
	const std::string SWITCH_IMG = "switchImg";
	const std::string SWITCH_BTN = "switchBtn";
	const std::string BACK_BTN = "BackBtn_swScene";
	const std::string FINISH_SW = "Finish_swScene";
#pragma endregion

#pragma region �摜�����p
	//�u�؂芷�������v
	const float STRING_EX = 1.4f;	//�T�C�Y
	const int IMG_SW_DIFF_Y = 100;	//Y�ʒu�����p
#pragma endregion

	const int COOL_TIME_MAX = 100;	//�N�[���^�C�����
}

SwitchControllerScene::SwitchControllerScene(void)
{
	coolTimeCnt_ = 0;
	isSwCoolTime_ = false;
	isSwBtn_ = false;
}

SwitchControllerScene::~SwitchControllerScene(void)
{
}

void SwitchControllerScene::Init(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR screenSize = { static_cast<float>(Application::SCREEN_SIZE_X),static_cast<float>(Application::SCREEN_SIZE_Y),0.0f };

	//�؂�ւ��m�F�摜
	uiM.Add(SWITCH_IMG, rsM.Load(ResourceManager::SRC::CONFIRM_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_IMG, VECTOR{ screenSize.x / 2.0f,screenSize.y / 2.0f - IMG_SW_DIFF_Y,0.0f }, IMG_EX_SB_SCENE);

	//�؂�ւ���{�^��
	uiM.Add(SWITCH_BTN, rsM.Load(ResourceManager::SRC::SWITCH_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_BTN, VECTOR{ screenSize.x / 2.0f,screenSize.y - BTN_DIFF_Y_SB_SCENE,0.0f }, BTN_EX_MAX_SB_SCENE);
	uiM.SetUIDirectionPram(SWITCH_BTN, UI_GROUP::ZOOM, BTN_EX_ACC_SB_SCENE, BTN_EX_MAX_SB_SCENE, BTN_EX_MIN_SB_SCENE);

	//�߂�{�^��
	uiM.Add(BACK_BTN, rsM.Load(ResourceManager::SRC::BACK_BTN).handleId_, UI_DIREC::ZOOM_INOUT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_BTN, VECTOR{ BTN_DIFF_X_SB_SCENE,screenSize.y - BTN_DIFF_Y_SB_SCENE,0.0f }, BTN_EX_MAX_SB_SCENE);
	uiM.SetUIDirectionPram(BACK_BTN, UI_GROUP::ZOOM, BTN_EX_ACC_SB_SCENE, BTN_EX_MAX_SB_SCENE, BTN_EX_MIN_SB_SCENE);

	//�؂�ւ������ʒm
	uiM.Add(FINISH_SW, rsM.Load(ResourceManager::SRC::FINISH_SWITCH).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(FINISH_SW, VECTOR{ screenSize.x - BTN_DIFF_X_SB_SCENE,Application::SCREEN_SIZE_Y - BTN_DIFF_Y_SB_SCENE,0.0f }, STRING_EX);
}

void SwitchControllerScene::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//�؂�ւ���
	if (isSwCoolTime_) {
		//�J�E���^���Z
		coolTimeCnt_++;

		if (coolTimeCnt_ > COOL_TIME_MAX) {
			//�؂芷���N�[���^�C������
			isSwCoolTime_ = false;
			coolTimeCnt_ = 0;
		}
	}

	//����̓���
	if (inpM.IsTrigerrDown("action",false)) {
		if (isSwBtn_) {
			//�؂芷�������E�N�[���^�C������
			SceneManager::GetInstance().SwitchController();
			isSwCoolTime_ = true;
		}
		else {
			//�V�[�����|�b�v
			SceneManager::GetInstance().PopScene();
			return;
		}
	}

	//�ړ�����
	if (inpM.IsTrigerrDown("right",false)) {
		//�u�؂芷���v�{�^���̈ʒu�ɂȂ邽��true��
		isSwBtn_ = true;
	}
	else if (inpM.IsTrigerrDown("left",false)) {
		//�u�߂�v�{�^���̈ʒu�ɂȂ邽��false��
		isSwBtn_ = false;
	}

	//�{�^���g��E�k��(���ݑI�����Ă���{�^���̂�)
	if (isSwBtn_) {
		uiM.Update(SWITCH_BTN);
	}
	else {
		uiM.Update(BACK_BTN);
	}
}

void SwitchControllerScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//�ǉ��V�[���Ȃ̂ł�������Ɣw�i�ł���Q�[���V�[�����f��悤�ɂ���B
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//UI�`��
	uiM.Draw({ SWITCH_IMG,SWITCH_BTN,BACK_BTN });

	//�N�[���^�C����
	if (isSwCoolTime_) {
		//�u�������܂����v�Ƃ������͕\��
		uiM.Draw(FINISH_SW);
	}
}

void SwitchControllerScene::Release(void)
{
	//UI�̍폜
	UIManager2d::GetInstance().DeleteUI({ SWITCH_IMG,SWITCH_BTN,BACK_BTN });
}

void SwitchControllerScene::Reset(void)
{
}

void SwitchControllerScene::InitSound(void)
{
}

void SwitchControllerScene::InitEffect(void)
{
}
