#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class Game;
class EnemyManager;
class PlayerChara;
class AttackManager;
class LockOnManager;

class PlayerManager
{
public:
	//�U���o�^��(�䂭�䂭�͊O���f�[�^)
	static const std::string ATTACK_NOMAL;
	//�f�o�b�O�p
	static constexpr int ATTACK_TIME = 50;

	//��ԏ������
	static constexpr int LIMIT_AVOID_STATE = 30;	//���

	PlayerManager(Game& _gameScene, EnemyManager& _enemy);
	~PlayerManager(void);

	void Init(void);
	void Update(AttackManager& _atk);
	void Draw(void);
	void Release(void);

	//�L�����N�^�[�擾
	std::weak_ptr<PlayerChara> GetPlayer(void);

	//�ʒu�E��]�擾
	const VECTOR GetPos(void);			//���W
	const Quaternion GetQua(void);		//��]
	const VECTOR GetFocusPoint(void);	//�����_

	//�������Ă��邩
	const bool IsAlive(void)const;

	//���b�N�I���E�I�t���ɕK�v�ȏ���
	void RedyLockOn(void);
	void RedyLockOff(void);
	
	//�f�o�b�O
	void DrawDebug(void);

private:
	/// <summary>
	/// �v���C���[����̓��͎�t�֌W
	/// </summary>
	void UserInput(AttackManager& _atk);

	/// <summary>
	/// ����\�����Ɉړ����͂����邩
	/// </summary>
	const bool IsDudgeMove(void)const;
	void DoDudge(void);

	void RedyStateCount(const int _limit);

	Game& scene_;	//�Q�[���N���X�Q��
	std::shared_ptr<PlayerChara> character_;//�L�����N�^�[
	std::unique_ptr<LockOnManager>lockOn_;	//���b�N�I���֌W

	int stateCnt_;		//�L�����N�^�[��ԊǗ��p�J�E���^�[
	int stateLimit_;	//��Ԃ��̎��ԏ��
};

