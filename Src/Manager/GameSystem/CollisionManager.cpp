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
	//敵の攻撃には前隙・後隙があるのでそれらの判定も込みで行う

	const VECTOR pPos = _player.lock()->GetPos();		//プレイヤー位置(足元)
	const VECTOR pHeadPos = _player.lock()->GetHeight();//プレイヤー位置(頭上)
	VECTOR efcPos = pHeadPos;							//エフェクト位置(胴体)
	efcPos.y /= 2.0f;

	const std::string name = _player.lock()->GetSpeciesName();	//個体名

	//攻撃の数だけ回す
	for (auto& atkCol : _atks) {
		//攻撃が判定済み・同陣営の攻撃だった場合
		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::PLAYER) {
			//次へ
			continue;
		}

		//これ以降は攻撃の判定が可能な状態
		//攻撃位置・大きさ
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		const float DmgEfcScl = 25.0f;
		const float DmgEfcSpeed = 2.5;
		

		//攻撃(球)とキャラクター(カプセル)の当たり判定
		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, _player.lock()->GetCollisionRadius())) {
			
			//回避可能時間に当たっていたら
			if (atkCol.info.IsPreGap()) {

				//ジャスト回避できるよ！！という予測を出すようにする。
				if (!atkCol.info.isAllert) {
					_player.lock()->SetAtkAllert();
					SoundManager::GetInstance().Play("Allert");
					atkCol.info.isAllert = true;
				}

				//回避していたら
				if (_player.lock()->GetState() == PlayerChara::STATE::DODGE) {
					//スローに
					isSlow_ = true;
					//ジャスト回避の効果音流す
					SoundManager::GetInstance().Play("JustDodge");
					//判定済みに
					atkCol.info.isHit = true;
				}
				continue;
			}

			//発生時間中に当たっていたら
			if (atkCol.info.IsOuccerAttack()) {
				//ダメージ
				_player.lock()->Damage(atkCol.attack.pow);
				//ダメージエフェクト・SEの再生
				EffectManager::GetInstance().Play(name,"Damage", efcPos, _player.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
				//判定済みに
				atkCol.info.isHit = true;
			}
		}
	}


	//テキトーな移動制限
	if (Utility::MagnitudeF(pPos) > 5000.0f) {
		_player.lock()->SetPrePos();
	}
}

void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{

	//攻撃の数だけ回す
	for (auto& atkCol : _atks) {
		//攻撃がそもそも判定済み・同属の攻撃だった場合
		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::ENEMY) {
			//次へ
			continue;
		}

		//これ以降は攻撃の判定が可能な状態
		//攻撃位置
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		const float DmgEfcScl = 25.0f;
		const float DmgEfcSpeed = 2.5;
		const float SwordEfcScl = 50.0f;
		const float SwordEfcSpeed = 1.5;


		//敵の数だけ回す
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.lock()->GetPos();
			const VECTOR eHeadPos = enemy.lock()->GetHeight();
			//エフェクトの再生位置は胴体ぐらいの想定なので身長の半分くらい
			VECTOR eEfcpos = eHeadPos;
			eEfcpos.y /= 2.0f;

			const std::string name = enemy.lock()->GetSpeciesName();

			//攻撃(球)とキャラクター(カプセル)の当たり判定
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, enemy.lock()->GetCollisionRadius())) {
				//当たっていたら
				enemy.lock()->Damage(atkCol.attack.pow);
				//ダメージエフェクト・SEの再生
				EffectManager::GetInstance().Play(name,"Damage", eEfcpos, enemy.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
				//武器エフェクトの再生(現在は斬撃だけなので確定でこれを流すようになってる)
				EffectManager::GetInstance().Play(name, "Sword", eEfcpos, enemy.lock()->GetQua(), SwordEfcScl, SwordEfcSpeed);
				//判定済みに
				atkCol.info.isHit = true;
			}
			//テキトーな移動制限
			if (Utility::MagnitudeF(ePos) > 5000.0f) {
				enemy.lock()->SetPrePos();
			}
		}
	}
}
