#include "Stage.h"

#include"../../Utility/Utility.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Application.h"

Stage::Stage(bool _drawObj)
{
	isDrawObj_ = _drawObj;
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::STAGE_MDL).handleId_;
	objModelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OBJECTS_MDL).handleId_;

	pos_ = Utility::VECTOR_ZERO;
	pos_.y = -50.0f;
	scl_ = { 100.0f,100.0f,100.0f };
	rot_ = Utility::VECTOR_ZERO;

	MV1SetPosition(modelId_,pos_);
	MV1SetPosition(objModelId_,pos_);
}

void Stage::Update(void)
{
	// ‰ñ“]
	rot_ = quaRot_.ToEuler();
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
	if (isDrawObj_) {
		MV1DrawModel(objModelId_);
	}
}
