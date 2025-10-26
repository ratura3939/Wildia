#pragma once
#include<DxLib.h>
#include "../../Common/Quaternion.h"
#include<memory>

class Stage
{
public:
	Stage(bool _drawObj);
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

private:
	int modelId_;	//ƒ‚ƒfƒ‹ID
	int objModelId_;
	VECTOR pos_;	//À•W
	VECTOR scl_;	//ƒ‚ƒfƒ‹‘å‚«‚³
	VECTOR rot_;	//‰ñ“]î•ñ(XYZ)

	bool isDrawObj_;

	// ‰ñ“]
	Quaternion quaRot_;
};

