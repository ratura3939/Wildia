#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial) : pixelMaterial_(pixelMaterial)
{
}

PixelRenderer::~PixelRenderer(void)
{
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{

	pos_ = pos;
	size_ = size;

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	// �S���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// ����
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// �E��
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// �E��
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// ����
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	/*
	�@�`�`�`�`�`�`
		0-----1
		|     |
		|     |
		3-----2
	�@�`�`�`�`�`�`
		0-----1
		|  �^
		|�^
		3
	�@�`�`�`�`�`�`
			  1
		   �^ |
		 �^   |
		3-----2
	�@�`�`�`�`�`�`
	*/


	// ���_�C���f�b�N�X
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;

}

void PixelRenderer::MakeSquereVertex(void)
{
	MakeSquereVertex(pos_, size_);
}

void PixelRenderer::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void PixelRenderer::SetSize(Vector2 size)
{
	size_ = size;
}

void PixelRenderer::Draw(void)
{

	// �s�N�Z���V�F�[�_�ݒ�
	SetUsePixelShader(pixelMaterial_.GetShader());

	size_t size;

	// �s�N�Z���V�F�[�_�Ƀe�N�X�`����]��
	const auto& textures = pixelMaterial_.GetTextures();
	size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, textures[i]);
	}

	// �萔�o�b�t�@�n���h��
	int constBuf = pixelMaterial_.GetConstBuf();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = pixelMaterial_.GetConstBufs();

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

	// �萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(constBuf);

	// �萔�o�b�t�@���s�N�Z���V�F�[�_�[�p�萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// �e�N�X�`���A�h���X�^�C�v�̎擾
	auto texA = pixelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// �e�N�X�`���A�h���X�^�C�v��ύX
	SetTextureAddressModeUV(texAType, texAType);

	// �`��
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// �e�N�X�`���A�h���X�^�C�v�����ɖ߂�
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);
	
	// ��n��
	//-----------------------------------------

	// �e�N�X�`������
	size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, -1);
	}

	// �s�N�Z���V�F�[�_����
	SetUsePixelShader(-1);

	// �I���W�i���V�F�[�_�ݒ�(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

}

void PixelRenderer::Draw(int x, int y)
{
	pos_.x = x;
	pos_.y = y;
	MakeSquereVertex();
	Draw();
}
