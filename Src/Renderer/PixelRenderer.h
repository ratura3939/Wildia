#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/PixelMaterial.h"

class PixelRenderer
{

public:

	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// ���_��
	static constexpr int NUM_VERTEX = 4;

	// ���_�C���f�b�N�X��
	static constexpr int NUM_VERTEX_IDX = 6;

	// �|���S����
	static constexpr int NUM_POLYGON = 2;

	// �R���X�g���N�^
	PixelRenderer(PixelMaterial& pixelMaterial);

	// �f�X�g���N�^
	~PixelRenderer(void);

	// �`���`�̐���
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	void MakeSquereVertex(void);

	// ���W�ݒ�
	void SetPos(Vector2 pos);

	// �摜�T�C�Y�ݒ�
	void SetSize(Vector2 size);

	// �`��
	void Draw(void);
	void Draw(int x, int y);

private:

	// ���W
	Vector2 pos_;

	// �`��T�C�Y
	Vector2 size_;

	// ���_
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// ���_�C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];

	// �s�N�Z���}�e���A��
	PixelMaterial& pixelMaterial_;

};
