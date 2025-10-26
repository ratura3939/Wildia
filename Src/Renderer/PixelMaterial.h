#pragma once
#include <string>
#include <vector>
#include <DxLib.h>

class PixelMaterial
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
		MAX
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="shaderFileName">�V�F�[�_�[�t�@�C����</param>
	/// <param name="constBufFloat4Size">�萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)</param>
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);

	// �f�X�g���N�^
	~PixelMaterial(void);

	// �萔�o�b�t�@��ǉ�
	void AddConstBuf(const FLOAT4& contBuf);

	// �萔�o�b�t�@���X�V
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	// �e�N�X�`���o�b�t�@��ǉ�
	void AddTextureBuf(int texDiffuse);

	// �e�N�X�`�����X�V
	void SetTextureBuf(int idx, int texDiffuse);

	// �V�F�[�_
	int GetShader(void) const;

	// �萔�o�b�t�@�n���h��
	int GetConstBuf(void) const;

	// �萔�o�b�t�@
	const std::vector<FLOAT4>& GetConstBufs(void) const;

	// �摜
	const std::vector<int>& GetTextures(void) const;

	// �e�N�X�`���A�h���X���擾
	TEXADDRESS GetTextureAddress(void) const;

	// �e�N�X�`���A�h���X��ݒ�
	void SetTextureAddress(TEXADDRESS texA);

private:

	// �V�F�[�_
	int shader_;

	// �萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	int constBufFloat4Size_;

	// �萔�o�b�t�@�n���h��
	int constBuf_;

	// �e�N�X�`���A�h���X
	TEXADDRESS texAddress_;

	// �萔�o�b�t�@
	std::vector<FLOAT4> constBufs_;

	// �摜
	std::vector<int> textures_;

};
