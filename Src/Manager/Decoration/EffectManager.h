#pragma once
#include<DxLib.h>
#include"SoundManager.h"
#include"../../Common/Quaternion.h"
#include<unordered_map>
#include<map>
#include<string>

//���ʂ̂Ȃ��悤�ɗv���P

class EffectManager
{
public:

	//�e��G�t�F�N�g�̏��
	static constexpr int NONE_MAX = 5;


	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static EffectManager& GetInstance(void);

	/// <summary>
	/// �G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="_name">�G�t�F�N�g�o�^��</param>
	/// <param name="_data">�G�t�F�N�g�̃f�[�^</param>
	void Add(const std::string& _name,int _data);

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g�o�^��</param>
	/// <param name="_pos">�Đ��ʒu</param>
	/// <param name="_qua">�p�x</param>
	/// <param name="_size">�傫��</param>
	/// <param name="_sound">���ʉ�</param>
	void Play(const std::string _master, const std::string& _name,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed = 1.0f,
		const std::string& _sndName = "");

	/// <summary>
	/// �G�t�F�N�g�̍Đ���~
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g�o�^��</param>
	void Stop(const std::string _master,const std::string& _name);

	/// <summary>
	/// �G�t�F�N�g�̊e�p�����[�^����
	/// </summary>
	/// <param name="_efc">�G�t�F�N�g��</param>
	/// <param name="_pos">�ʒu���</param>
	/// <param name="_qua">��]���</param>
	/// <param name="_size">�傫��</param>
	void SyncEffect(const std::string _master,const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size,const float& _speed,const int _idx);


	//�X�V����
	void Update(void);

	//�������
	void Release(void);

	//��������
	void Destroy(void);

private:
	//�C���X�^���X�p
	static EffectManager* instance_;

	//�G�t�F�N�g�f�[�^�i�[�p
	std::unordered_map<std::string,int> effectRes_;	//�����f�[�^
	std::map<std::string,std::map<std::string,std::vector<int>>> effectPlay_;//�Đ��f�[�^

	//�R���X�g���N�^���f�X�g���N�^
	EffectManager(void);
	~EffectManager() = default;
};

