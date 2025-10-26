#pragma once
#include "../SceneBase.h"
#include<memory>
#include<string>

class PlayerManager;
class EnemyManager;
class AttackManager;
class CollisionManager;
class Stage;

class PixelMaterial;
class PixelRenderer;

class Game :
    public SceneBase
{
public:
	static constexpr int CAMERA_DIRECTION_NUM = 2;			//�J�������o�ɂ�����ړ���

	/// <summary>
	/// �{�X�̉��o
	/// </summary>
	enum class BOSS_DIRECTION {
		NONE,
		POST_EFFECT,	//�|�X�g�G�t�F�N�g
		SHAKE_SCREEN,	//��ʗh��
		CAMERA_MOVE,	//�J�����ړ�
		END
	};

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̎��
	/// </summary>
	enum class ACTION_DIRECTION {
		NOMAL,
		BLUR,
		JUST_DODGE,
		SCAN_LINE,
		END
	};

	Game(void);
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Release(void) override;
	void Reset(void)override;

	//�{�X�o���ŏ��̏����p
	void StartBossFaze(void);

	//�u���[����邩����Ȃ���
	void ChangeActionDirec(const ACTION_DIRECTION _direc);	

	//�X���[�I��
	void EndSlow(void);		

private:
	//�e������
	void InitSound(void)override;
	void InitEffect(void)override;
	void InitShader(void);

	//�e��X�V
	void GameUpdate(void);			//�Q�[���ʏ�
	void DirectionUpdate(void);		//���o�A�b�v�f�[�g
	bool DirectionPostEffect(void);	//�|�X�g�G�t�F�N�g
	bool DirectionShakeScreen(void);//��ʗh�ꉉ�o
	void DoShake(void);				//�h�炷
	bool DirectionCameraMove(void);	//�J�����ړ�

	//�e��`�揈��(�|�X�g�G�t�F�N�g)
	void DrawScanLine(void);	//������
	void DrawBlur(void);		//�u���[
	void DrawDodgeEffect(void);	//�W���X�g�����

	/// <summary>
	/// �U���̊�b���o�^(�䂭�䂭�͊O���f�[�^�ɂ�����)
	/// </summary>
	void AttackDataInit(void);

	/// <summary>
	/// ���b�N�I���̑Ώی���
	/// </summary>
	/// <param name=""></param>
	const int DecideRockEnemy(void);

	//�؂芷���I�����̏���
	void FinishSwitchBgm(void);

	//�X���[���o�J�n
	void StartSlow(void);	
	
	//�f�o�b�O�`��
	void DrawDebug(void);


	//�ϐ�
#pragma region �C���X�^���X
	std::unique_ptr<PlayerManager>player_;			//�v���C���[
	std::unique_ptr<EnemyManager>enemy_;			//�G
	std::unique_ptr<AttackManager>atkMng_;			//�U���֘A
	std::unique_ptr<CollisionManager>collision_;	//����֘A
	std::unique_ptr<Stage>stage_;					//�X�e�[�W
#pragma endregion

#pragma region �֐��|�C���^
	//�X�V�֐�
	using Update_f = void(Game::*)(void);
	using DirecUpdate_f = bool(Game::*)(void);
	Update_f update_;			//�ʏ�E���o�̓���Ǘ�
	DirecUpdate_f direcUpdate_;	//���o�̃|�X�g�G�t�F�N�g�E��ʗh��E�J�����ړ��̎O���Ǘ�

	//�`��֐�
	using DrawPostEffect_f = void(Game::*)(void);
	DrawPostEffect_f drawPostEffect_;	//�|�X�g�G�t�F�N�g�Ǘ�
#pragma endregion

#pragma region shader�֘A
	//������
	std::unique_ptr<PixelMaterial>scanLineMaterial_;
	std::unique_ptr<PixelRenderer>scanLineRender_;
	int scanLineScreen_;

	//�u���[�֘A
	std::unique_ptr<PixelMaterial>blurMaterial_;
	std::unique_ptr<PixelRenderer>blurRender_;
	int blurScreen_;
	//�W���X�g���
	std::unique_ptr<PixelMaterial>dodgeMaterial_;
	std::unique_ptr<PixelRenderer>dodgeRender_;
	int dodgeScreen_;

	bool isDrawPostEffect_;	//�|�X�g�G�t�F�N�g�������邩
#pragma endregion


#pragma region ���̑��ϐ�
	//�X���[���o
	bool isSlowEffect_;	//ON/OFF�t���O
	int slowCnt_;		//�J�E���^

	//BGM
	std::string nowBgmStr_;		//���݂�BGM
	std::string switchBgmStr_;	//�؂�ւ����BGM
	int nextBgmVol_;			//���ʒ����p(BGM�؂�ւ����Ɏg�p)
	bool switchBgm_;			//�؂芷���J�n�t���O

	BOSS_DIRECTION direcState_;		//�{�X���o�Ǘ�
	int direcCnt_;					//���o�Ɋւ��J�E���^

	//�J�����̉��o�p
	VECTOR cameraMoveStartPos_;						//�����ʒu
	VECTOR cameraMoveGoalPos_[CAMERA_DIRECTION_NUM];	//�ڕW�ʒu
	int cameraShakeCollTimeCnt_;	//��ʗh��N�[���^�C��
	bool stayCameraShake_;			//��ʗh��ҋ@�t���O true=�ҋ@
#pragma endregion
};

