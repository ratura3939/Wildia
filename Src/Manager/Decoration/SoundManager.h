#pragma once
#include<unordered_map>
#include<string>


//Dxlib内に音声データを3D空間上に再生する処理がある
//簡単に言えば音声に距離をつけることが可能
//少しばかり複雑そうなので後日に回す


class SoundManager
{
public:
	//再生の種類分けするときに使う
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	struct SOUND_DATA
	{
		int data;		//音声データ格納
		TYPE type;		//再生種類
		int playMode;	//音声データの再生タイプ
	};

	static constexpr int VOLUME_BASIC = 255 / 100;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SoundManager& GetInstance(void);

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_type">音の種類分け(SEかBGMか)</param>
	/// <param name="_name">登録名</param>
	/// <param name="_data">音のデータ</param>
	/// <param name="_interval">再生間隔制限</param>
	void Add(const TYPE _type, const std::string _name, const int _data, const int _interval = 0);

	/// <summary>
	/// 音声データ
	/// </summary>
	/// <param name="_name">登録名</param>
	void Play(const std::string _name);

	/// <summary>
	/// 停止処理
	/// </summary>
	/// <param name="_name">登録名</param>
	void Stop(const std::string _name);

	//更新
	void Update(void);

	//解放
	void Release(void);

	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_persent">調整割合(0%～100%)</param>
	void AdjustVolume(const std::string _name, const int _persent);
	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="_playType">調節対象(BGMorSE)</param>
	/// <param name="_persent">調整割合(0%～100%)</param>
	void AdjustVolume(const TYPE _playType, const int _persent);

	void Destroy(void);
private:
	//インスタンス用
	static SoundManager* instance_;

	//データ格納用
	std::unordered_map<std::string, SOUND_DATA>sounds_;
	std::string activeBgm_;

	//再生間隔
	std::unordered_map<std::string,int> intervales_;
	std::unordered_map<std::string,int> counteres_;

	//コンストラクタ＆デストラクタ
	SoundManager() = default;
	~SoundManager() = default;
};

