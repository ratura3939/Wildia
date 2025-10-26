#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;


class Title : public SceneBase
{

public:
	//�f�o�C�X���
	enum class DEVICE {
		KEY,
		PAD,
		MAX
	};
	
	Title(void);
	~Title(void);

	void Init(void) override;
	void InitUI(void);
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	void NomalUpdate(void);
	void SelectDeviceUpdate(void);

	/// <summary>
	/// �g�p����f�o�C�X�̑I��ݒ�
	/// </summary>
	/// <param name="_device">�g�p���悤�Ƃ��Ă���f�o�C�X</param>
	void SetSelectDevice(const DEVICE _device);

	//�f�o�C�X�I��
	void DrawDevice(void);

#pragma region shader�֘A
	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;
#pragma endregion

#pragma region ���̑��ϐ�
	int deviceImgs_[static_cast<int>(DEVICE::MAX)];		//�R���g���[���[�摜

	bool isSelectDevice_;								//�R���g���[���[�I�𒆂��ǂ���
	bool selectDevice_[static_cast<int>(DEVICE::MAX)];	//�I�΂�Ă��邩
	bool selectExit_;									//�u�߂�v���I������Ă��邩�ǂ���

	VECTOR allowPos_[static_cast<int>(DEVICE::MAX)];	//���ʒu(���ꂼ��i�[)

	using Update_f = void(Title::*)(void);
	Update_f update_;
#pragma endregion
};
