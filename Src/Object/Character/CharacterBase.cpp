#include"../../Utility/Utility.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void)
{
	modelId_ = -1;
	pos_ = Utility::VECTOR_ZERO;
	scl_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;
	hp_ = -1.0f;

	characterRotY_ = Quaternion();

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotOrigin_ = Quaternion();
	quaRotLocal_ = Quaternion();
	goalQua_ = Quaternion();
	stepRotation_ = 0.0f;

	colRadius_ = -1;
	prePos_ = pos_;
	uiPos_ = Utility::VECTOR_ZERO;
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::Update(void)
{
}

void CharacterBase::Update(const VECTOR _pPos, AttackManager& _atk)
{
}

void CharacterBase::UpdateAnimOnly(void)
{
	animController_->Update();
}

void CharacterBase::Draw(void)
{
	//描画処理
	//DrawSphere3D(pos_, 30, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(modelId_);
	DrawUI();
}

const bool CharacterBase::Release(void)
{
	return true;
}

void CharacterBase::UpdateRotQuat(void)
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
	if (modelId_ != -1){
		MV1SetMatrix(modelId_, mat);
	}
}


void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetAngle();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, characterRotY_);
	// しきい値より大きかったら
	if (angleDiff > THRESHOLD_ROT){
		//回転し始めに設定
		stepRotation_ = 0.0f;
	}
	goalQua_ = axis;
}

void CharacterBase::Rotation(void)
{
	//回転時間増加
	stepRotation_ +=PER_ROT;
	// 回転の球面補間
	characterRotY_ = Quaternion::Slerp(
		characterRotY_, goalQua_, stepRotation_);

	//初期化を行い新たな回転量を設定する
	quaRot_ = Quaternion();
	quaRot_ = quaRot_.Mult(characterRotY_);
}

VECTOR CharacterBase::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR CharacterBase::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR CharacterBase::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR CharacterBase::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR CharacterBase::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR CharacterBase::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR CharacterBase::GetDir(const VECTOR& _vec) const
{
	return quaRot_.PosAxis(_vec);
}

const VECTOR CharacterBase::GetPos(void) const
{
	return pos_;
}

const VECTOR CharacterBase::GetHeight(void) const
{
	return VAdd(pos_, CHARACTER_HEIGHT);
}

const float CharacterBase::GetCollisionRadius(void) const
{
	return colRadius_;
}

void CharacterBase::SetPrePos(void)
{
	pos_ = prePos_;
}

const Quaternion CharacterBase::GetQua(void) const
{
	return characterRotY_;

	Quaternion retRot = Quaternion();
	//VECTORなのでQuaternionに変化
	retRot = retRot.Mult(characterRotY_);
	return retRot;
}

const std::string CharacterBase::GetSpeciesName(void) const
{
	return speciesName_;
}

const bool CharacterBase::IsAlive(void) const
{
	return hp_ > 0;
}

void CharacterBase::Deth(void)
{
	hp_ = 0;
}

void CharacterBase::DrawDebug(void)
{
}

void CharacterBase::DrawCupcel(void)
{
	// 上の球体
	VECTOR pos1 = GetHeight();
	DrawSphere3D(pos1, CHARACTER_RADIUS, 5, COLOR, COLOR, false);

	// 下の球体
	VECTOR pos2 = GetPos();
	DrawSphere3D(pos2, CHARACTER_RADIUS, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// 球体を繋ぐ線(X+)
	dir = GetRight();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(X-)
	dir = GetLeft();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(Z+)
	dir = GetForward();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(Z-)
	dir = GetBack();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// カプセルの中心
	DrawSphere3D(VAdd(GetPos(),VScale(CHARACTER_HEIGHT,2.0f)), 5.0f, 10, COLOR, COLOR, true);
}
