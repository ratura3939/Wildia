#pragma once
#include "../CharacterBase.h"

//敵はスローの影響を受ける
//よって更新に関わるカウンターはSceneManagerのupdateSpeedRateを参照する

class Game;
class EnemyUIController;

class EnemyBase :
    public CharacterBase
{
public:
#pragma region ステータス関連

    //その他パラメータ類
    static constexpr float MOVE_POW = 3.0f;     //移動量
    static constexpr float MOVE_POW_FIND = 5.0f;//移動量(発見時)
    static constexpr float CHARA_SCALE = 0.7f;  //サイズ
    static constexpr float ENEMY_HP = 70.0f;

#pragma endregion

#pragma region 状態ごと関連

      //状態遷移に関わるもの
    static constexpr float FIELD_VISION_DEG_HALF = 40.0f;	//視界の角度(両方向に展開するので全体の半分の角度を明記)
    static constexpr float FIELD_VISION_DISTANCE = 1000.0f;	//視界の距離
    static constexpr float ALERT_DISTANCE = 3000.0f;     	//警戒の距離
    static constexpr float ATTACK_DISTANCE = 200.0f;     	//攻撃開始の距離
    static constexpr float BATTLE_FINISH_DISTANCE = 1500.0f; //攻撃状態解除の距離


    static constexpr float SEARCH_RESTART_TIME = 100.0f;  //警戒再開時間
    static constexpr float SEARCH_CNT_MAX = 60.0f;  //サーチ完了時間

    //乱数移動量
    static constexpr float MOVE_RANDOM_MIN = 200.0f;                   //最低値
    static constexpr float MOVE_RANDOM_MAX = 600.0f - MOVE_RANDOM_MIN;;//最高値(実際の計算で採算を合わせるためこのようになる。実際の最大値から最小値を引く)

    static constexpr float STAY_TIME = 200.0f;   //ステイの時間
    static constexpr VECTOR SCALE_DOWN = { 0.01f,0.01f,0.01f };

#pragma endregion

    static constexpr float SPEED_ANIM = 1.0f;  //アニメーションスピード

    //攻撃関連(外部ファイル化させる)
    static constexpr VECTOR RELATIVE_ATTACK_POS = { 0.0f, 75.0f, 100.0f };
    static constexpr float SCALE_ATTACK_NOMAL = 70.0f;
    static constexpr float POW_ATTACK_NOMAL = 1.0f;
    static constexpr float INTERVAL_ATTACK_NOMAL = 150.0f;

    /// <summary>
    /// 敵の状態
    /// </summary>
    enum class ENEMY_STATE {
        NOMAL,
        SEARCH,
        BATTLE,
        DETH,
        END
    };
    /// <summary>
    /// 視界判定
    /// </summary>
    struct Vision {
        VECTOR center;
        VECTOR left;
        VECTOR right;
        bool isFind;
    };

    EnemyBase(VECTOR& _pos);
    ~EnemyBase(void)override;

    const bool Init(const int _num)override;
    void Update(const VECTOR _pPos, AttackManager& _atk)override;
    void Draw(void)override;

    //生存判定
    const bool IsAlive(void)const override;
    const ENEMY_STATE GetNowState(void)const { return state_; }
    /// <summary>
    /// アニメーションのスピード変更
    /// </summary>
    /// <param name="_percent">パーセンテージ</param>
    const void SetAnimSpeedRate(const float _percent);

    void SetIsLockTraget(const bool _flag) { isLockTarget_ = _flag; }
    void SetIsLocked(const bool _flag);

    //ダメージ
    void Damage(const float _pow)override;
    //死亡させる
    void Deth(void)override;

    virtual void Shout(void);//ボス専用

     //デバッグ用
    void SetColor(int _color);
    void SetPos(VECTOR _pos);
    void DrawDebug(void)override;
  
protected:
    virtual void SetPram(void) = 0;    //各敵の固有情報
    virtual void InitAnim(void)override;
    void InitUI(void)override;

#pragma region 各種状態更新

    //共通の基礎処理
    //特有の処理がある場合overrideする
    virtual void UpdateNomal(const VECTOR& _pPos, AttackManager& _atk);  //通常
    virtual void UpdateSearch(const VECTOR& _pPos, AttackManager& _atk); //索敵
    virtual void UpdateBattle(const VECTOR& _pPos, AttackManager& _atk); //戦闘
    void UpdateDeth(const VECTOR& _pPos, AttackManager& _atk); //死亡

    //各種移動処理
    void MoveNomal(const VECTOR& _pPos);
    void MoveSearch(const VECTOR& _pPos);
    virtual void MoveBattle(const VECTOR& _pPos);

    void OderGoalRot(const VECTOR _pPos);   //プレイヤーまでの角度

    void ChangeState(const ENEMY_STATE _state); //状態の遷移

#pragma endregion

    virtual void DrawUI(void)override;

    using Update_f = void(EnemyBase::*)(const VECTOR& _pPos, AttackManager& _atk);
    using Move_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //更新関数
    Move_f move_;       //移動関数

    std::unique_ptr<EnemyUIController>uiCntl_;
    float maxHp_;

    VECTOR preStayPos_; //前回停止位置
    float uiDeviationY_;//UI位置調整
    float moveOneTime_; //一回の移動量
    float moveSped_;    //１フレームでの移動量
    bool isStay_;       //ステイかどうか
    float stayCnt_;     //ステイ状態のカウンタ
    float stopTime_;    //攻撃時の停止時間
    float intervalCnt_; //攻撃間隔のカウンタ

    float searchRestartCnt_;    //疑い再開カウンタ
    float searchCnt_;   //疑いカウンタ

    bool isAlive_;     //削除していいか
    ENEMY_STATE state_;//状態

    bool isLockTarget_;   //ロックオン対象になっているか(マネージャでのみ変更が可能)

    VECTOR atkRelative_;    //攻撃の発生位置の相対座標
    float atkScale_;    //攻撃の大きさ
    float atkDistance_; //攻撃可能距離

    //デバッグ用
    int color_;
    int serchCol_;
    int alertCol_;
    double debugRot_;
};

