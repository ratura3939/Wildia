#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../../Object/Character/CharacterBase.h"
#include"../../Utility/Utility.h"
#include"../Decoration/SoundManager.h"
#include"../Generic/ResourceManager.h"
#include"../Decoration/EffectManager.h"

#include "CollisionManager.h"

using atkM = AttackManager;

CollisionManager::CollisionManager(void)
{
	isSlow_ = false;
}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{
	isSlow_ = false;
	CollisionPlayer(_player, _atks);
	CollisionEnemy(_enemy, _atks);

	return isSlow_;
}

void CollisionManager::CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks)
{
	//�G�̍U���ɂ͑O���E�㌄������̂ł����̔�������݂ōs��

	const VECTOR pPos = _player.lock()->GetPos();		//�v���C���[�ʒu(����)
	const VECTOR pHeadPos = _player.lock()->GetHeight();//�v���C���[�ʒu(����)
	VECTOR efcPos = pHeadPos;							//�G�t�F�N�g�ʒu(����)
	efcPos.y /= 2.0f;

	const std::string name = _player.lock()->GetSpeciesName();	//�̖�

	//�U���̐�������
	for (auto& atkCol : _atks) {
		//�U��������ς݁E���w�c�̍U���������ꍇ
		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::PLAYER) {
			//����
			continue;
		}

		//����ȍ~�͍U���̔��肪�\�ȏ��
		//�U���ʒu�E�傫��
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		const float DmgEfcScl = 25.0f;
		const float DmgEfcSpeed = 2.5;
		

		//�U��(��)�ƃL�����N�^�[(�J�v�Z��)�̓����蔻��
		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, _player.lock()->GetCollisionRadius())) {
			
			//����\���Ԃɓ������Ă�����
			if (atkCol.info.IsPreGap()) {

				//�W���X�g����ł����I�I�Ƃ����\�����o���悤�ɂ���B
				if (!atkCol.info.isAllert) {
					_player.lock()->SetAtkAllert();
					SoundManager::GetInstance().Play("Allert");
					atkCol.info.isAllert = true;
				}

				//������Ă�����
				if (_player.lock()->GetState() == PlayerChara::STATE::DODGE) {
					//�X���[��
					isSlow_ = true;
					//�W���X�g����̌��ʉ�����
					SoundManager::GetInstance().Play("JustDodge");
					//����ς݂�
					atkCol.info.isHit = true;
				}
				continue;
			}

			//�������Ԓ��ɓ������Ă�����
			if (atkCol.info.IsOuccerAttack()) {
				//�_���[�W
				_player.lock()->Damage(atkCol.attack.pow);
				//�_���[�W�G�t�F�N�g�ESE�̍Đ�
				EffectManager::GetInstance().Play(name,"Damage", efcPos, _player.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
				//����ς݂�
				atkCol.info.isHit = true;
			}
		}
	}


	//�e�L�g�[�Ȉړ�����
	if (Utility::MagnitudeF(pPos) > 5000.0f) {
		_player.lock()->SetPrePos();
	}
}

void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{

	//�U���̐�������
	for (auto& atkCol : _atks) {
		//�U����������������ς݁E�����̍U���������ꍇ
		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::ENEMY) {
			//����
			continue;
		}

		//����ȍ~�͍U���̔��肪�\�ȏ��
		//�U���ʒu
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		const float DmgEfcScl = 25.0f;
		const float DmgEfcSpeed = 2.5;
		const float SwordEfcScl = 50.0f;
		const float SwordEfcSpeed = 1.5;


		//�G�̐�������
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.lock()->GetPos();
			const VECTOR eHeadPos = enemy.lock()->GetHeight();
			//�G�t�F�N�g�̍Đ��ʒu�͓��̂��炢�̑z��Ȃ̂Őg���̔������炢
			VECTOR eEfcpos = eHeadPos;
			eEfcpos.y /= 2.0f;

			const std::string name = enemy.lock()->GetSpeciesName();

			//�U��(��)�ƃL�����N�^�[(�J�v�Z��)�̓����蔻��
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, enemy.lock()->GetCollisionRadius())) {
				//�������Ă�����
				enemy.lock()->Damage(atkCol.attack.pow);
				//�_���[�W�G�t�F�N�g�ESE�̍Đ�
				EffectManager::GetInstance().Play(name,"Damage", eEfcpos, enemy.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
				//����G�t�F�N�g�̍Đ�(���݂͎a�������Ȃ̂Ŋm��ł���𗬂��悤�ɂȂ��Ă�)
				EffectManager::GetInstance().Play(name, "Sword", eEfcpos, enemy.lock()->GetQua(), SwordEfcScl, SwordEfcSpeed);
				//����ς݂�
				atkCol.info.isHit = true;
			}
			//�e�L�g�[�Ȉړ�����
			if (Utility::MagnitudeF(ePos) > 5000.0f) {
				enemy.lock()->SetPrePos();
			}
		}
	}
}
