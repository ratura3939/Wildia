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
	//�A�z�z����ɂ��łɗv�f�������Ă��邩������
	//�����Ă����珈���I��
	if (sounds_.find(_name) != sounds_.end())return;


	//�Đ�����Ƃ��f�[�^�̎�ނɂ����
	//���[�v�Đ����P�����𔻒f����
	int mode = -1;
	if (_type == TYPE::BGM) mode = DX_PLAYTYPE_LOOP;
	else mode = DX_PLAYTYPE_BACK;

	//�V�K�f�[�^�̂��ߏ���ǉ�
	sounds_.emplace(_name, SOUND_DATA{ _data,_type,mode});
	intervales_[_name] = _interval;
}


void SoundManager::Play(const std::string _name)
{
	//���f�[�^���Ȃ��Ƃ��͌x��
	if (sounds_.find(_name) == sounds_.end())assert("�ݒ肵�Ă��Ȃ��������Đ����悤�Ƃ��Ă��܂��B");

	//�������Ƃ��Ă���̂�BGM�̂Ƃ�
	if (sounds_[_name].type == TYPE::BGM) {
		if (activeBgm_ == _name) {
			//����BGM���Đ����悤�Ƃ��Ă���̂�
			return;
		}
		//�����Ă���BGM�̍X�V
		activeBgm_ = _name;
	}

	//�J�E���^�[�ɗv�f���Ȃ��Ƃ�
	//�J�E���^�[�ɗv�f���Ȃ��Ƃ������ݍĐ��\��
	if(!counteres_.contains(_name)) {
		//�Đ�����
		PlaySoundMem(sounds_[_name].data, sounds_[_name].playMode);
		//�C���^�[�o��������ꍇ�J�E���^�𐶐�
		if (intervales_[_name] > 0) {
			counteres_.emplace(_name, 0);
		}
	}
	
}


void SoundManager::Stop(const std::string _name)
{
	//���f�[�^���Ȃ��Ƃ��͌x��
	if (sounds_.find(_name) == sounds_.end())assert("�ݒ肵�Ă��Ȃ��������~���悤�Ƃ��Ă��܂��B");

	//��~����
	StopSoundMem(sounds_[_name].data);
}

void SoundManager::Update(void)
{
	//�J�E���^�����ۑ��p
	std::vector<std::string>delteNames;

	//�J�E���^�X�V
	for (auto& counter : counteres_) {
		counter.second++;
		//�C���^�[�o���𖞗�������
		if (counter.second >= intervales_[counter.first]) {
			//�폜�\��ɓ����
			delteNames.push_back(counter.first);
		}
	}

	for (auto& name : delteNames) {
		//�Y���̃J�E���^������
		counteres_.erase(name);
	}
}

void SoundManager::Release(void)
{
	//�z��v�f�P�폜
	sounds_.clear();
	activeBgm_ = "";
}


void SoundManager::AdjustVolume(const std::string _name, const int _persent)
{
	//���f�[�^���Ȃ��Ƃ��͌x��
	if (sounds_.find(_name) == sounds_.end())assert("�ݒ肵�Ă��Ȃ�������ݒ肵�悤�Ƃ��Ă��܂��B");
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
