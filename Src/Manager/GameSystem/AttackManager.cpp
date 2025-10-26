#include<cassert>
#include<map>
#include"../../Object/Attack/Arrow.h"
#include"../../Object/Character/CharacterBase.h"
#include"../Decoration/SoundManager.h"

#include "AttackManager.h"


void AttackManager::AddAttack(const std::string _name, const ATTACK_TYPE& _type, const bool _copy, const bool _friendFire,
	const float _total, const float _start, const float _end,const int _modelId)
{
	//‚·‚Å‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if (attackInfoes_.contains(_name)) {
		//ƒGƒ‰[–h~
		assert("‚·‚Å‚É“o˜^‚µ‚Ä‚¢‚é‚à‚Ì‚ğÄ“o˜^‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//İ’è‚³‚ê‚½î•ñ‚©‚çUŒ‚‚ğ¶¬
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

	//UŒ‚î•ñ‚ğ’Ç‰Á
	attackInfoes_.emplace(_name, info);
}

void AttackManager::Attack(const std::string _master, const std::string _name, const float _pow, const VECTOR& _pos, const Quaternion& _qua, const ATTACK_MASTER _group, const float _scale, const std::string _sndName, const int _arrowModel)
{
	//‚»‚à‚»‚àg—p‚µ‚½‚¢UŒ‚‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚«
	if (!attackInfoes_.contains(_name)) {
		//ƒGƒ‰[–h~
		assert("“o˜^‚³‚ê‚Ä‚¢‚È‚¢UŒ‚‚ğ”­¶‚³‚¹‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//ƒRƒs[•s‰Â‚ÈUŒ‚‚ªA‚·‚Å‚ÉUŒ‚’†‚Ì‚È‚©‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if (activeAttacks_[_master].contains(_name) && !attackInfoes_[_name].isCopy) {
		//‚»‚à‚»‚à‚ ‚é‚Ì‚Åˆ—‚µ‚È‚¢
		return;
	}

	//’Ç‰Á‚·‚éUŒ‚‚Ìì¬
	//î•ñ•”
	attackInfoes_[_name].scale = _scale;
	attackInfoes_[_name].group = _group;
	attackInfoes_[_name].isAllert = false;
	//UŒ‚•”
	AttackItself addAtk = { _pos,_pow };
	//‡‚í‚¹‚½‚à‚Ì
	AttackCollision addCol = { attackInfoes_[_name],addAtk };


	//UŒ‚”»’è‚Ì¶¬**
	//Œ•‚Ìê‡
	if (attackInfoes_[_name].type == ATTACK_TYPE::SWORD) {
		//’Ç‰Áv
		activeAttacks_[_master].emplace(_name, addCol);
	}
	//‹|‚Ìê‡
	else if (attackInfoes_[_name].type == ATTACK_TYPE::BOW) {
		//”O‚Ì‚½‚ß‚Ì—\–hô
		if (_arrowModel == -1) {
			assert("‹|‚Ìƒ‚ƒfƒ‹‚ªİ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
		//‹|–î‚Ìì¬
		//arrows_.push_back(std::make_unique<Arrow>(addInfo.master, _arrowModel, _pos, _pow, _qua));
	}

	//‰½‚©Ä¶‚·‚é•¨‚ª‚ ‚éê‡
	if (_sndName != "") {
		//Œø‰Ê‰¹‚ÌÄ¶
		SoundManager::GetInstance().Play(_sndName);
	}
	
}

bool AttackManager::Update(void)
{
	//íœ€–Ú‹L‰¯—p
	std::map<std::string, std::vector<std::string>>deleteAtkName;

	//UŒ‚XVˆ—iUŒ‚”­¶Ò”•ªj
	//master=first:UŒ‚”­¶Ò–¼ second:UŒ‚ƒf[ƒ^
	for (auto& master : activeAttacks_) {
		//atkData=first:UŒ‚í—Ş–¼@second:UŒ‚î•ñ
		for (auto& atkData : master.second) {
			auto data = atkData.second;

			AttackInfo& info = data.info;
			//ƒJƒEƒ“ƒ^[‚ªãŒÀ‚æ‚èã‚¾‚Á‚½‚ç
			if (info.counter >= info.totalMotion) {
				//I—¹
				info.counter = 0;
				info.isHit = false;
				//íœ€–Ú‚É’Ç‰Á
				//”­¶Ò(atkAll.first)‚Ì‚±‚ÌUŒ‚(atkData.first)‚ğíœ‚·‚é‚Æ‹L‰¯
				deleteAtkName[master.first].push_back(atkData.first);
				continue;
			}
			//ƒJƒEƒ“ƒ^[‚ÌXV
			info.counter++;
		}
	}

	//íœ
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
	//UŒ‚‚Ì€–Ú”•ª
	for (auto& master : activeAttacks_) {
		//‚»‚Ì€–Ú–¼‚ªg‚í‚ê‚Ä‚¢‚éUŒ‚”•ª
		for (auto& atkData : master.second) {
			//î•ñ‚ğ“ü‚ê‚é
			retVector.push_back(atkData.second);
		}
	}

	return retVector;
}

const float AttackManager::GetTotalTime(const std::string _name) const
{
	//—v‘f‚ª‚È‚¢‚Æ‚«
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
			//ƒfƒoƒbƒO—p‚Ì‹…‘Ì‚ğ•`‰æ
			DrawSphere3D(atkData.second.attack.pos, info.scale, 8, color, color, false);
		}
	}
}
