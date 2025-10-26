#pragma once
#include<string>
#include<vector>
#include<map>
#include<DxLib.h>

class UIManager2d
{
public:
	//UI�̉��o
	enum class UI_DIRECTION_2D {
		NOMAL,		//�ʏ�
		MOVE_UP,	//��ړ�
		MOVE_DOWN,	//���ړ�
		MOVE_LEFT,	//���ړ�
		MOVE_RIGHT,	//�E�ړ�
		UP_DOWN,	//�㉺�ړ�(���[�v)
		LEFT_RIGHT,	//���E�ړ�(���[�v)
		ZOOM_IN,	//�g��
		ZOOM_OUT,	//�k��
		ZOOM_INOUT,	//�g��k��(���[�v)
		ROT_RIGHT,	//�E��]
		ROT_LEFT,	//����]
		ROT_CRADLE,	//���E�ɉ�](���[�v)
		GRAD_DISAP,	//���X�ɏ�����
		GRAD_AP,	//���X�Ɍ����
		FLASHING,	//�_��(���[�v)
	};

	/// <summary>
	/// ���ڕ���(�P���ڂɂ���܂ł�����)
	/// </summary>
	enum class UI_DIRECTION_GROUP {
		NONE,
		MOVE,		//�ړ�
		ZOOM,		//�g��k��
		ROTATION,	//��]
		GRADUALLY,	//�����E�s����(Transpercent�����Ƃ��Ǝg���Ă����̂Łu���X�Ɂv�Ƃ����p�P��ɂ���)
	};

	enum class UI_DRAW_DIMENSION {
		DIMENSION_2,
		DIMENSION_3,
	};

	/// <summary>
	/// �`��ɕK�v�Ȃ���
	/// </summary>
	struct UIInfo {
		UI_DRAW_DIMENSION dimension;
		VECTOR pos;	//�ʒu
		float scl;	//�傫��
		float deg;	//�p�x
		float alpha;//�����x
	};

	//���o��ɂ��K�v�Ȃ���
	struct DirectionInfo {
		UI_DIRECTION_2D type;	//���o���
		float pow;	//���܂ł̉��Z�ʁi���Rotation��Move�Ŏg�p�A�J�n�����̏�Ԃ��O�Ƃ��Ĉ������ߏ�������ɒ��ӂ��K�v�j
		float acc;	//���Z��
		float max;	//�ő�l
		float min;	//�ŏ��l
		bool isFinish;//�I��������
	};

	static constexpr float ALPHA_MAX = 255.0f;
	static constexpr float ALPHA_MIN = 0.0f;

	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static UIManager2d& GetInstance(void);

	/// <summary>
	/// �`����̓o�^
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_imgHndl">�`�悷��摜</param>
	/// <param name="_type">���o</param>
	void Add(const std::string& _name, const int _imgHndl, const UI_DIRECTION_2D _type,const UI_DRAW_DIMENSION _dimension);

	/// <summary>
	/// �v�f�̏���
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void DeleteUI(const std::string& _name);				//�P��
	void DeleteUI(const std::vector<std::string> _names);	//����

	/// <summary>
	/// ���o�ǉ�
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_type">�ǉ����鉉�o</param>
	void PushUIDirection(const std::string& _name, UI_DIRECTION_2D _type);
	/// <summary>
	/// ���߂Œǉ��������o���폜
	/// </summary>
	/// <param name="_name"></param>
	void PopUIDirection(const std::string& _name);

	/// <summary>
	/// �`���b���ݒ�
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_pos">�`��ʒu(���S���W)</param>
	/// <param name="_scale">�傫��</param>
	/// <param name="_deg">�p�x</param>
	/// <param name="_alpha">�����x</param>
	void SetUIInfo(const std::string& _name, const VECTOR _pos, const float _scale = 1.0f, const float _deg = 0.0f, const float _alpha = ALPHA_MAX);

	/// <summary>
	/// UI���o�p���ݒ�
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <param name="_acc">�����x</param>
	/// <param name="_max">�ő�l</param>
	/// <param name="_min">�ŏ��l</param>
	void SetUIDirectionPram(const std::string& _name, const UI_DIRECTION_GROUP _group, const float _acc, const float _max, const float _min);

	//�ʒu�Đݒ�
	void SetPos(const std::string& _name,const VECTOR& _pos);

	//�摜�̍����ւ�
	void SetImage(const std::string& _name, const int _imageHndl);

	//�X�V
	void Update(const std::string _name);				//�P��
	void Update(const std::vector<std::string> _names);	//����

	//�`��
	void Draw(const std::string _name);					//�P��
	void Draw(const std::vector<std::string> _names);	//����

	//���
	void Relese(void);

	//����
	void Destroy(void);

	//�X�V�̏�����
	void ResetUpdate(const std::string _name, const UI_DIRECTION_GROUP _group);

	/// <summary>
	/// ����̍X�V���I��������(���[�v���Ȃ����̂��O��Ƃ���)
	/// </summary>
	/// <param name="_name"></param>
	/// <param name="_group"></param>
	/// <returns></returns>
	const bool IsFinishDirection(const std::string _name,const UI_DIRECTION_GROUP _group);

	const bool IsLoopUpdate(const std::string _name, const UI_DIRECTION_GROUP _group);

	const VECTOR GetDrawPos(const std::string _name)const;

private:
	/// <summary>
	/// ���o�̑�܂��Ȏ�ނ��擾
	/// </summary>
	/// <param name="_name">�o�^��</param>
	/// <returns>�O���[�v</returns>
	const UI_DIRECTION_GROUP GetDirectionGroup(const std::string _name);
	//��ޔ�
	const UI_DIRECTION_GROUP GetDirectionGroup(const UI_DIRECTION_2D _type);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void Move(const std::string& _name, DirectionInfo& _direcInfo);

	void Zoom(const std::string& _name, DirectionInfo& _direcInfo);

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void Rotation(const std::string& _name, DirectionInfo& _direcInfo);

	/// <summary>
	/// �A���t�@�l����
	/// </summary>
	/// <param name="_name">�o�^��</param>
	void AlphaAcc(const std::string& _name, DirectionInfo& _direcInfo);

	//�C���X�^���X�p
	static UIManager2d* instance_;

	std::map<std::string, int>images_;				//�`��摜�i�[
	std::map<std::string, UIInfo>infoes_;			//UI�̕`��Ɋւ����
	std::map<std::string, std::vector<DirectionInfo>>direcInfoes_;	//UI�̉��o�Ɋւ����


	using Update_UI = void(UIManager2d::*)(const std::string&, DirectionInfo& _direcInfo);
	std::map<std::string, std::vector<Update_UI>>updates_;	//UI�X�V����
};

