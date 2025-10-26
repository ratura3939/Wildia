#include<DxLib.h>
#include<cassert>
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new SoundManager();
	}
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}


void SoundManager::Add(const TYPE _type, const std::string _name, const int _data, const int _interval)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (sounds_.find(_name) != sounds_.end())return;


	//再生するときデータの種類によって
	//ループ再生か単発かを判断する
	int mode = -1;
	if (_type == TYPE::BGM) mode = DX_PLAYTYPE_LOOP;
	else mode = DX_PLAYTYPE_BACK;

	//新規データのため情報を追加
	sounds_.emplace(_name, SOUND_DATA{ _data,_type,mode});
	intervales_[_name] = _interval;
}


void SoundManager::Play(const std::string _name)
{
	//元データがないときは警告
	if (sounds_.find(_name) == sounds_.end())assert("設定していない音声を再生しようとしています。");

	//流そうとしているのがBGMのとき
	if (sounds_[_name].type == TYPE::BGM) {
		if (activeBgm_ == _name) {
			//同じBGMを再生しようとしているので
			return;
		}
		//流しているBGMの更新
		activeBgm_ = _name;
	}

	//カウンターに要素がないとき
	//カウンターに要素がないとき＝現在再生可能時
	if(!counteres_.contains(_name)) {
		//再生処理
		PlaySoundMem(sounds_[_name].data, sounds_[_name].playMode);
		//インターバルがある場合カウンタを生成
		if (intervales_[_name] > 0) {
			counteres_.emplace(_name, 0);
		}
	}
	
}


void SoundManager::Stop(const std::string _name)
{
	//元データがないときは警告
	if (sounds_.find(_name) == sounds_.end())assert("設定していない音声を停止しようとしています。");

	//停止処理
	StopSoundMem(sounds_[_name].data);
}

void SoundManager::Update(void)
{
	//カウンタ満了保存用
	std::vector<std::string>delteNames;

	//カウンタ更新
	for (auto& counter : counteres_) {
		counter.second++;
		//インターバルを満了したら
		if (counter.second >= intervales_[counter.first]) {
			//削除予定に入れる
			delteNames.push_back(counter.first);
		}
	}

	for (auto& name : delteNames) {
		//該当のカウンタを消去
		counteres_.erase(name);
	}
}

void SoundManager::Release(void)
{
	//配列要素善削除
	sounds_.clear();
	activeBgm_ = "";
}


void SoundManager::AdjustVolume(const std::string _name, const int _persent)
{
	//元データがないときは警告
	if (sounds_.find(_name) == sounds_.end())assert("設定していない音声を設定しようとしています。");
	ChangeVolumeSoundMem(VOLUME_BASIC * _persent, sounds_[_name].data);
}

void SoundManager::AdjustVolume(const TYPE _playType, const int _persent)
{
	for (auto& info : sounds_) {
		if (info.second.type == _playType) {
			ChangeVolumeSoundMem(VOLUME_BASIC * _persent, info.second.data);
		}
	}
}

void SoundManager::Destroy(void)
{
	Release();
	delete instance_;
}
