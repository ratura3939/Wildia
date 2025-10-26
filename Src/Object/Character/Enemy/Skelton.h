#pragma once
#include "EnemyBase.h"
class Skelton :
    public EnemyBase
{
public:
    Skelton(VECTOR& _pos);
    ~Skelton(void)override;

private:
    void InitAnim(void)override;
    void SetPram(void)override;
};

