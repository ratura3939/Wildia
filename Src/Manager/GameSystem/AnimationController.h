#pragma once
#include<string>
#include<vector>
#include<unordered_map>

//����Wilidia�ł�"idle"�Ƃ������O�̃A�j���[�V������K������邱�ƁI
//NOMAL�̃A�j���[�V�����͏I����idle���Đ�����悤�ɂ��邽��


class AnimationController
{
public:
	/// <summary>
	/// �Đ����
	/// </summary>
	enum class PLAY_TYPE {
		NOMAL,	//�ʏ�Đ�
		LOOP,	//���[�v�Đ�
		RETURN,	//�t�Đ�
		MAX
	};

	//�A�j���[�V�����֘A���
	struct AnimationInfo {
		PLAY_TYPE type;	//�Đ��^�C�v
		int idx;		//�A�j���[�V�����ԍ�
		float total;	//���Đ�����
	};


	AnimationController(int& _model);
	~AnimationController(void);

	/// <summary>
	/// �A�j���[�V�����ǉ��֐�
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_attach">�A�j���[�V�����ԍ�</param>
	/// <param name="_type">�Đ��^�C�v</param>
	void Add(const std::string& _name,const int _attach, const PLAY_TYPE _type);
	/// <summary>
	/// �Đ��J�n����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_speed">�Đ����x</param>
	/// <param name="_next">�A�����čĐ����镨����<�Ō�ȊO��LOOP�̂��̂����Ȃ����ƁI�I></param>
	void Play(const std::string& _name, const float _speed, const std::vector<std::string> _next = {});
	/// <summary>
	/// �A�����čĐ�����A�j���[�V������r���Œǉ�����
	/// </summary>
	/// <param name="_name">�ǉ�����A�j���[�V������</param>
	void AddNextAnim(const std::string& _name);
	/// <summary>
	/// �A�����čĐ�����A�j���[�V������r���Œǉ�����
	/// </summary>
	/// <param name="_name">�ǉ�����A�j���[�V������(����)</param>
	void AddNextAnim(const std::vector<std::string> _names);
	void Update(void);

	/// <summary>
	/// �Đ����x(����)��ݒ�
	/// </summary>
	/// <param name="_percent">0���`100��</param>
	void ChangeSpeedRate(const float _percent);

private:
	//�A�j���[�V�����X�V����
	void UpdateNomalAnim(void);
	void UpdateReturnAnim(void);

	//�A�j���[�V�����I��������
	void FinishAnimNomal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

	int& modelId_;	//���f��ID
	std::unordered_map<std::string, AnimationInfo>animDatas_;	//�A�j���[�V�����f�[�^���܂Ƃ�
	AnimationInfo activeAnim_;	//�Đ����̃A�j���[�V�������
	int attachAnim_;//���ۂ̍Đ����Ă������
	float speedAnim;//�Đ����x
	float counter_;	//�X�V�J�E���^�[
	float speedRate_;

	std::vector<std::string> nextAnim_;	//���ɍĐ�����A�j���[�V����(LOOP�ȊO�ɓK�p)<�ŏI�ȊO��LOO�o�����Ȃ�����>

	using FinishAnimation = void(AnimationController::*)(void);
	using UpdateAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;
	UpdateAnimation updateAnim_;
};

