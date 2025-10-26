#include"../../Utility/Utility.h"
#include "Arrow.h"

Arrow::Arrow(const AttackManager::ATTACK_MASTER& _master, const int _mdlId, const VECTOR _pos, const float _pow, const Quaternion& _qua)
{
	master_ = _master;
	modelId_ = _mdlId;
	atkPow_ = _pow;

	pos_ = _pos;
	scl_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();

	quaRot_ = _qua;
	quaRotOrigin_ = Quaternion();
	quaRotLocal_ = Quaternion();

	isAlive_ = true;
	state_ = STATE::NONE;
}

Arrow::~Arrow(void)
{
}

void Arrow::Init(void)
{
	//諸々モデルの初期化
	VECTOR localPos = quaRot_.PosAxis(ARROW_LOCAL_POS);
	pos_ = VAdd(pos_, localPos);

	scl_ = Utility::VECTOR_ONE;

	quaRotLocal_ =
		Quaternion::Euler(Utility::Deg2RadF(90.0f), 0.0f, 0.0f);

	ChangeState(STATE::SHOT);
	SetIsAlive(true);

	UpdateRotQuat();
}

void Arrow::Update(void)
{
	switch (state_)
	{
	case Arrow::STATE::NONE:
		break;
	case Arrow::STATE::SHOT:
		//移動処理
		Move();
		break;
	case Arrow::STATE::DESTROY:
		break;
	case Arrow::STATE::END:
		break;
	default:
		break;
	}

	// モデル制御の基本情報更新
	UpdateRotQuat();

}

void Arrow::Draw(void)
{
	//ショット状態のときにのみ描画する
	if (state_ == STATE::SHOT)//DrawSphere3D(pos_, 5.0f, 20, 0x00ff00, 0x00ff00, true);
	MV1DrawModel(modelId_);
}

void Arrow::Release()
{
}

void Arrow::Destroy(void)
{
	ChangeState(STATE::DESTROY);
	SetIsAlive(false);
}

void Arrow::UpdateRotQuat(void)
{
	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1) {
		MV1SetMatrix(modelId_, mat);
	}
}

void Arrow::Move(void)
{
	// 前方向を取得
	VECTOR forward = GetForward();
	//下方向の取得
	VECTOR downward = GetDown();

	//移動量ベクトル
	VECTOR widthMovePow = VScale(forward, MOVE_SPEED);

	// 移動
	//前方
	pos_ = VAdd(pos_, widthMovePow);
	//下方
	pos_ =
		VAdd(pos_, VScale(downward, GRAVITY));
}

VECTOR Arrow::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}


VECTOR Arrow::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR Arrow::GetDir(const VECTOR& _vec) const
{
	return quaRot_.PosAxis(_vec);
}
