#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(int modelId, ModelMaterial& modelMaterial)
	: modelId_(modelId), modelMaterial_(modelMaterial)
{
}

ModelRenderer::~ModelRenderer(void)
{
}

void ModelRenderer::Draw(void)
{

	// �I���W�i���V�F�[�_�ݒ�(ON)
	MV1SetUseOrigShader(true);

	// �V�F�[�_�ݒ�(���_)
	SetReserveVS();

	// �V�F�[�_�ݒ�(�s�N�Z��)
	SetReservePS();

	// �e�N�X�`���A�h���X�^�C�v�̎擾
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// �e�N�X�`���A�h���X�^�C�v��ύX
	SetTextureAddressModeUV(texAType, texAType);

	// �`��
	MV1DrawModel(modelId_);

	// �e�N�X�`���A�h���X�^�C�v�����ɖ߂�
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// ��n��
	//-----------------------------------------

	// �e�N�X�`������
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// �O��g�p���̃e�N�X�`���������p���Ȃ��悤��
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// ���_�V�F�[�_����
	SetUseVertexShader(-1);

	// �s�N�Z���V�F�[�_����
	SetUsePixelShader(-1);

	// �I���W�i���V�F�[�_�ݒ�(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

}

void ModelRenderer::SetReserveVS(void)
{

	// �萔�o�b�t�@�n���h��
	int constBuf = modelMaterial_.GetConstBufVS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsVS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ���_�V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(constBuf);

	// ���_�V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// ���_�V�F�[�_�[�ݒ�
	SetUseVertexShader(modelMaterial_.GetShaderVS());

}

void ModelRenderer::SetReservePS(void)
{

	// �s�N�Z���V�F�[�_�[�Ƀe�N�X�`����]��
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// �O��g�p���̃e�N�X�`���������p���Ȃ��悤��
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
	}

	// �萔�o�b�t�@�n���h��
	int constBuf = modelMaterial_.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsPS();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(constBuf);

	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@��萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// �s�N�Z���V�F�[�_�[�ݒ�
	SetUsePixelShader(modelMaterial_.GetShaderPS());

}
