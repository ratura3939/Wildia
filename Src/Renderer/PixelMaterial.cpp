#include "../Application.h"
#include "PixelMaterial.h"

PixelMaterial::PixelMaterial(std::string shaderFileName, int constBufFloat4Size)
{

	// �s�N�Z���V�F�[�_�̃��[�h
	shader_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileName).c_str());

	// �萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	constBufFloat4Size_ = constBufFloat4Size;

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���쐬
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size);

	// �e�N�X�`���A�h���X
	texAddress_ = TEXADDRESS::CLAMP;

}

void PixelMaterial::AddConstBuf(const FLOAT4& contBuf)
{

	if (constBufFloat4Size_ > constBufs_.size())
	{
		constBufs_.emplace_back(contBuf);
	}

}

void PixelMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufs_.size())
	{
		return;
	}

	constBufs_[idx] = contBuf;

}

void PixelMaterial::AddTextureBuf(int texDiffuse)
{

	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}

}

void PixelMaterial::SetTextureBuf(int idx, int texDiffuse)
{

	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;

}

const std::vector<FLOAT4>& PixelMaterial::GetConstBufs(void) const
{
	return constBufs_;
}

const std::vector<int>& PixelMaterial::GetTextures(void) const
{
	return textures_;
}

PixelMaterial::TEXADDRESS PixelMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void PixelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}

int PixelMaterial::GetShader(void) const
{
	return shader_;
}

int PixelMaterial::GetConstBuf(void) const
{
	return constBuf_;
}

PixelMaterial::~PixelMaterial(void)
{
	DeleteShader(shader_);
	DeleteShaderConstantBuffer(constBuf_);
}
