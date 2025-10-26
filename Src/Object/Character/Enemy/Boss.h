#pragma once
#include "EnemyBase.h"
class Boss :
    public EnemyBase
{
public:
    Boss(VECTOR& _pos);
    ~Boss(void)override;

    void Shout(void)override;

private:
    void InitAnim(void)override;
    void SetPram(void)override;

    void UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)override; //êÌì¨
    void MoveBattle(const VECTOR& _pPos)override;

    void DrawUI(void)override;
};

