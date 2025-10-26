#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include "Skelton.h"

namespace {
#pragma region アニメーション関連
    const int ANIM_IDLE = 42;    //待機

    //攻撃(継承先ごとで違うので最終的にはここではない)
    const int ANIM_ATTACK_NOMAL = 9;     //攻撃

      //その他アクション
    const int ANIM_WALK = 94;            //歩き
    const int ANIM_DUSH_FORWARD = 55;    //ダッシュ(前方)

    //演出
    const int ANIM_DETH_START = 29;      //死亡開始
    const int ANIM_DETH_SUSTANABLE = 28; //死亡持続
#pragma endregion
}

Skelton::Skelton(VECTOR& _pos) :EnemyBase(_pos)
{
}

Skelton::~Skelton(void)
{
}

void Skelton::InitAnim(void)
{
    animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("attack", ANIM_ATTACK_NOMAL, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dush", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void Skelton::SetPram(void)
{
	//各敵たち
	//後々Jsonやったら楽になるかも？
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);

	if (modelId_ == -1) {
		return;
	}
	//パラメータ関係
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//初期化用に一回実行
	UpdateRotQuat();

	//当たり判定大きさ
	colRadius_ = CHARACTER_RADIUS;

	//攻撃の発生位置(相対座標)
	atkRelative_ = RELATIVE_ATTACK_POS;
	//攻撃の大きさ
	atkScale_ = SCALE_ATTACK_NOMAL;
	//攻撃可能距離
	atkDistance_ = ATTACK_DISTANCE;

	//アニメーション初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	uiDeviationY_ = 250.0f;
	maxHp_ = ENEMY_HP;

	//位置設定
	uiPos_ = pos_;
	//頭位置
	uiPos_.y += uiDeviationY_;

	//UI初期化
	InitUI();

	//モデル描画クラス生成
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS


	//PS
	//各色の強さ(拡散光)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//ブラーの強さ(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//サンプル数(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });

	intervalCnt_ = 0.0f;

	//状態を通常に
	ChangeState(ENEMY_STATE::NOMAL);
}
