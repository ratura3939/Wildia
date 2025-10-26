#pragma once
#include "../SceneBase.h"

#include<vector>
#include<string>

class PauseScene :
    public SceneBase
{
public:
	/// <summary>
	/// ���j���[���̍���
	/// </summary>
	enum class MENU_ITEM {
		NONE,
		BACK_GAME,	//�Q�[���ɖ߂�
		CONFIG,		//������@
		SWITCH_CNTL,//����؂芷��
		BACK_TITLE,	//�Q�[����߂�(�^�C�g���ɖ߂�)
	};

	PauseScene(void);
	~PauseScene(void)override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	//���͎�t
	void InputUser(void);

	//����`�悷�ׂ��ʒu���擾
	const VECTOR GetDrawPosOfArrow(void)const;

	std::vector<std::string>drawBtnList_;	//�`�悳���{�^�����ォ�珇��
	int selectIdx_;	//�J�[�\���ʒu
};

