#pragma once
#include <string>
#include <vector>

class Resource
{

public:

	// ���\�[�X�^�C�v
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		CSV,
		MODEL,
		SOUND,
		EFFEKSEER
	};

	// �R���X�g���N�^
	Resource(void);
	// �R���X�g���N�^
	Resource(TYPE type, const std::string& path);
	// �R���X�g���N�^(IMGS�p)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// �f�X�g���N�^
	~Resource(void);

	// �ǂݍ���
	void Load(void);

	void LoadCsv(void);

	// ���
	void Release(void);

	// �����摜�n���h����ʔz��ɃR�s�[
	void CopyHandle(int* imgs);

	// ���\�[�X�^�C�v
	TYPE resType_;

	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;

	// �摜�ƃ��f���̃n���h��ID
	int handleId_;

	// IMGS::LoadDivGraph�p
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	//Csv�p�̓��I��
	std::vector<int> dmcHndIdX_;
	std::vector<std::vector<int>> dmcHandleIds_;
	std::vector<std::vector<int>>::iterator csv_;

	// ���f�������p
	std::vector<int> duplicateModelIds_;

};

