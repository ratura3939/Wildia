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
	//���łɗv�f�����鎞
	if (animDatas_.contains(_name)) {
		//�G���[�h�~
		assert("���łɓo�^���Ă�����̂��ēo�^���悤�Ƃ��Ă��܂��B");
		return;
	}

	//������(�A�^�b�`�ԍ����������)
	AnimationInfo anim = {};
	anim.type = _type;
	anim.idx = _attach;
	
	//���Đ����Ԃ��擾���邽�߂ɂ͈��A�^�b�`����K�v������
	attachAnim_ = MV1AttachAnim(modelId_, anim.idx);
	//���Ԏ擾
	anim.total = MV1GetAttachAnimTotalTime(modelId_, attachAnim_);
	//�K�v�Ȃ��̂Ńf�^�b�`
	MV1DetachAnim(modelId_, attachAnim_);

	//�A�j���[�V�������ǉ�
	animDatas_.emplace(_name, anim);

	attachAnim_ = -1;
}

void AnimationController::Play(const std::string& _name, const float _speed, const std::vector<std::string> _next)
{
	//�v�f���Ȃ��Ƃ�
	if (!animDatas_.contains(_name)) {
		//�G���[�h�~
		assert("�o�^����Ă��Ȃ��v�f���Đ����悤�Ƃ��Ă��܂��B");
		return;
	}

	//�����������炱��if�̒��g���ǉ������͂��邩��
	//���݃A�^�b�`���Ă�����̂Ɠ������̂Ȃ珈���͍s��Ȃ�
	if (activeAnim_.idx == animDatas_[_name].idx)return;

	//�����l�ȊO�̂Ƃ�
	if (attachAnim_ != -1) {
		//���݂̂��̂��f�^�b�`
		MV1DetachAnim(modelId_, attachAnim_);
	}
	
	//���ɍĐ������A�j���[�V�������ݒ肳��Ă���Ƃ�
	if (!_next.empty()) {
		//�~�X�m�F�p(�Ō��LOOP��OK)
		for (auto& string : _next) {
			if (animDatas_[string].type == PLAY_TYPE::LOOP && string != _next.back()) {
				assert("�����Đ��̌����������Ă��������B");
			}
		}
		nextAnim_ = _next;
	}

	//�V�K����
	activeAnim_.type = animDatas_[_name].type;
	activeAnim_.idx = animDatas_[_name].idx;
	activeAnim_.total = animDatas_[_name].total;
	//�A�^�b�`
	attachAnim_ = MV1AttachAnim(modelId_, activeAnim_.idx);
	;

	//�I���������̐ݒ�
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
		assert("�A�j���[�V�����o�^�̂Ƃ���ŃG���[���N���Ă��܂�");
		break;
	}

	//�X�s�[�h�ݒ�
	speedAnim = _speed;
	//�J�E���^�[�̏�����
	counter_ = 0.0f;
	if (activeAnim_.type == PLAY_TYPE::RETURN) {
		counter_ = activeAnim_.total;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::AddNextAnim(const std::string& _name)
{
	//�v�f���Ȃ��Ƃ�
	if (!animDatas_.contains(_name)) {
		//�G���[�h�~
		assert("�o�^����Ă��Ȃ��v�f��A���ōĐ����悤�Ƃ��Ă��܂��B");
		return;
	}
	nextAnim_.push_back(_name);
}

void AnimationController::AddNextAnim(const std::vector<std::string> _names)
{
	for (auto& add : _names) {
		//�v�f���Ȃ��Ƃ�
		if (!animDatas_.contains(add)) {
			//�G���[�h�~
			assert("�o�^����Ă��Ȃ��v�f��A���ōĐ����悤�Ƃ��Ă��܂��B");
			return;
		}
		nextAnim_.push_back(add);
	}
}

void AnimationController::Update(void)
{
	//�����l�̂Ƃ�
	if (attachAnim_ == -1)return;

	//�J�E���^�X�V
	(this->*updateAnim_)();

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::ChangeSpeedRate(const float _percent)
{
 	speedRate_ = _percent / 100.0f;
}

void AnimationController::UpdateNomalAnim(void)
{
	// �A�j���[�V�����Đ�
	counter_ += speedAnim * speedRate_;
	//�Đ�����ɂ������ꍇ
	if (counter_ > activeAnim_.total)
	{
		//�A�j���[�V�����I��������
		(this->*finishAnim_)();
	}
}

void AnimationController::UpdateReturnAnim(void)
{
	// �A�j���[�V�����Đ�
	counter_ -= speedAnim * speedRate_;
	//�Đ�����ɂ������ꍇ
	if (counter_ <= 0.0f)
	{
		//�A�j���[�V�����I��������
		(this->*finishAnim_)();
	}
}

void AnimationController::FinishAnimNomal(void)
{
	//���ɍĐ�����Ă��镨���ݒ肳��Ă���Ƃ�
	if (!nextAnim_.empty()) {
		//�z��̍őO����Đ�
		Play(nextAnim_[0], speedAnim);
		//�v�f�̍폜
		nextAnim_.erase(nextAnim_.begin());
		return;
	}
	//�ҋ@�ɖ߂�
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
