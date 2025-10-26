#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Scene/Main/Title.h"
#include "../../Scene/Sub/KeyConfigScene.h"
#include "../../Scene/Sub/SwitchControllerScene.h"
#include"../../Utility/Utility.h"
#include "PauseScene.h"

//���[�J���萔
namespace {
#pragma region UI�o�^��
	std::string BACK_TITLE_BTN = "BackTitle_PauseScene";
	std::string BACK_GAME_BTN = "BackGame_PauseScene";
	std::string CHECK_CONFIG_BTN = "Config_PauseScene";
	std::string SWITCH_OPE_BTN = "SwitchCntl_PauseScene";
	std::string RIGHT_ARROW = "Arrow_PauseScene";
#pragma endregion

#pragma region �摜�����p
	const float BTN_SIZE = 256.0f;			//���摜�傫��
	const float BTN_DRAW_SIZE = 0.6f;		//�{�^��(�Q�[���ɖ߂�E�^�C�g���ɖ߂�)��`�悷��Ƃ��̑傫���{��
	const float BIG_BTN_DRAW_SIZE = 0.5f;	//�{�^��(������@�E����؂�ւ�)��`�悷��Ƃ��̑傫���{��

	const float ARRW_DIFF_X = -300.0f;		//���̒����`��ʒu����̍����iX���j
	const float ARROW_LOCAL_ROT = -270.0f;	//���̉摜��]�p
	const float ARROW_ACC = -3.0f;			//���̈ړ���
	const float ARROW_MOVE_MAX = 0;			//�ړ���(���)
	const float ARROW_MOVE_MIN = -60;		//�ړ���(����)
#pragma endregion

	const int MENU_LIST_NONE_DIFFER = 1;	//�񋓂�NONE�������Ă���̂ł���̍����p
}

PauseScene::PauseScene(void)
{
	drawBtnList_ = {};
	selectIdx_ = 0;
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);

	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	VECTOR screenSize = { static_cast<float>(Application::SCREEN_SIZE_X),static_cast<float>(Application::SCREEN_SIZE_Y),0.0f };

	//�Q�[���ɖ߂�
	uiM.Add(BACK_GAME_BTN, resM.Load(ResourceManager::SRC::BACK_GAME_BTN).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_GAME_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) - BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//������@
	uiM.Add(CHECK_CONFIG_BTN, resM.Load(ResourceManager::SRC::CHECK_CONFIG_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(CHECK_CONFIG_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) - BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//����؂�ւ�
	uiM.Add(SWITCH_OPE_BTN, resM.Load(ResourceManager::SRC::SWITCH_OPERATOR_IMG).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SWITCH_OPE_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) + BTN_SIZE / 3.0f,0.0f }, BIG_BTN_DRAW_SIZE);

	//�^�C�g���ɖ߂�
	uiM.Add(BACK_TITLE_BTN, resM.Load(ResourceManager::SRC::STOP_GAME_BTN).handleId_, UI_DIREC::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(BACK_TITLE_BTN, { screenSize.x / 2.0f,(screenSize.y / 2.0f) + BTN_SIZE,0.0f }, BTN_DRAW_SIZE);

	//���X�g����
	drawBtnList_ = { BACK_GAME_BTN, CHECK_CONFIG_BTN, SWITCH_OPE_BTN, BACK_TITLE_BTN };

	//���
	uiM.Add(RIGHT_ARROW, resM.Load(ResourceManager::SRC::ARROW_DOWN_IMG).handleId_, UI_DIREC::LEFT_RIGHT, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(RIGHT_ARROW, GetDrawPosOfArrow(), BTN_DRAW_SIZE, ARROW_LOCAL_ROT);
	uiM.SetUIDirectionPram(RIGHT_ARROW, UI_GROUP::MOVE, ARROW_ACC, ARROW_MOVE_MAX, ARROW_MOVE_MIN);
	
	//���o������
	InitSound();
	InitEffect();
}

void PauseScene::InitSound(void)
{
	//�����ǉ�
}

void PauseScene::InitEffect(void)
{
	//�����ǉ�
}

void PauseScene::Update(void)
{
	//���͎�t
	InputUser();

	//UI�X�V
	UIManager2d::GetInstance().Update(RIGHT_ARROW);
}

void PauseScene::InputUser(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();

	//�|�[�Y�{�^���������ꂽ��
	if (inpM.IsTrigerrDown("pause")) {
		//�V�[���ړ�(1�O�̃V�[�����Q�[���V�[���ɖ߂�)
		scM.PopScene();
	}

	//�������
	if (inpM.IsTrigerrDown("action",false)) {

		//���ʒu���珈����I��
		switch (static_cast<MENU_ITEM>(selectIdx_ + MENU_LIST_NONE_DIFFER)) {
		case MENU_ITEM::BACK_GAME:
			//�V�[�����|�b�v
			scM.PopScene();
			break;

		case MENU_ITEM::CONFIG:
			//������@�V�[���̒ǉ�
			scM.PushScene(std::make_shared<KeyConfigScene>());
			break;

		case MENU_ITEM::SWITCH_CNTL:
			//����؂芷���V�[���̒ǉ�
			scM.PushScene(std::make_shared<SwitchControllerScene>());
			break;

		case MENU_ITEM::BACK_TITLE:
			//�^�C�g���V�[���ɕύX
			scM.ChangeScene(std::make_shared<Title>());
			break;
		}
	}

	//�����
	if (inpM.IsTrigerrDown("up",false)) {
		//Idx�̌����B�͈͊O�ɂȂ�Ȃ��悤����
		selectIdx_--;
		selectIdx_ = (selectIdx_ + drawBtnList_.size()) % static_cast<int>(drawBtnList_.size());

		//���̕`��ʒu�ݒ�
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetDrawPosOfArrow());
	}
	//������
	else if (inpM.IsTrigerrDown("down",false)) {
		//Idx�̑����B�͈͊O�ɂȂ�Ȃ��悤����
		selectIdx_++;
		selectIdx_ = selectIdx_ % static_cast<int>(drawBtnList_.size());

		//���̕`��ʒu�ݒ�
		UIManager2d::GetInstance().SetPos(RIGHT_ARROW, GetDrawPosOfArrow());
	}
}

void PauseScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//�ǉ��V�[���Ȃ̂ł�������Ɣw�i�ł���Q�[���V�[�����f��悤�ɂ���B
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�{�^���̕`��
	uiM.Draw(drawBtnList_);

	//�����̕`��
	uiM.Draw(RIGHT_ARROW);
}

void PauseScene::Release(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void PauseScene::Reset(void)
{
	//�|�[�Y�V�[���ł�BGM�𗬂��Ȃ�
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 0);	//�O�V�[���ŗ���Ă���BGM�̉��ʂ��O��
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);
}

const VECTOR PauseScene::GetDrawPosOfArrow(void) const
{
	//�I������Ă���{�^���̈ʒu
	VECTOR arrowPos = UIManager2d::GetInstance().GetDrawPos(drawBtnList_[selectIdx_]);
	//�`��ʒu�̓{�^���̍���(�����𑫂�)
	arrowPos.x += ARRW_DIFF_X;
	return arrowPos;
}
