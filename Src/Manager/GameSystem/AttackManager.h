#pragma once
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include<map>
#include<DxLib.h>
#include"../../Common/Quaternion.h"


//class AttackBase;
class Arrow;

class AttackManager
{
public:
	//�W���X�g�K�[�h�P�\
	static constexpr float GRACE_JUST_GUARD = 20.0f;

	//�U�������҂̐w�c
	enum class ATTACK_MASTER {
		NONE,
		PLAYER,
		ENEMY
	};

	//�U���̎��
	enum class ATTACK_TYPE {
		SWORD,	//��(�ߐ�)
		BOW,	//�|(������)
	};

	//�U����b���
	struct AttackInfo {
		ATTACK_MASTER group;//�U���̎�̐w�c
		ATTACK_TYPE type;	//�U���̑���
		bool isCopy;		//���������\��(�v���C���[�͈�̂Ȃ̂ŕ����͂��Ȃ��Ă���)
		float scale;		//����̑傫��
		float totalMotion;	//���[�V����������
		float startAttack;	//�U�����蔭������
		float endAttack;	//�U��������Ŏ���
		float counter;		//�J�E���^�[
		bool isFriendFire;	//FF�A�����ǂ���
		bool isHit;			//���łɔ��肳�ꂽ���ǂ���
		bool isAllert;

		//�ȉ��̊֐��͓G�p

		/// <summary>
		/// �O�����ǂ���
		/// </summary>
		/// <returns>true=Yes/false=No</returns>
		const bool IsPreGap(void)const { return (counter > 0.0f) && (counter < startAttack); }
		/// <summary>
		/// �W���X�g�K�[�h�̃^�C�~���O�ł��邩
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		const bool IsJustGuardTiming(void)const { return (counter >= 0.0f) && (counter <= startAttack + GRACE_JUST_GUARD); }

		const bool IsOuccerAttack(void)const { return (counter >= startAttack) && (counter <= endAttack); }
	};

	//�U�����̂���
	struct AttackItself {
		VECTOR pos;
		float pow;
	};

	/// <summary>
	/// �����蔻��Ɏg�p��������܂Ƃ߂�����
	/// </summary>
	struct AttackCollision {
		AttackInfo& info;
		AttackItself attack;
	};

	/// <summary>
	/// �U���̓o�^
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_type">�U���̎��</param>
	/// <param name="_friendFire">FF���肩�ǂ���</param>
	/// <param name="_total">���Đ�����</param>
	/// <param name="_modelId">���f�����(�|����)</param>
	/// <param name="_start">����J�n</param>
	/// <param name="_end">����I��</param>
	void AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _copy, const bool _friendFire,
		const float _total, const float _start = 0.0f, const float _end = 0.0f, const int _modelId = -1);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_pow">�U����</param>
	/// <param name="_pos">�����ʒu</param>
	/// <param name="_qua">��]���</param>
	/// <param name="_maseter">�w�c</param>
	/// <param name="_scale">�傫��</param>
	/// <param name="_sndName">�Đ�������ʉ�</param>
	/// <param name="_arrowModel">��̃��f��(�|�U�����̂�)</param>
	void Attack(const std::string _master,const std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _group, const float _scale, const std::string _sndName = "", const int _arrowModel = -1);

	bool Update(void);

	/// <summary>
	/// �A�N�e�B�u�ȍU�������ׂēn��
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<AttackCollision> GetActiveAttacks(void);

	/// <summary>
	/// �����[�V�������Ԏ擾
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <returns>����</returns>
	const float GetTotalTime(const std::string _name)const;

	void DrawDebug(void);

private:
	std::unordered_map<std::string, AttackInfo>attackInfoes_;	//�U�����
	/// <summary>
	/// ���L�ҁA�U�����A�U�����
	/// </summary>
	std::map<std::string,std::map<std::string, AttackCollision>>activeAttacks_;	//�L���ȍU��
};

