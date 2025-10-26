#include<cassert>
#include<map>
#include"../../Object/Attack/Arrow.h"
#include"../../Object/Character/CharacterBase.h"
#include"../Decoration/SoundManager.h"

#include "AttackManager.h"


void AttackManager::AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _copy, const bool _friendFire,
	const float _total, const float _start, const float _end,const int _modelId)
{
	//���łɗv�f������Ƃ�
	if (attackInfoes_.contains(_name)) {
		//�G���[�h�~
		assert("���łɓo�^���Ă�����̂��ēo�^���悤�Ƃ��Ă��܂�");
		return;
	}

	//�ݒ肳�ꂽ��񂩂�U���𐶐�
	AttackInfo info = {};
	info.group = ATTACK_MASTER::NONE;
	info.type = _type;
	info.isCopy = _copy;

	info.isFriendFire = _friendFire;

	info.totalMotion = _total;
	info.startAttack = _start;
	info.endAttack = _end;
	info.counter = 0.0f;
	info.isHit = false;
	info.isAllert = false;

	//�U������ǉ�
	attackInfoes_.emplace(_name, info);
}

void AttackManager::Attack(const std::string _master, const std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _group, const float _scale, const std::string _sndName, const int _arrowModel)
{
	//���������g�p�������U�����o�^����Ă��Ȃ��Ƃ�
	if (!attackInfoes_.contains(_name)) {
		//�G���[�h�~
		assert("�o�^����Ă��Ȃ��U���𔭐������悤�Ƃ��Ă��܂�");
		return;
	}

	//�R�s�[�s�ȍU�����A���łɍU�����̂Ȃ��ɗv�f������Ƃ�
	if (activeAttacks_[_master].contains(_name) && !attackInfoes_[_name].isCopy) {
		//������������̂ŏ������Ȃ�
		return;
	}

	//�ǉ�����U���̍쐬
	//���
	attackInfoes_[_name].scale = _scale;
	attackInfoes_[_name].group = _group;
	attackInfoes_[_name].isAllert = false;
	//�U����
	AttackItself addAtk = { _pos,_pow };
	//���킹������
	AttackCollision addCol = { attackInfoes_[_name],addAtk };


	//�U������̐���**
	//���̏ꍇ
	if (attackInfoes_[_name].type == ATTACK_TYPE::SWORD) {
		//�ǉ��v
		activeAttacks_[_master].emplace(_name, addCol);
	}
	//�|�̏ꍇ
	else if (attackInfoes_[_name].type == ATTACK_TYPE::BOW) {
		//�O�̂��߂̗\�h��
		if (_arrowModel == -1) {
			assert("�|�̃��f�����ݒ肳��Ă��܂���");
		}
		//�|��̍쐬
		//arrows_.push_back(std::make_unique<Arrow>(addInfo.master, _arrowModel, _pos, _pow, _qua));
	}

	//�����Đ����镨������ꍇ
	if (_sndName != "") {
		//���ʉ��̍Đ�
		SoundManager::GetInstance().Play(_sndName);
	}
	
}

bool AttackManager::Update(void)
{
	//�폜���ڋL���p
	std::map<std::string, std::vector<std::string>>deleteAtkName;

	//�U���X�V�����i�U�������Ґ����j
	//master=first:�U�������Җ� second:�U���f�[�^
	for (auto& master : activeAttacks_) {
		//atkData=first:�U����ޖ��@second:�U�����
		for (auto& atkData : master.second) {
			auto data = atkData.second;

			AttackInfo& info = data.info;
			//�J�E���^�[��������ゾ������
			if (info.counter >= info.totalMotion) {
				//�I��
				info.counter = 0;
				info.isHit = false;
				//�폜���ڂɒǉ�
				//������(atkAll.first)�̂��̍U��(atkData.first)���폜����ƋL��
				deleteAtkName[master.first].push_back(atkData.first);
				continue;
			}
			//�J�E���^�[�̍X�V
			info.counter++;
		}
	}

	//�폜
	for (auto& master : deleteAtkName) {
		for (auto& atk : master.second) {
			activeAttacks_[master.first].erase(atk);
		}
		if (activeAttacks_[master.first].size() <= 0) {
			activeAttacks_.erase(master.first);
		}
	}

	return true;
}

std::vector<AttackManager::AttackCollision> AttackManager::GetActiveAttacks(void)
{
	std::vector<AttackCollision>retVector;
	//�U���̍��ڐ���
	for (auto& master : activeAttacks_) {
		//���̍��ږ����g���Ă���U������
		for (auto& atkData : master.second) {
			//��������
			retVector.push_back(atkData.second);
		}
	}

	return retVector;
}

const float AttackManager::GetTotalTime(const std::string _name) const
{
	//�v�f���Ȃ��Ƃ�
	if (!attackInfoes_.contains(_name)) {
		return -1.0f;
	}
	return attackInfoes_.at(_name).totalMotion;
}

void AttackManager::DrawDebug(void)
{
	int color = 0xff00ff;
	for (auto& master : activeAttacks_) {
		for (auto& atkData : master.second) {
			auto& info = attackInfoes_[atkData.first];
			if (info.group == ATTACK_MASTER::ENEMY) {
				if (info.counter < info.startAttack) {
					color = 0x00ff00;
				}
				else if (info.counter >= info.endAttack) {
					color = 0x0000ff;
				}
				else color = 0xff00ff;
			}
			//�f�o�b�O�p�̋��̂�`��
			DrawSphere3D(atkData.second.attack.pos, info.scale, 8, color, color, false);
		}
	}
}
