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
	int modelId_;	//���f��ID
	int objModelId_;
	VECTOR pos_;	//���W
	VECTOR scl_;	//���f���傫��
	VECTOR rot_;	//��]���(XYZ)

	bool isDrawObj_;

	// ��]
	Quaternion quaRot_;
};

