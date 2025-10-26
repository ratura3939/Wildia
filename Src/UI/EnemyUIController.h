//敵キャラクターに付随するもの
#pragma once
#include "CharacterUIController.h"
#include<memory>
#include<DxLib.h>

#include"../Object/Character/Enemy/EnemyBase.h"
#include"Enemy/EnemyFind.h"
#include"Enemy/EnemyHp.h"
#include"Enemy/EnemyTargetting.h"

class EnemyUIController :
    public CharacterUIController
{
public:
    enum class ENEMY_UI {
        HP,
        TARGETTING,
        FIND,
    };

    EnemyUIController(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state);
    ~EnemyUIController(void)override;

    void Init(const std::string& _master)override;
    void CreateUI(const std::string& _master,float& hp, float hpMax);
    void Update(void)override;
    void Draw(const ENEMY_UI _type);
    void Release(void)override;

    void SetDrawPos(const VECTOR _pos)override;

    void FindReset(void);

    /// <summary>
    /// ロックオンUI切り換え
    /// </summary>
    /// <param name="_flag">true=ロックオン中UI/false=ロックオン予定UI</param>
    void ChangeTargetUI(const bool _flag);

private:

    std::unique_ptr<EnemyFind>findUI_;
    std::unique_ptr<EnemyHp>hpUI_;
    std::unique_ptr<EnemyTargetting>targetUI_;

    EnemyBase::ENEMY_STATE& eState_;
};

