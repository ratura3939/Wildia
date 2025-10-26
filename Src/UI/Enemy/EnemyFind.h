#pragma once
#include<string>
#include "../UIBase.h"
#include"../../Object/Character/Enemy/EnemyBase.h"

class EnemyFind :
    public UIBase
{
public:
	//�u�H�v�g��p
	static constexpr float SUSPECT_EXT_MAX = 100.0f;//�g�嗦�ő�l
	static constexpr float SUSPECT_EXT_ACC = 2.0f;	//�g��X�s�[�h
	//�u�I�v�`��p
	static constexpr float FIND_UI_DRAW_TIME = 30.0f;	//�`�掞��
	static constexpr float FIND_UI_DRAW_SIZE = 50.0f;	//�`��T�C�Y

	EnemyFind(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state);
	~EnemyFind(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string suspectStr_;
	std::string findStr_;

	float findUICnt_;   //������\����������J�E���^�[

	EnemyBase::ENEMY_STATE& eState_;
};

