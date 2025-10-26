#include<EffekseerForDXLib.h>
#include<cassert>
#include "EffectManager.h"

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new EffectManager();
	}
}

EffectManager& EffectManager::GetInstance(void)
{
	return *instance_;
}

EffectManager::EffectManager(void) {

}

void EffectManager::Add(const std::string& _name, int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (effectRes_.find(_name) != effectRes_.end())return;

	//新規データのため情報を追加
	effectRes_.emplace(_name, _data);
}

void EffectManager::Play(const std::string _master,const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed, const std::string& _sndName)
{
	//元データがないときは警告
	if (effectRes_.find(_name) == effectRes_.end())assert("設定していないエフェクトを再生しようとしています。");

	//要素の追加
	int addEfc = PlayEffekseer3DEffect(effectRes_[_name]);
	effectPlay_[_master][_name].push_back(addEfc);
	

	//各種設定同期
	SyncEffect(_master, _name, _pos, _qua, _size, _speed, static_cast<int>(effectPlay_[_master][_name].size()) - 1);

	//効果音の再生
		if (_sndName != "") {
			SoundManager::GetInstance().Play(_sndName);
		}
}

void EffectManager::Stop(const std::string _master,const std::string& _name)
{
	//配列内に入っていないものを停止しようとしたら警告
	if (effectPlay_.find(_master) == effectPlay_.end())assert("設定していないエフェクトを停止しようとしています。");
	//再生停止
	for (auto& data : effectPlay_[_master][_name]) {
		StopEffekseer3DEffect(data);
	}
}

void EffectManager::SyncEffect(const std::string _master,const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed,const int _idx)
{
	//その他各種設定
	//大きさ
	SetScalePlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _size, _size, _size);
	//角度
	SetRotationPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//位置
	SetPosPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _pos.x, _pos.y, _pos.z);
	//速度
	SetSpeedPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _speed);
}

void EffectManager::Update(void)
{
	//削除項目記憶用
	std::map<std::string, std::map<std::string, std::vector<int>>>deleteField = {};//<所有者・止めるエフェクト詳細>
	std::map<std::string, std::vector<int>>deleteIdx = {};	//止めるエフェクト詳細<エフェクト名/停止する番号>

	//master=first:所有者/second:止めるエフェクト詳細
	for (auto& master : effectPlay_) {
		//data=first:エフェクト名/second:再生データ
		for (auto& data : master.second) {
			//動的配列カウンタ用
			int cnt = 0;
			//再生データ個数分
			for (auto& efc : data.second) {
				if (IsEffekseer3DEffectPlaying(efc) == -1) {
					deleteIdx[data.first].push_back(cnt);
				}
				//カウンタ増加
				cnt++;
			}
		}
		//所有者と削除データを保存
		deleteField.emplace(master.first, deleteIdx);
	}

	//削除
	//master=first:所有者/second:止めるエフェクト詳細
	for (auto& master : deleteField) {
		//idx=first:エフェクト名/second:停止番号
		for (auto& idx : master.second) {
			//削除
			effectPlay_[master.first][idx.first];
		}
	}
}

void EffectManager::Release(void)
{
	//配列内の要素を全て消去
	//元々のデータはリソースマネージャが持っているのでおｋ。
	effectRes_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	delete instance_;
}
