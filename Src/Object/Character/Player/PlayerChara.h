#pragma once
#include <DxLib.h>
#include<memory>
#include "../CharacterBase.h"
#include"../../../UI/PlayerUIController.h"

//�v���C���[�̓X���[�̉e�����󂯂Ȃ�
//����čX�V�Ɋւ��J�E���^�[�͒ʏ�̃C���N�������g

class Game;

class PlayerChara :
    public CharacterBase
{
public:
    /// <summary>
    /// ���b�N�I������Ԃɂ��邩�ǂ���
    /// </summary>
    enum class ROCK_STATE {
        NOMAL,  //�ʏ�
        LOCKON, //���b�N�I��
        MAX
    };

    /// <summary>
    /// �v���C���[�s�����
    /// </summary>
    enum class STATE {
        NOMAL,
        GUARD,
        DODGE,
        JUST_DODGE,
        ATTACK,
    };

    /// <summary>
    /// �ړ�����
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

    //�����_�̎擾
    const VECTOR GetFocusPoint(void)const;

    /// <summary>
    /// ���b�N�I���֌W�̏�ԕω�
    /// </summary>
    /// <param name="_state">ture=���b�N�I��,false=���b�N�I�t</param>
    void ChangeRockState(const bool _state);

    //��Ԋ֌W
    const STATE GetState(void)const;    //�擾
    void SetState(const STATE& _state); //�ݒ�(PlayerManager����̎�t)
    void PlayAnim(const std::string _anim); //�A�j���[�V�����̍Đ�(�O��<�}�l�[�W���E�����蔻��>���)
    const bool IsRock(void);
    void Damage(const float _pow)override;

    void DrawDebug(void)override;

    //���͎�t
    void InputDash(const bool _flag) { isDush_ = _flag; }
    void InputMoveDir(const MOVE_DIR _dir) { moveDir_ = _dir; }

    float GetToLockDeg(void);   //���b�N�I���p�x

    void SetAtkAllert(void);    //�U����@�@�m

private:
    void InitAnim(void)override;
    void InitUI(void)override;
    void DrawUI(void)override;
    void Move(void);    //�ړ�����
    const std::string DecideAnim(const MOVE_DIR _dir)const;

    ROCK_STATE rState_; //���b�N�I�����
    STATE state_;       //���

    VECTOR focusPoint_; //�����_

    //���͎�t�ϐ�
    MOVE_DIR moveDir_;  //�ړ�����
    bool isDush_;        //����

    std::unique_ptr<PlayerUIController>uiCntl_; //UI

    //UI�֘A
    int allertTime_;
};

