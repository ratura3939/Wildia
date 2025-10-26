#include<DxLib.h>
#include<cassert>
#include"../../Application.h"
#include "AnimationController.h"

AnimationController::AnimationController(int& _model):modelId_(_model)
{
	activeAnim_.type=PLAY_TYPE::MAX;
	activeAnim_.idx = -1;
	activeAnim_.total = -1.0f;

	attachAnim_ = -1;
	speedAnim = -1.0f;
	counter_ = -1.0f;
	speedRate_ = 1.0f;

	finishAnim_ = &AnimationController::FinishAnimNomal;
	updateAnim_ = &AnimationController::UpdateNomalAnim;

	nextAnim_ = {};
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(const std::string& _name, const int _attach, const PLAY_TYPE _type)
{
	//すでに要素がある時
	if (animDatas_.contains(_name)) {
		//エラー防止
		assert("すでに登録しているものを再登録しようとしています。");
		return;
	}

	//初期化(アタッチ番号だけ入れる)
	AnimationInfo anim = {};
	anim.type = _type;
	anim.idx = _attach;
	
	//総再生時間を取得するためには一回アタッチする必要がある
	attachAnim_ = MV1AttachAnim(modelId_, anim.idx);
	//時間取得
	anim.total = MV1GetAttachAnimTotalTime(modelId_, attachAnim_);
	//必要ないのでデタッチ
	MV1DetachAnim(modelId_, attachAnim_);

	//アニメーション情報追加
	animDatas_.emplace(_name, anim);

	attachAnim_ = -1;
}

void AnimationController::Play(const std::string& _name, const float _speed, const std::vector<std::string> _next)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を再生しようとしています。");
		return;
	}

	//もしかしたらこのifの中身が追加条件はいるかも
	//現在アタッチしているものと同じものなら処理は行わない
	if (activeAnim_.idx == animDatas_[_name].idx)return;

	//初期値以外のとき
	if (attachAnim_ != -1) {
		//現在のものをデタッチ
		MV1DetachAnim(modelId_, attachAnim_);
	}
	
	//次に再生されるアニメーションが設定されているとき
	if (!_next.empty()) {
		//ミス確認用(最後にLOOPはOK)
		for (auto& string : _next) {
			if (animDatas_[string].type == PLAY_TYPE::LOOP && string != _next.back()) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnim_ = _next;
	}

	//新規を代入
	activeAnim_.type = animDatas_[_name].type;
	activeAnim_.idx = animDatas_[_name].idx;
	activeAnim_.total = animDatas_[_name].total;
	//アタッチ
	attachAnim_ = MV1AttachAnim(modelId_, activeAnim_.idx);
	;

	//終了時処理の設定
	switch (activeAnim_.type)
	{
	case PLAY_TYPE::NOMAL:
		finishAnim_ = &AnimationController::FinishAnimNomal;
		updateAnim_ = &AnimationController::UpdateNomalAnim;
		break;
	case PLAY_TYPE::LOOP:
		finishAnim_ = &AnimationController::FinishAnimLoop;
		updateAnim_ = &AnimationController::UpdateNomalAnim;
		break;
	case PLAY_TYPE::RETURN:
		finishAnim_ = &AnimationController::FinishAnimReturn;
		updateAnim_ = &AnimationController::UpdateReturnAnim;
		break;
	default:
		assert("アニメーション登録のところでエラーが起きています");
		break;
	}

	//スピード設定
	speedAnim = _speed;
	//カウンターの初期化
	counter_ = 0.0f;
	if (activeAnim_.type == PLAY_TYPE::RETURN) {
		counter_ = activeAnim_.total;
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::AddNextAnim(const std::string& _name)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を連続で再生しようとしています。");
		return;
	}
	nextAnim_.push_back(_name);
}

void AnimationController::AddNextAnim(const std::vector<std::string> _names)
{
	for (auto& add : _names) {
		//要素がないとき
		if (!animDatas_.contains(add)) {
			//エラー防止
			assert("登録されていない要素を連続で再生しようとしています。");
			return;
		}
		nextAnim_.push_back(add);
	}
}

void AnimationController::Update(void)
{
	//初期値のとき
	if (attachAnim_ == -1)return;

	//カウンタ更新
	(this->*updateAnim_)();

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::ChangeSpeedRate(const float _percent)
{
 	speedRate_ = _percent / 100.0f;
}

void AnimationController::UpdateNomalAnim(void)
{
	// アニメーション再生
	counter_ += speedAnim * speedRate_;
	//再生上限にいった場合
	if (counter_ > activeAnim_.total)
	{
		//アニメーション終了時処理
		(this->*finishAnim_)();
	}
}

void AnimationController::UpdateReturnAnim(void)
{
	// アニメーション再生
	counter_ -= speedAnim * speedRate_;
	//再生上限にいった場合
	if (counter_ <= 0.0f)
	{
		//アニメーション終了時処理
		(this->*finishAnim_)();
	}
}

void AnimationController::FinishAnimNomal(void)
{
	//次に再生されている物が設定されているとき
	if (!nextAnim_.empty()) {
		//配列の最前列を再生
		Play(nextAnim_[0], speedAnim);
		//要素の削除
		nextAnim_.erase(nextAnim_.begin());
		return;
	}
	//待機に戻る
	Play("idle", 1.0f);
}

void AnimationController::FinishAnimLoop(void)
{
	counter_ = 0.0f;
}

void AnimationController::FinishAnimReturn(void)
{
	counter_ = activeAnim_.total;
}
