#pragma once
#include<unordered_map>
#include<string>


//Dxlib���ɉ����f�[�^��3D��ԏ�ɍĐ����鏈��������
//�ȒP�Ɍ����Ή����ɋ��������邱�Ƃ��\
//�����΂��蕡�G�����Ȃ̂Ō���ɉ�


class SoundManager
{
public:
	//�Đ��̎�ޕ�������Ƃ��Ɏg��
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	struct SOUND_DATA
	{
		int data;		//�����f�[�^�i�[
		TYPE type;		//�Đ����
		int playMode;	//�����f�[�^�̍Đ��^�C�v
	};

	static constexpr int VOLUME_BASIC = 255 / 100;

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SoundManager& GetInstance(void);

	/// <summary>
	/// �T�E���h�̒ǉ�
	/// </summary>
	/// <param name="_type">���̎�ޕ���(SE��BGM��)</param>
	/// <param name="_name">�o�^��</param>
	/// <param name="_data">���̃f�[�^</param>
	/// <param name="_interval">�Đ��Ԋu����</param>
	void Add(const TYPE _type, const std::string _name, const int _data, const int _interval = 0);

	/// <summary>
	/// �����f�[�^
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void Play(const std::string _name);

	/// <summary>
	/// ��~����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void Stop(const std::string _name);

	//�X�V
	void Update(void);

	//���
	void Release(void);

	/// <summary>
	/// ���ʒ���
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_persent">��������(0%�`100%)</param>
	void AdjustVolume(const std::string _name, const int _persent);
	/// <summary>
	/// ���ʒ���
	/// </summary>
	/// <param name="_playType">���ߑΏ�(BGMorSE)</param>
	/// <param name="_persent">��������(0%�`100%)</param>
	void AdjustVolume(const TYPE _playType, const int _persent);

	void Destroy(void);
private:
	//�C���X�^���X�p
	static SoundManager* instance_;

	//�f�[�^�i�[�p
	std::unordered_map<std::string, SOUND_DATA>sounds_;
	std::string activeBgm_;

	//�Đ��Ԋu
	std::unordered_map<std::string,int> intervales_;
	std::unordered_map<std::string,int> counteres_;

	//�R���X�g���N�^���f�X�g���N�^
	SoundManager() = default;
	~SoundManager() = default;
};

