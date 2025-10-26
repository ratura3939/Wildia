#pragma once
#include "../SceneBase.h"
#include<memory>

class Stage;


class GameClear :
    public SceneBase
{
public:
	GameClear(void);
	~GameClear(void);

	void Init(void) override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	std::unique_ptr<Stage>stage_;	//ステージ
};

