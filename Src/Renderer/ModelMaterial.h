#pragma once
#include <string>
#include <vector>
#include <map>
#include <DxLib.h>

class ModelMaterial
{

public:

	// �ݒ�ł���ő�̃e�N�X�`���̐�
	static constexpr int MAX_TEXTURES = 4;

	// �e�N�X�`���A�h���X�^�C�v
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIST_NOISE,
		MAX
	};

	// �R���X�g���N�^
	ModelMaterial(
		std::string shaderFileNameVS, int constBufFloat4SizeVS,
		std::string shaderFileNamePS, int constBufFloat4SizePS
	);

	// �f�X�g���N�^
	~ModelMaterial(void);

	// �萔�o�b�t�@��ǉ�
	void AddConstBufVS(const FLOAT4& contBuf);
	void AddConstBufPS(const FLOAT4& contBuf);

	// �萔�o�b�t�@���X�V
	void SetConstBufVS(int idx, const FLOAT4& contBuf);
	void SetConstBufPS(int idx, const FLOAT4& contBuf);

	// �e�N�X�`�����X�V
	void SetTextureBuf(int slot, int texDiffuse);

	// �V�F�[�_
	int GetShaderVS(void) const;
	int GetShaderPS(void) const;

	// �萔�o�b�t�@�n���h��
	int GetConstBufVS(void) const;
	int GetConstBufPS(void) const;

	// �萔�o�b�t�@
	const std::vector<FLOAT4>& GetConstBufsVS(void) const;
	const std::vector<FLOAT4>& GetConstBufsPS(void) const;

	// �摜
	const std::map<int, int>& GetTextures(void) const;

	// �e�N�X�`���A�h���X���擾
	TEXADDRESS GetTextureAddress(void) const;

	// �e�N�X�`���A�h���X��ݒ�
	void SetTextureAddress(TEXADDRESS texA);

private:

	// ���_�V�F�[�_
	int shaderVS_;

	// �s�N�Z���V�F�[�_
	int shaderPS_;


	// ���_�萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	int constBufFloat4SizeVS_;

	// ���_�萔�o�b�t�@�n���h��
	int constBufVS_;

	// �s�N�Z���萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	int constBufFloat4SizePS_;

	// �s�N�Z���萔�o�b�t�@�n���h��
	int constBufPS_;


	// �e�N�X�`���A�h���X
	TEXADDRESS texAddress_;

	// �萔�萔�o�b�t�@
	std::vector<FLOAT4> constBufsVS_;

	// �s�N�Z���萔�o�b�t�@
	std::vector<FLOAT4> constBufsPS_;

	// �摜
	std::map<int, int> textures_;

};
