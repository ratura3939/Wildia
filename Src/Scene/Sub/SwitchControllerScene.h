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

	int coolTimeCnt_;		//クールタイム
	bool isSwCoolTime_;		//切り換え処理が行われたか
	bool isSwBtn_;			//切り替えボタンにいるかどうか
};

