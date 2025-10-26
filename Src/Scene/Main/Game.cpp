#include<EffekseerForDXLib.h>
#include"../../Application.h"
#include"../../Manager/GameSystem/PlayerManager.h"
#include"../../Manager/GameSystem/EnemyManager.h"
#include"../../Manager/GameSystem/AttackManager.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/EffectManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "../../Scene/Main/GameOver.h"
#include "../../Scene/Main/GameClear.h"
#include "../../Scene/Sub/PauseScene.h"
#include"../../Object/Stage/Stage.h"
#include"../../Utility/Utility.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include "Game.h"

//���[�J���萔
namespace {
	constexpr VECTOR CAMERA_START_1 = { 600.0f,200.0f,0.0f };	//�J�������o�J�n�ʒu
	constexpr VECTOR CAMERA_GOAL_1 = { 600.0f,1000.0f,0.0f };	//�J�������o�ڕW�ʒu���̇@
	constexpr VECTOR CAMERA_GOAL_2 = { 0.0f,800.0f,600.0f };	//�J�������o�ڕW�ʒu���̇A
	constexpr float ALLOWABLE_DISTANCE = 10.0f;		//�J�����̈ړ�������������Ζڂɂ��邽�߂�
	constexpr int BOSS_IDX = 0;		//�{�X�̔z��ԍ�(�{�X�P�̂̂��ߕK��0)


	const int LIMIT_SLOW = 200;					//�X���[���o����
	const int BGM_VOL_MAX = 100;					//BGM���ʍő�l
	const int BGM_VOL_ACC = 1;					//BGM�؂芷���X�s�[�h
	const float NOMAL_SPEED_PERCENT = 100.0f;	//�ʏ�̊���
	const float SLOW_SPEED_PERCENT = 25.0f;		//�X���[�̊���(�ʏ펞���甼���̑��x�ɂ���)
		  
	const int WARNING_DIRECTION_TIME = 150;		//WARNING�x������
	const int CAMERA_SHAKE_NUM = 3;				//�J�������o�ɂ�����U����
	const int CAMERA_SHAKE_COOL_TIME = 40;		//�U���̃N�[���^�C��
	

	const std::string MENU_BTN = "menuBtn";
	const std::string WARNING_STR_IMG = "WarningImg";
	const float BTN_EX = 0.6f;
	const int BTN_DIFF_X = 300;
	const int BTN_DIFF_Y = 100;
}

Game::Game(void)
{
	isSlowEffect_ = false;
	slowCnt_ = -1;
	nextBgmVol_ = 0;
	switchBgm_ = false;

	cameraMoveStartPos_ = CAMERA_START_1;
	cameraMoveGoalPos_[0] = CAMERA_GOAL_1;
	cameraMoveGoalPos_[1] = CAMERA_GOAL_2;
	direcState_ = BOSS_DIRECTION::NONE;
	cameraShakeCollTimeCnt_ = 0;
	stayCameraShake_ = false;

	isDrawPostEffect_ = false;
	direcCnt_ = 0;

	update_ = &Game::GameUpdate;
	drawPostEffect_ = &Game::DrawScanLine;
	direcUpdate_ = &Game::DirectionPostEffect;

	blurScreen_ = -1;
	dodgeScreen_ = -1;
	scanLineScreen_ = -1;
}

Game::~Game(void)
{
	DeleteGraph(scanLineScreen_);
	DeleteGraph(blurScreen_);
}

void Game::Init(void)
{
	//���\�[�X����
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::GAME);

	//����	//�G
	enemy_ = std::make_unique<EnemyManager>(*this);
	enemy_->Init();

	//�v���C���[
	player_ = std::make_unique<PlayerManager>(*this,*enemy_);
	player_->Init();

	//�U��
	atkMng_ = std::make_unique<AttackManager>();
	//�U���̓o�^
	AttackDataInit();

	//����
	collision_ = std::make_unique<CollisionManager>();

	//�X�e�[�W
	stage_ = std::make_unique<Stage>(false);
	stage_->Init();

	//�J�����̏����ݒ�
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//���[�h�I��
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//�Ǐ]�Ώ�
	camera.SetFocusPos(player_->GetFocusPoint());				//�����_

	//���֌W�����ݒ�
	InitSound();
	//�G�t�F�N�g�֌W������
	InitEffect();
	//�V�F�[�_�[������
	InitShader();

	auto& uiM = UIManager2d::GetInstance();

	//�uWARNING�v�摜
	uiM.Add(WARNING_STR_IMG, rsM.Load(ResourceManager::SRC::WARNING_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::FLASHING, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(WARNING_STR_IMG, VECTOR{static_cast<float>(Application::SCREEN_SIZE_X)/2.0f,static_cast<float>(Application::SCREEN_SIZE_Y) / 2.0f,0.0f });
	uiM.SetUIDirectionPram(WARNING_STR_IMG, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, 10.0f, 255.0f, 0.0f);

	//���j���[�{�^��
	uiM.Add(MENU_BTN, rsM.Load(ResourceManager::SRC::MENU_BTN).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(MENU_BTN, VECTOR{ static_cast<float>(Application::SCREEN_SIZE_X - BTN_DIFF_X),static_cast<float>(Application::SCREEN_SIZE_Y - BTN_DIFF_Y),0.0f }, BTN_EX);
}

void Game::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::GAME_BGM).handleId_);
	//�o�g��BGM
	sndM.Add(SoundManager::TYPE::BGM, "BattleBgm",
		rsM.Load(ResourceManager::SRC::BATTLE_BGM).handleId_);

	//�o�g��BGM
	sndM.Add(SoundManager::TYPE::BGM, "BossBgm",
		rsM.Load(ResourceManager::SRC::BOSS_BGM).handleId_);

	//�x����
	sndM.Add(SoundManager::TYPE::BGM, "WarningBgm",
		rsM.Load(ResourceManager::SRC::WARNING_BGM).handleId_);

	//����͕��ʂ�BGM
	sndM.Play("NomalBgm");
	nowBgmStr_ = "NomalBgm";
	switchBgmStr_ = "BattleBgm";

	//SE
	//����
	sndM.Add(SoundManager::TYPE::SE, "Walk",
		rsM.Load(ResourceManager::SRC::WALK_SE).handleId_,20);
	sndM.AdjustVolume("Walk",60);

	//����
	sndM.Add(SoundManager::TYPE::SE, "Dush",
		rsM.Load(ResourceManager::SRC::RUN_SE).handleId_,10);
	sndM.AdjustVolume("Dush", 60);

	//����U��
	sndM.Add(SoundManager::TYPE::SE, "SwingSword",
		rsM.Load(ResourceManager::SRC::SWING_SWORD_SE).handleId_);
	//���b�N�I��
	sndM.Add(SoundManager::TYPE::SE, "RockOn",
		rsM.Load(ResourceManager::SRC::LOCK_ON_SE).handleId_);
	//�v���C���[�𔭌�
	sndM.Add(SoundManager::TYPE::SE, "FindPlayer",
		rsM.Load(ResourceManager::SRC::FIND_PLAYER_SE).handleId_);
	//�_���[�W
	sndM.Add(SoundManager::TYPE::SE, "Damage",
		rsM.Load(ResourceManager::SRC::DAMAGE_SE).handleId_);

	//�{�X����
	sndM.Add(SoundManager::TYPE::SE, "Impact",
		rsM.Load(ResourceManager::SRC::BOSS_IMPACT_SE).handleId_);
	sndM.AdjustVolume("Impact", 60);

	//�U���x����
	sndM.Add(SoundManager::TYPE::SE, "Allert",
		rsM.Load(ResourceManager::SRC::ATK_ALLERT_SE).handleId_);

	//�����
	sndM.Add(SoundManager::TYPE::SE, "Dodge",
		rsM.Load(ResourceManager::SRC::DODGE_SE).handleId_);
	sndM.AdjustVolume("Dodge", 45);

	//�W���X�g�����
	sndM.Add(SoundManager::TYPE::SE, "JustDodge",
		rsM.Load(ResourceManager::SRC::JUST_DODGE_SE).handleId_);
	sndM.AdjustVolume("JustDodge", 80);

}

void Game::InitEffect(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	EffectManager& efcM = EffectManager::GetInstance();

	//��
	efcM.Add("Sword", rsM.Load(ResourceManager::SRC::SWORD_EFC).handleId_);
	//�_���[�W
	efcM.Add("Damage", rsM.Load(ResourceManager::SRC::DAMAGE_EFC).handleId_);
}

void Game::InitShader(void)
{
	//�u���[
	//PS
	blurMaterial_ = std::make_unique<PixelMaterial>("Blur.cso", 3);
	//�g�U��
	blurMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,0.0f });
	//����
	blurMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	//��ʑ傫��
	blurMaterial_->AddConstBuf({ Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,0.0f,0.0f });

	blurRender_ = std::make_unique<PixelRenderer>(*blurMaterial_);
	blurRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// �|�X�g�G�t�F�N�g�p�X�N���[��
	blurScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//�u���[(���p)
	//PS
	dodgeMaterial_ = std::make_unique<PixelMaterial>("JustDodgePS.cso", 3);
	//�g�U��
	dodgeMaterial_->AddConstBuf({ 1.0f,1.0f,1.0f,1.0f });
	//����
	dodgeMaterial_->AddConstBuf({ 0.0f,0.0f ,0.0f,0.0f });
	//���X�EY�E�����E���a
	dodgeMaterial_->AddConstBuf({ Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y ,0.02f,0.4f });

	dodgeMaterial_->SetTextureBuf(11, ResourceManager::GetInstance().Load(ResourceManager::SRC::FOCUS_IMG).handleId_);

	dodgeRender_ = std::make_unique<PixelRenderer>(*dodgeMaterial_);

	dodgeRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// �|�X�g�G�t�F�N�g�p�X�N���[��
	dodgeScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//������
	//PS
	scanLineMaterial_ = std::make_unique<PixelMaterial>("ScanLine.cso", 2);
	//�g�U��
	scanLineMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,0.0f });
	//����
	scanLineMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });

	scanLineRender_ = std::make_unique<PixelRenderer>(*scanLineMaterial_);
	scanLineRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// �|�X�g�G�t�F�N�g�p�X�N���[��
	scanLineScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void Game::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();
	InputManager& inpM = InputManager::GetInstance();

#pragma region �V�[���J��(���[��)
	//�v���C���[������ł�����
	if (!player_->IsAlive()) {
		//BGM�O�̂��ߗ�����~
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//�V�[���J��
		scM.ChangeScene(std::make_shared<GameOver>());
	}
#pragma endregion

	//�X�V
	(this->*update_)();
}

void Game::GameUpdate(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();
	Camera& camera = scM.GetCamera();
	
	//�G�����Ȃ��Ȃ�����
	if (enemy_->GetEnemys().size() <= 0) {
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//�V�[���J��
		scM.ChangeScene(std::make_shared<GameClear>());
	}

	//�|�[�Y�V�[���J��
	if (inpM.IsTrigerrDown("pause")) {
		//�V�[���ǉ�(�����)
		scM.PushScene(std::make_shared<PauseScene>());
	}

#pragma region ��b�A�v�f
	player_->Update(*atkMng_);
	//�G�̓X���[�̌��ʂ��󂯂�
	if (isSlowEffect_) {
		//�X���[���̍X�V(���̃J�E���^�̓X���[�̉e�����󂯂Ȃ�)
		slowCnt_++;
		if (slowCnt_ >= LIMIT_SLOW) {
			EndSlow();
		}
	}

	//�G
	enemy_->Update(player_->GetPos(), *atkMng_);
	//�U��
	atkMng_->Update();

	//���菈��/���̒��ŃX���[���o�����邩�ǂ���
	if (collision_->Collision(player_->GetPlayer(), enemy_->GetEnemys(), atkMng_->GetActiveAttacks())) {
		StartSlow();
	}
#pragma endregion

#pragma region BGM
	//�G�̏��(�퓬�E����ȊO)�̃g���K
	if (enemy_->IsSwitchBattleOrNomalEnemyTrg()) {
		//���Ƃ��Ɛ؂芷������������
		if (switchBgm_) {
			//�����I������
			FinishSwitchBgm();
		}

		//�؂芷���J�n
		switchBgm_ = true;
		//�؂�ւ���̍Đ�
		sndM.Play(switchBgmStr_);
	}

	//BGM�؂芷�����s��
	if (switchBgm_) {
		//���ʒ����ɉ��Z
		nextBgmVol_ += BGM_VOL_ACC;
		sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);			//����BGM�͉��ʂ�������
		sndM.AdjustVolume(nowBgmStr_, (BGM_VOL_MAX - nextBgmVol_));	//���݂�BGM�͉��ʂ�������

		//�����{�����[�����ő�l�ȏ�Ȃ�
		if (nextBgmVol_ >= BGM_VOL_MAX) {
			//���ʂ��ő�l��
			nextBgmVol_ = BGM_VOL_MAX;	
			//�I������
			FinishSwitchBgm();
		}
	}
#pragma endregion

#pragma region �J����
	//�J�����̐ݒ�
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//�Ǐ]�Ώۂ̍X�V
#pragma endregion
}

void Game::DirectionUpdate(void)
{
	//�댯�̃|�X�g�G�t�F�N�g����ʗh�ꁨ�J����
	if ((this->*direcUpdate_)()) {
		//���̉��o��
		direcState_ = static_cast<BOSS_DIRECTION>(static_cast<int>(direcState_) + 1);
		direcCnt_ = 0;

		//�����I��������
		if (direcState_ == BOSS_DIRECTION::END) {
			//�J�����̒Ǐ]�Ώۂ�߂�
			Camera& camera = SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::FOLLOW);					//���[�h�I��
			camera.SetFollow(player_->GetPos(), player_->GetQua());		//�Ǐ]�Ώ�
			camera.SetFocusPos(player_->GetFocusPoint());				//�����_

			//�u���[���Ȃ���
			ChangeActionDirec(ACTION_DIRECTION::NOMAL);

			//BGM����
			SoundManager::GetInstance().Play("BossBgm");
			nowBgmStr_ = "BossBgm";
			switchBgm_ = false;

			//�X�V��ʏ��
			update_ = &Game::GameUpdate;
		}
		//��ʗh��
		else if (direcState_ == BOSS_DIRECTION::SHAKE_SCREEN) {
			//���s
			DoShake();
			//���o�̍X�V���u��ʗh��v��
			direcUpdate_ = &Game::DirectionShakeScreen;
		}
		//�J�����ړ�
		else if (direcState_ == BOSS_DIRECTION::CAMERA_MOVE) {
			//�{�X�̐���
			enemy_->CreateBoss();

			//�J�����������ړ��ɐݒ�
			auto& camera = SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::AUTO_MOVE);

			//�ꏊ�̐ݒ�(�{�X�̉����炢)
			auto bossPos = enemy_->GetPos(BOSS_IDX);
			camera.SetPos(VAdd(bossPos,cameraMoveStartPos_), bossPos);
			camera.SetGoalPos(VAdd(bossPos, cameraMoveGoalPos_[direcCnt_]));

			//���o���u�J�����ړ��ɕύX
			direcUpdate_ = &Game::DirectionCameraMove;
		}
	}
}

bool Game::DirectionPostEffect(void)
{
	//WARNING�X�V
	UIManager2d::GetInstance().Update(WARNING_STR_IMG);

	//�|�X�g�G�t�F�N�g�X�V
	scanLineMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });	//�����C���ړ��p

	//���o�J�E���^�X�V
	direcCnt_++;
	//��莞�ԉ߂�����
	if (direcCnt_ > WARNING_DIRECTION_TIME) {
		//���o�I��
		SoundManager::GetInstance().Stop("WarningBgm");	//�x�����~�߂�
		ChangeActionDirec(ACTION_DIRECTION::NOMAL);		//�|�X�g�G�t�F�N�g�I��
		return true;
	}
	//���o������
	return false;
}

bool Game::DirectionShakeScreen(void)
{
	//�J�����m�[�V�F�C�N��
	if (stayCameraShake_) {
		//�N�[���^�C������
		cameraShakeCollTimeCnt_++;

		//��莞�Ԍo�ߌ�
		if (cameraShakeCollTimeCnt_ >= CAMERA_SHAKE_COOL_TIME) {
			//�ēx�h�炷
			DoShake();
			stayCameraShake_ = false;
		}
		return false;
	}

	//�J�����V�F�C�N�I����
	if (SceneManager::GetInstance().GetCamera().IsFinishShake()) {
		//���o�J�E���^����
		direcCnt_++;

		//��萔�s������
		if (direcCnt_ >= CAMERA_SHAKE_NUM) {
			//���o�I��
			return true;
		}
		//�N�[���^�C���֌W���Z�b�g
		cameraShakeCollTimeCnt_ = 0;
		stayCameraShake_ = true;
	}
	//���o������
	return false;
}

void Game::DoShake(void)
{
	SoundManager::GetInstance().Play("Impact");	//���ʉ��Đ�
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::SHAKE);	//�h�炷
}

bool Game::DirectionCameraMove(void)
{
	//�A�j���[�V�����̂ݍX�V
	enemy_->UpdateAnim();

	//�J�������o�p
	auto& camera = SceneManager::GetInstance().GetCamera();

	//�S�[���ʒu�ɂ����玟�̃X�^�[�g�ʒu��
	auto cameraPos = camera.GetPos();
	if (Utility::MagnitudeF(VSub(camera.GetGoalPos(), cameraPos)) <= ALLOWABLE_DISTANCE) {
		//���o�J�E���^����
		direcCnt_++;

		//�ړ����o�񐔂̏���ɓ��B���Ă�����
		if (direcCnt_ >= CAMERA_DIRECTION_NUM) {
			//���o�I��
			return true;
		}
		else {
			//���̖ڕW�n�_�ւ̐ݒ�
			camera.SetGoalPos(VAdd(enemy_->GetPos(BOSS_IDX), cameraMoveGoalPos_[direcCnt_]));

			//���ڂ̈ړ��̓{�X�́u���сv�������
			enemy_->BossShout();
			//�u���сv���o�p�̃u���[��
			ChangeActionDirec(ACTION_DIRECTION::BLUR);
		}
	}
	//���o������
	return false;
}

void Game::Draw(void)
{
	stage_->Draw();
	enemy_->Draw();
	player_->Draw();

	//���j���[�{�^���̕\��
	UIManager2d::GetInstance().Draw(MENU_BTN);

	//DrawDebug();

	//�|�X�g�G�t�F�N�g��������Ƃ�
	if (isDrawPostEffect_) {
		//�`��
		(this->*drawPostEffect_)();
	}
}

void Game::DrawScanLine(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	SetDrawScreen(scanLineScreen_);

	// ��ʂ�������
	//ClearDrawScreen();

	DrawGraph(0, 0, mainScreen, false);
	scanLineRender_->Draw();

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, scanLineScreen_, false);
	//�|�X�g�G�t�F�N�g�̏ォ��N���ȕ������o��
	UIManager2d::GetInstance().Draw(WARNING_STR_IMG);
}

void Game::DrawBlur(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	blurMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });

	SetDrawScreen(blurScreen_);

	DrawGraph(0, 0, mainScreen, false);
	blurRender_->Draw();

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, blurScreen_, false);
}

void Game::DrawDodgeEffect(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	dodgeMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });

	SetDrawScreen(dodgeScreen_);

	DrawGraph(0, 0, mainScreen, false);
	dodgeRender_->Draw();

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, dodgeScreen_, false);
}

void Game::Release(void)
{
	player_->Release();
	enemy_->Release();
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.Stop("NomalBgm");	//���܂ŗ����Ă������̂��~
	sndM.Stop("BattleBgm");	//���܂ŗ����Ă������̂��~
}

void Game::Reset(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 80);	//�O�V�[���ɖ߂�̂�BGM�̉��ʂ𕜊�
	//�Ƃ肠�������j���[����̕��A���͒Ǐ]��
	//���j���[�J�����O�ɕς���\����
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void Game::StartBossFaze(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	ChangeActionDirec(ACTION_DIRECTION::SCAN_LINE);

	sndM.Stop("NomalBgm");	//���܂ŗ����Ă������̂��~
	sndM.Stop("BattleBgm");	//���܂ŗ����Ă������̂��~
	sndM.Play("WarningBgm");//�x��������

	//���o�����ݒ�
	direcState_ = BOSS_DIRECTION::POST_EFFECT;
	update_ = &Game::DirectionUpdate;
	direcUpdate_ = &Game::DirectionPostEffect;
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);	//���o���̓J����������󂯕t���Ȃ�
}

void Game::ChangeActionDirec(const ACTION_DIRECTION _direc)
{
	//�Ƃ肠�����|�X�g�G�t�F�N�g��`�悷��悤��
	isDrawPostEffect_ = true;

	//�e�|�X�g�G�t�F�N�g�̕`��ݒ�
	if (_direc == ACTION_DIRECTION::SCAN_LINE) {
		drawPostEffect_ = &Game::DrawScanLine;
	}
	else if (_direc == ACTION_DIRECTION::BLUR) {
		drawPostEffect_ = &Game::DrawBlur;
	}
	else if (_direc == ACTION_DIRECTION::JUST_DODGE) {
		drawPostEffect_ = &Game::DrawDodgeEffect;
	}
	else {
		//��L�O�ȊO�̏ꍇ�̓|�X�g�G�t�F�N�g�������Ȃ�
		isDrawPostEffect_ = false;
	}
}

void Game::AttackDataInit(void)
{
	//�U���̏�����
	atkMng_->AddAttack(PlayerManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD,false, false, PlayerManager::ATTACK_TIME);
	atkMng_->AddAttack(EnemyManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, true,false, EnemyManager::ATTACK_TIME, EnemyManager::ATTACK_TIME_START, EnemyManager::ATTACK_TIME_END);
}

const int Game::DecideRockEnemy(void)
{
	return enemy_->GetNearEnemyNum(player_->GetPos());
}

void Game::FinishSwitchBgm(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	//�؂芷���I��
	switchBgm_ = false;
	sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);
	//���܂ŗ����Ă������̂��~
	sndM.Stop(nowBgmStr_);	
	//���݂�BGM���Ɛ؂�ւ����BGM���̐؂芷��
	auto ret = nowBgmStr_;
	nowBgmStr_ = switchBgmStr_;
	switchBgmStr_ = ret;
	//������
	nextBgmVol_ = 0;
}

void Game::StartSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	//�X���[���o����
	slowCnt_ = 0;
	ChangeActionDirec(ACTION_DIRECTION::JUST_DODGE);	//���o
	isSlowEffect_ = true;
	//�X�V�X�s�[�h��50���ɐݒ�
	scM.SetUpdateSpeedRate_(SLOW_SPEED_PERCENT);
	//�G������ɑΉ�
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());
}

void Game::EndSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	isSlowEffect_ = false;
	ChangeActionDirec(ACTION_DIRECTION::NOMAL);
	//�X�V������100���ɂ��ǂ�
	scM.SetUpdateSpeedRate_(NOMAL_SPEED_PERCENT);
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());
}


void Game::DrawDebug(void)
{
	//SceneManager::GetInstance().GetCamera().DrawDebug();
	//if (isSlowEffect_) {
	//	DrawString(0, 140, "NOW_SLOW", 0xffffff);
	//}

	////player_->DrawDebug();
	//enemy_->DrawDebug();
	atkMng_->DrawDebug();
}
