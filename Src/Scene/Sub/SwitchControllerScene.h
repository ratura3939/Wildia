#pragma once
#include "../SceneBase.h"
class SwitchControllerScene :
    public SceneBase
{
public:
	SwitchControllerScene(void);
	~SwitchControllerScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	int coolTimeCnt_;		//�N�[���^�C��
	bool isSwCoolTime_;		//�؂芷���������s��ꂽ��
	bool isSwBtn_;			//�؂�ւ��{�^���ɂ��邩�ǂ���
};

