#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;


class Title : public SceneBase
{

public:
	//デバイス種類
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
	/// 使用するデバイスの選択設定
	/// </summary>
	/// <param name="_device">使用しようとしているデバイス</param>
	void SetSelectDevice(const DEVICE _device);

	//デバイス選択
	void DrawDevice(void);

#pragma region shader関連
	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;
#pragma endregion

#pragma region その他変数
	int deviceImgs_[static_cast<int>(DEVICE::MAX)];		//コントローラー画像

	bool isSelectDevice_;								//コントローラー選択中かどうか
	bool selectDevice_[static_cast<int>(DEVICE::MAX)];	//選ばれているか
	bool selectExit_;									//「戻る」が選択されているかどうか

	VECTOR allowPos_[static_cast<int>(DEVICE::MAX)];	//矢印位置(それぞれ格納)

	using Update_f = void(Title::*)(void);
	Update_f update_;
#pragma endregion
};
