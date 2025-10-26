#pragma once
#include "../CharacterBase.h"

//�G�̓X���[�̉e�����󂯂�
//����čX�V�Ɋւ��J�E���^�[��SceneManager��updateSpeedRate���Q�Ƃ���

class Game;
class EnemyUIController;

class EnemyBase :
    public CharacterBase
{
public:
#pragma region �X�e�[�^�X�֘A

    //���̑��p�����[�^��
    static constexpr float MOVE_POW = 3.0f;     //�ړ���
    static constexpr float MOVE_POW_FIND = 5.0f;//�ړ���(������)
    static constexpr float CHARA_SCALE = 0.7f;  //�T�C�Y
    static constexpr float ENEMY_HP = 70.0f;

#pragma endregion

#pragma region ��Ԃ��Ɗ֘A

      //��ԑJ�ڂɊւ�����
    static constexpr float FIELD_VISION_DEG_HALF = 40.0f;	//���E�̊p�x(�������ɓW�J����̂őS�̂̔����̊p�x�𖾋L)
    static constexpr float FIELD_VISION_DISTANCE = 1000.0f;	//���E�̋���
    static constexpr float ALERT_DISTANCE = 3000.0f;     	//�x���̋���
    static constexpr float ATTACK_DISTANCE = 200.0f;     	//�U���J�n�̋���
    static constexpr float BATTLE_FINISH_DISTANCE = 1500.0f; //�U����ԉ����̋���


    static constexpr float SEARCH_RESTART_TIME = 100.0f;  //�x���ĊJ����
    static constexpr float SEARCH_CNT_MAX = 60.0f;  //�T�[�`��������

    //�����ړ���
    static constexpr float MOVE_RANDOM_MIN = 200.0f;                   //�Œ�l
    static constexpr float MOVE_RANDOM_MAX = 600.0f - MOVE_RANDOM_MIN;;//�ō��l(���ۂ̌v�Z�ō̎Z�����킹�邽�߂��̂悤�ɂȂ�B���ۂ̍ő�l����ŏ��l������)

    static constexpr float STAY_TIME = 200.0f;   //�X�e�C�̎���
    static constexpr VECTOR SCALE_DOWN = { 0.01f,0.01f,0.01f };

#pragma endregion

    static constexpr float SPEED_ANIM = 1.0f;  //�A�j���[�V�����X�s�[�h

    //�U���֘A(�O���t�@�C����������)
    static constexpr VECTOR RELATIVE_ATTACK_POS = { 0.0f, 75.0f, 100.0f };
    static constexpr float SCALE_ATTACK_NOMAL = 70.0f;
    static constexpr float POW_ATTACK_NOMAL = 1.0f;
    static constexpr float INTERVAL_ATTACK_NOMAL = 150.0f;

    /// <summary>
    /// �G�̏��
    /// </summary>
    enum class ENEMY_STATE {
        NOMAL,
        SEARCH,
        BATTLE,
        DETH,
        END
    };
    /// <summary>
    /// ���E����
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

    //��������
    const bool IsAlive(void)const override;
    const ENEMY_STATE GetNowState(void)const { return state_; }
    /// <summary>
    /// �A�j���[�V�����̃X�s�[�h�ύX
    /// </summary>
    /// <param name="_percent">�p�[�Z���e�[�W</param>
    const void SetAnimSpeedRate(const float _percent);

    void SetIsLockTraget(const bool _flag) { isLockTarget_ = _flag; }
    void SetIsLocked(const bool _flag);

    //�_���[�W
    void Damage(const float _pow)override;
    //���S������
    void Deth(void)override;

    virtual void Shout(void);//�{�X��p

     //�f�o�b�O�p
    void SetColor(int _color);
    void SetPos(VECTOR _pos);
    void DrawDebug(void)override;
  
protected:
    virtual void SetPram(void) = 0;    //�e�G�̌ŗL���
    virtual void InitAnim(void)override;
    void InitUI(void)override;

#pragma region �e���ԍX�V

    //���ʂ̊�b����
    //���L�̏���������ꍇoverride����
    virtual void UpdateNomal(const VECTOR& _pPos, AttackManager& _atk);  //�ʏ�
    virtual void UpdateSearch(const VECTOR& _pPos, AttackManager& _atk); //���G
    virtual void UpdateBattle(const VECTOR& _pPos, AttackManager& _atk); //�퓬
    void UpdateDeth(const VECTOR& _pPos, AttackManager& _atk); //���S

    //�e��ړ�����
    void MoveNomal(const VECTOR& _pPos);
    void MoveSearch(const VECTOR& _pPos);
    virtual void MoveBattle(const VECTOR& _pPos);

    void OderGoalRot(const VECTOR _pPos);   //�v���C���[�܂ł̊p�x

    void ChangeState(const ENEMY_STATE _state); //��Ԃ̑J��

#pragma endregion

    virtual void DrawUI(void)override;

    using Update_f = void(EnemyBase::*)(const VECTOR& _pPos, AttackManager& _atk);
    using Move_f = void(EnemyBase::*)(const VECTOR& _pPos);
    Update_f update_;   //�X�V�֐�
    Move_f move_;       //�ړ��֐�

    std::unique_ptr<EnemyUIController>uiCntl_;
    float maxHp_;

    VECTOR preStayPos_; //�O���~�ʒu
    float uiDeviationY_;//UI�ʒu����
    float moveOneTime_; //���̈ړ���
    float moveSped_;    //�P�t���[���ł̈ړ���
    bool isStay_;       //�X�e�C���ǂ���
    float stayCnt_;     //�X�e�C��Ԃ̃J�E���^
    float stopTime_;    //�U�����̒�~����
    float intervalCnt_; //�U���Ԋu�̃J�E���^

    float searchRestartCnt_;    //�^���ĊJ�J�E���^
    float searchCnt_;   //�^���J�E���^

    bool isAlive_;     //�폜���Ă�����
    ENEMY_STATE state_;//���

    bool isLockTarget_;   //���b�N�I���ΏۂɂȂ��Ă��邩(�}�l�[�W���ł̂ݕύX���\)

    VECTOR atkRelative_;    //�U���̔����ʒu�̑��΍��W
    float atkScale_;    //�U���̑傫��
    float atkDistance_; //�U���\����

    //�f�o�b�O�p
    int color_;
    int serchCol_;
    int alertCol_;
    double debugRot_;
};

