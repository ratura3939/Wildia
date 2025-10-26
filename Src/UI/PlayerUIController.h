//プレイヤーキャラクターに付随するもの
#pragma once
#include "CharacterUIController.h"
#include<memory>
#include <DxLib.h>
class PlayerHp;
class NoticeDodge;

class PlayerUIController :
    public CharacterUIController
{
public:
    enum class PlayerUI {
        HP,
        ALLERT,
    };

    PlayerUIController(VECTOR& _followPos,const int _hp);
    ~PlayerUIController(void)override;

    void Init(const std::string& _master)override;
    void Update(void)override;
    void Draw(void);
    void Release(void)override;

    void SetDrawPos(const VECTOR _pos)override;
    void ChangeAllert(const bool _flag) { isAllert_ = _flag; }

    void Damage(void);
private:
    std::unique_ptr<PlayerHp>hp_;
    std::unique_ptr<NoticeDodge>dodge_;

    bool isAllert_;

    VECTOR hpDrawPos_;
};

