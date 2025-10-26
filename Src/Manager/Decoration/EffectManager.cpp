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
	//�A�z�z����ɂ��łɗv�f�������Ă��邩������
	//�����Ă����珈���I��
	if (effectRes_.find(_name) != effectRes_.end())return;

	//�V�K�f�[�^�̂��ߏ���ǉ�
	effectRes_.emplace(_name, _data);
}

void EffectManager::Play(const std::string _master,const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed, const std::string& _sndName)
{
	//���f�[�^���Ȃ��Ƃ��͌x��
	if (effectRes_.find(_name) == effectRes_.end())assert("�ݒ肵�Ă��Ȃ��G�t�F�N�g���Đ����悤�Ƃ��Ă��܂��B");

	//�v�f�̒ǉ�
	int addEfc = PlayEffekseer3DEffect(effectRes_[_name]);
	effectPlay_[_master][_name].push_back(addEfc);
	

	//�e��ݒ蓯��
	SyncEffect(_master, _name, _pos, _qua, _size, _speed, static_cast<int>(effectPlay_[_master][_name].size()) - 1);

	//���ʉ��̍Đ�
		if (_sndName != "") {
			SoundManager::GetInstance().Play(_sndName);
		}
}

void EffectManager::Stop(const std::string _master,const std::string& _name)
{
	//�z����ɓ����Ă��Ȃ����̂��~���悤�Ƃ�����x��
	if (effectPlay_.find(_master) == effectPlay_.end())assert("�ݒ肵�Ă��Ȃ��G�t�F�N�g���~���悤�Ƃ��Ă��܂��B");
	//�Đ���~
	for (auto& data : effectPlay_[_master][_name]) {
		StopEffekseer3DEffect(data);
	}
}

void EffectManager::SyncEffect(const std::string _master,const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed,const int _idx)
{
	//���̑��e��ݒ�
	//�傫��
	SetScalePlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _size, _size, _size);
	//�p�x
	SetRotationPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//�ʒu
	SetPosPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _pos.x, _pos.y, _pos.z);
	//���x
	SetSpeedPlayingEffekseer3DEffect(effectPlay_[_master][_name][_idx], _speed);
}

void EffectManager::Update(void)
{
	//�폜���ڋL���p
	std::map<std::string, std::map<std::string, std::vector<int>>>deleteField = {};//<���L�ҁE�~�߂�G�t�F�N�g�ڍ�>
	std::map<std::string, std::vector<int>>deleteIdx = {};	//�~�߂�G�t�F�N�g�ڍ�<�G�t�F�N�g��/��~����ԍ�>

	//master=first:���L��/second:�~�߂�G�t�F�N�g�ڍ�
	for (auto& master : effectPlay_) {
		//data=first:�G�t�F�N�g��/second:�Đ��f�[�^
		for (auto& data : master.second) {
			//���I�z��J�E���^�p
			int cnt = 0;
			//�Đ��f�[�^����
			for (auto& efc : data.second) {
				if (IsEffekseer3DEffectPlaying(efc) == -1) {
					deleteIdx[data.first].push_back(cnt);
				}
				//�J�E���^����
				cnt++;
			}
		}
		//���L�҂ƍ폜�f�[�^��ۑ�
		deleteField.emplace(master.first, deleteIdx);
	}

	//�폜
	//master=first:���L��/second:�~�߂�G�t�F�N�g�ڍ�
	for (auto& master : deleteField) {
		//idx=first:�G�t�F�N�g��/second:��~�ԍ�
		for (auto& idx : master.second) {
			//�폜
			effectPlay_[master.first][idx.first];
		}
	}
}

void EffectManager::Release(void)
{
	//�z����̗v�f��S�ď���
	//���X�̃f�[�^�̓��\�[�X�}�l�[�W���������Ă���̂ł����B
	effectRes_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	delete instance_;
}
