#include "EnemyHp.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/Utility.h"

//���[�J���萔
namespace {
	const float HP_START_POS_X = -50.0f;						//HP�J�v�Z���J�n�ʒu(X)
	const VECTOR HP_START_POS = { HP_START_POS_X,0.0f,0.0f };	//HP�J�v�Z���J�n�ʒu
	const float PERCENT_MAX = 100.0f;							//�{��

	const int HP_COLOR = 0xff5555;		//�J�v�Z���F
	const int NONE_COLOR = 0x000000;	
	const int HP_DIV = 4;	
	const int HP_RANGE = 6;
}

EnemyHp::EnemyHp(VECTOR& _followPos,float& _now, float _max) :UIBase(_followPos),nowHp_(_now), hpMax_(_max)
{
}

EnemyHp::~EnemyHp(void)
{
}

bool EnemyHp::Init(const std::string& _master)
{
	return false;
}

bool EnemyHp::Update(void)
{
	return false;
}

void EnemyHp::Draw(void)
{
	//�c��HP�̊���
	float hpPercent = nowHp_ / hpMax_;

	//Hp�J�v�Z���̎n�_�ƏI�_�̑��΍��W
	VECTOR startPos = HP_START_POS;
	VECTOR endPos = { HP_START_POS_X + (PERCENT_MAX * hpPercent),0.0f,0.0f };

	//�J�������擾
	auto& camera = SceneManager::GetInstance().GetCamera();

	//�J��������G�ʒu�ւ̃x�N�g��
	VECTOR angle = VSub(drawFollowPos_, camera.GetPos());
	//�p�x���߂�
	float afterDeg = atan2(angle.x, angle.z);

	//��]���̐���
	Quaternion qua = {};
	qua = qua.AngleAxis(afterDeg, Utility::AXIS_Y);
	//���΍��W�̉�]
	startPos = qua.PosAxis(startPos);
	endPos = qua.PosAxis(endPos);

	//���ۂ̕\���ʒu
	VECTOR hpUIStartPos = VAdd(drawFollowPos_, startPos);
	hpUIStartPos.y -= 50;
	VECTOR hpUIEndPos = VAdd(drawFollowPos_, endPos);
	hpUIEndPos.y -= 50;

	//�\��
	DrawCapsule3D(hpUIStartPos, hpUIEndPos, HP_RANGE, HP_DIV, HP_COLOR, NONE_COLOR, true);
}

void EnemyHp::Reset(void)
{
}
