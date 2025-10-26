#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

#include<fstream>
#include<sstream>


#include"../../Utility/Utility.h"

Resource::Resource(void)
{
	resType_ = TYPE::NONE;
	path_ = "";

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path)
{
	resType_ = type;
	path_ = path;

	numX_ = -1;
	numY_ = -1;
	sizeX_ = -1;
	sizeY_ = -1;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY)
{
	resType_ = type;
	path_ = path;
	numX_ = numX;
	numY_ = numY;
	sizeX_ = sizeX;
	sizeY_ = sizeY;

	handleId_ = -1;
	handleIds_ = nullptr;
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		// 画像
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::IMGS:
		// 複数画像
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case Resource::TYPE::CSV:
		LoadCsv();
		csv_ = dmcHandleIds_.begin();
		break;

	case Resource::TYPE::MODEL:
		// モデル
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::SOUND:
		handleId_=LoadSoundMem(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:

		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	}

}

void Resource::LoadCsv(void)
{
	std::ifstream ifs = std::ifstream(path_.c_str());

	if (!ifs)
	{
		return;
	}

	int chipNo = 0;
	//列の先頭から保存する
	int x = 0;
	

	//行格納用領域
	std::string line;
	while (getline(ifs, line))	//行がある間
	{
		//Split関数戻り値受け取り用
		std::vector<std::string> strSplit;

		strSplit = Utility::Split(line, ',');	//関数の呼び出し

		//一文字の情報
		std::string chipData;
		//分割したマップデータを格納する
		for (int x = 0; x < strSplit.size(); x++)
		{
			chipNo = stoi(strSplit[x]);
			dmcHndIdX_.push_back(chipNo);	//配列内に格納
		}
		dmcHandleIds_.push_back(dmcHndIdX_);	//配列内に格納
		dmcHndIdX_.clear();
	}
}

void Resource::Release(void)
{

	switch (resType_)
	{
	case Resource::TYPE::IMG:
		DeleteGraph(handleId_);
		break;

	case Resource::TYPE::IMGS:
	{
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
		break;

	case Resource::TYPE::CSV:
	
		break;

	case Resource::TYPE::MODEL:
	{
		MV1DeleteModel(handleId_);
		auto ids = duplicateModelIds_;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case Resource::TYPE::EFFEKSEER:

		DeleteEffekseerEffect(handleId_);
		break;

	}

}

void Resource::CopyHandle(int* imgs)
{

	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}
