#pragma once
#include <DxLib.h>
#include<memory>
#include "../CharacterBase.h"
#include"../../../UI/PlayerUIController.h"

//プレイヤーはスローの影響を受けない
//よって更新に関わるカウンターは通常のインクリメント

class Game;

class PlayerChara :
    public CharacterBase
{
public:
    /// <summary>
    /// ロックオン時状態にあるかどうか
    /// </summary>
    enum class ROCK_STATE {
        NOMAL,  //通常
        LOCKON, //ロックオン
        MAX
    };

    /// <summary>
    /// プレイヤー行動状態
    /// </summary>
    enum class STATE {
        NOMAL,
        GUARD,
        DODGE,
        JUST_DODGE,
        ATTACK,
    };

    /// <summary>
    /// 移動方向
    /// </summary>
    enum class MOVE_DIR {
        NONE,
        FORWARD,
        LEFT,
        RIGHT,
        BACK
    };

    PlayerChara(void);
    ~PlayerChara(void)override;

    const bool Init(const int _num)override;
    void Update(void)override;

    //注視点の取得
    const VECTOR GetFocusPoint(void)const;

    /// <summary>
    /// ロックオン関係の状態変化
    /// </summary>
    /// <param name="_state">ture=ロックオン,false=ロックオフ</param>
    void ChangeRockState(const bool _state);

    //状態関係
    const STATE GetState(void)const;    //取得
    void SetState(const STATE& _state); //設定(PlayerManagerからの受付)
    void PlayAnim(const std::string _anim); //アニメーションの再生(外部<マネージャ・当たり判定>より)
    const bool IsRock(void);
    void Damage(const float _pow)override;

    void DrawDebug(void)override;

    //入力受付
    void InputDash(const bool _flag) { isDush_ = _flag; }
    void InputMoveDir(const MOVE_DIR _dir) { moveDir_ = _dir; }

    float GetToLockDeg(void);   //ロックオン角度

    void SetAtkAllert(void);    //攻撃危機察知

private:
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Move(void);    //移動処理
    const std::string DecideAnim(const MOVE_DIR _dir)const;

    ROCK_STATE rState_; //ロックオン状態
    STATE state_;       //状態

    VECTOR focusPoint_; //注視点

    //入力受付変数
    MOVE_DIR moveDir_;  //移動方向
    bool isDush_;        //走る

    std::unique_ptr<PlayerUIController>uiCntl_; //UI

    //UI関連
    int allertTime_;
};

