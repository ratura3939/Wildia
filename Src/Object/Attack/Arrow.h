#pragma once
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Manager/GameSystem/AttackManager.h"

class Quaternion;

class Arrow
{
public:
	//�萔
	static constexpr VECTOR ARROW_LOCAL_POS = { 15.0f,30.0f,50.0f };	//�ł��n�߂Ɉ�a�����Ȃ��悤�ɒ���
	static constexpr float GRAVITY = 0.5f;			//�d��
	static constexpr float START_UP_ANGLE = 10.0f;	//�ŏ��̏�A���O��
	static constexpr float MOVE_SPEED = 5.0f;

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	/// <summary>
	/// �|��̐���
	/// </summary>
	/// <param name="_master">�U���Ґw�c</param>
	/// <param name="_mdlId">���f��</param>
	/// <param name="_pow">�U����</param>
	/// <param name="_qua">��]</param>
	Arrow(const AttackManager::ATTACK_MASTER& _master,const int _mdlId,const VECTOR _pos,const float _pow,const Quaternion& _qua);
	~Arrow(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release();
	void Destroy(void);

	const VECTOR GetPos(void)const { return pos_; }
	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

	//�󋵕ψ�
	void ChangeState(const STATE _state) { state_ = _state; }

private:
	void UpdateRotQuat(void);
	//�ړ�����
	void Move(void);
	//�����t���O�ݒ�
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }

	// �e��������擾
	VECTOR GetForward(void) const;
	VECTOR GetDown(void) const;

	// �Ώە������擾
	VECTOR GetDir(const VECTOR& _vec) const;

	//�ʒu���
	int modelId_;	//���f��ID
	VECTOR pos_;	//���W
	VECTOR scl_;	//���f���傫��
	VECTOR rot_;	//��]���(XYZ)

	//�e���̍s��
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// ��]
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ���[�J����]
	Quaternion quaRotLocal_;

	//������
	AttackManager::ATTACK_MASTER master_;
	//���
	STATE state_;
	//�U����
	float atkPow_;
	//��������
	bool isAlive_;
};

