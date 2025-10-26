#include "../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(
	std::string shaderFileNameVS, int constBufFloat4SizeVS,
	std::string shaderFileNamePS, int constBufFloat4SizePS)
{

	// ���_�V�F�[�_�̃��[�h
	shaderVS_ = LoadVertexShader(
		(Application::PATH_SHADER + shaderFileNameVS).c_str());

	// ���_�萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// ���_�V�F�[�_�[�p�̒萔�o�b�t�@���쐬
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);



	// �s�N�Z���V�F�[�_�̃��[�h
	shaderPS_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileNamePS).c_str());

	// �s�N�Z���萔�o�b�t�@�̊m�ۃT�C�Y(FLOAT4��������邩)
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���쐬
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);



	// �e�N�X�`���A�h���X
	texAddress_ = TEXADDRESS::CLAMP;

}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}

}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}

}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;

}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;

}

void ModelMaterial::SetTextureBuf(int slot, int texDiffuse)
{

	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, texDiffuse);
	}
	else
	{
		textures_[slot] = texDiffuse;
	}

}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsVS(void) const
{
	return constBufsVS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsPS(void) const
{
	return constBufsPS_;
}

const std::map<int, int>& ModelMaterial::GetTextures(void) const
{
	return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}

int ModelMaterial::GetShaderVS(void) const
{
	return shaderVS_;
}

int ModelMaterial::GetShaderPS(void) const
{
	return shaderPS_;
}

int ModelMaterial::GetConstBufVS(void) const
{
	return constBufVS_;
}

int ModelMaterial::GetConstBufPS(void) const
{
	return constBufPS_;
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}
