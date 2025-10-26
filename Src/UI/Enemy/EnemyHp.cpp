#include "EnemyHp.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/Utility.h"

//ローカル定数
namespace {
	const float HP_START_POS_X = -50.0f;						//HPカプセル開始位置(X)
	const VECTOR HP_START_POS = { HP_START_POS_X,0.0f,0.0f };	//HPカプセル開始位置
	const float PERCENT_MAX = 100.0f;							//倍率

	const int HP_COLOR = 0xff5555;		//カプセル色
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
	//残量HPの割合
	float hpPercent = nowHp_ / hpMax_;

	//Hpカプセルの始点と終点の相対座標
	VECTOR startPos = HP_START_POS;
	VECTOR endPos = { HP_START_POS_X + (PERCENT_MAX * hpPercent),0.0f,0.0f };

	//カメラ情報取得
	auto& camera = SceneManager::GetInstance().GetCamera();

	//カメラから敵位置へのベクトル
	VECTOR angle = VSub(drawFollowPos_, camera.GetPos());
	//角度求める
	float afterDeg = atan2(angle.x, angle.z);

	//回転情報の生成
	Quaternion qua = {};
	qua = qua.AngleAxis(afterDeg, Utility::AXIS_Y);
	//相対座標の回転
	startPos = qua.PosAxis(startPos);
	endPos = qua.PosAxis(endPos);

	//実際の表示位置
	VECTOR hpUIStartPos = VAdd(drawFollowPos_, startPos);
	hpUIStartPos.y -= 50;
	VECTOR hpUIEndPos = VAdd(drawFollowPos_, endPos);
	hpUIEndPos.y -= 50;

	//表示
	DrawCapsule3D(hpUIStartPos, hpUIEndPos, HP_RANGE, HP_DIV, HP_COLOR, NONE_COLOR, true);
}

void EnemyHp::Reset(void)
{
}
