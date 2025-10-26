#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;
class EnemyCount;
class Game;
class AttackManager;

class EnemyManager
{
public:
	static constexpr int ENEMY_NUM = 4;			//�G����

	//�U���o�^��(�䂭�䂭�͊O���f�[�^)
	static const std::string ATTACK_NOMAL;

	static constexpr float TARGETTING_PERMISSION_DISTANCE = 1000.0f;	//�^�[�Q�e�B���O��������͈�

	//�f�o�b�O�p
	static constexpr int ATTACK_TIME = 100;
	static constexpr int ATTACK_TIME_START = 20;
	static constexpr int ATTACK_TIME_END = 90;

	static constexpr VECTOR INIT_1 = { 0.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_2 = { 500.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_3 = { 0.0f,0.0f,-1000.0f };
	static constexpr VECTOR INIT_4 = { -500.0f,0.0f,-1000.0f };

	EnemyManager(Game& _scene);
	~EnemyManager(void);

	void Init(void);
	void Update(const VECTOR& _playerPos, AttackManager& _atkMng);
	void UpdateAnim(void);
	void Draw(void);
	void Release(void);

	//�G�����̎擾
	std::vector<std::weak_ptr<EnemyBase>> GetEnemys(void);

	//�ʒu�E��]�擾
	const VECTOR GetPos(const int _num);//���W
	const Quaternion GetQua(const int _num);//��]

	//��ԋ߂��G�̔z��ԍ���Ԃ�
	int GetNearEnemyNum(const VECTOR _pPos);
	//��ʓ��ł��邩�ǂ���
	bool InsideScreen(const VECTOR _pos);

	/// <summary>
	/// �퓬��Ԃ̓G�����邩�ǂ���
	/// </summary>
	/// <returns>true=����/false=���Ȃ�</returns>
	bool IsBattleEnemy(void);

	/// <summary>
	/// IsBattleEnemy��ON�܂���OFF�̃g���K�[
	/// </summary>
	/// <returns>true=�g���K�N��/false=�ω��Ȃ�</returns>
	bool IsSwitchBattleOrNomalEnemyTrg(void);

	//�A�j���̃X�s�[�h�̐ݒ�
	void SetAnimSpeedRate(const float _percent);

	/// <summary>
	/// ���b�N�I�������\���̂���G���Z�b�g
	/// </summary>
	/// <param name="_num">�G�ԍ�</param>
	void SetTargetEnemy(const int _num);

	/// <summary>
	/// ���b�N�I���ΏۂɂȂ�悤�ȓG�����Ȃ��Ƃ��ɋN���鏈��
	/// </summary>
	/// <param name=""></param>
	void NoTargetEnemy(void);

	//���b�N�I�����ꂽ����
	void LokedOn(const int _num);

	void CreateBoss(void);
	void BossShout(void);

	void DrawDebug(void);

private:
	std::vector<std::shared_ptr<EnemyBase>> characters_;
	bool preBattle_;	//�ʒu�t���[���O���퓬��Ԃł��邩�ǂ���

	int enemyCnt_;		//�G�J�E���g
	int* numImg_;		//�ԍ�
	VECTOR platePos_;	//�G���\���@�v���[�g

	bool createBoss_;	//�{�X�𐶐������`��

	std::unique_ptr<EnemyCount>counterUI_;	//�G�c���J�E���^�[

	Game& gameScene_;
};

