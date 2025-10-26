#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/ModelMaterial.h"

class ModelRenderer
{

public:

	// ���_�V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// �R���X�g���N�^
	ModelRenderer(int modelId_, ModelMaterial& modelMaterial);

	// �f�X�g���N�^
	~ModelRenderer(void);

	// �`��
	void Draw(void);

private:

	// ���f���̃n���h��ID
	int modelId_;

	// ���f���}�e���A��
	ModelMaterial& modelMaterial_;

	// �V�F�[�_�ݒ�(���_)
	void SetReserveVS(void);

	// �V�F�[�_�ݒ�(�s�N�Z��)
	void SetReservePS(void);

};
