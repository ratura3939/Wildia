#include"../../Utility/Utility.h"
#include "UIManager2d.h"

UIManager2d* UIManager2d::instance_ = nullptr;

void UIManager2d::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new UIManager2d();
	}
}

UIManager2d& UIManager2d::GetInstance(void)
{
	return *instance_;
}

void UIManager2d::Add(const std::string& _name, const int _imgHndl, const UI_DIRECTION_2D _type, const UI_DRAW_DIMENSION _dimension)
{
	//�v�f������Ƃ�
	if (direcInfoes_.contains(_name)) {
		//�����镪��
		for (auto& direc : direcInfoes_[_name]) {
			if (GetDirectionGroup(direc.type) == GetDirectionGroup(_type)) {
				return;
			}
		}
	}

	//�摜�̊i�[
	images_.emplace(_name, _imgHndl);

	//��b��񏉊���
	UIInfo info = {};
	info.dimension = _dimension;
	info.pos = Utility::VECTOR_INIT;
	info.scl = 1.0f;
	info.deg = 0.0f;
	info.alpha = ALPHA_MAX;

	//��b���ǉ�
	infoes_.emplace(_name, info);

	//�ʏ�`��Ȃ牉�o�ǉ��̕K�v�͂Ȃ�
	if (_type == UI_DIRECTION_2D::NOMAL)return;

	//���o�����̒ǉ�
	PushUIDirection(_name, _type);
}

void UIManager2d::DeleteUI(const std::string& _name)
{
	images_.erase(_name);
	infoes_.erase(_name);
	if (direcInfoes_.contains(_name)) {
		direcInfoes_.erase(_name);
	}
}

void UIManager2d::DeleteUI(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		images_.erase(name);
		infoes_.erase(name);
		if (direcInfoes_.contains(name)) {
			direcInfoes_.erase(name);
		}
	}
}

void UIManager2d::PushUIDirection(const std::string& _name, UI_DIRECTION_2D _type)
{
	//���o��񏉊���
	DirectionInfo direcInfo = {};
	direcInfo.type = _type;
	direcInfo.pow = 0.0f;
	direcInfo.acc = 0.0f;
	direcInfo.max = 0.0f;
	direcInfo.min = 0.0f;

	//���o���̒ǉ�
	direcInfoes_[_name].push_back(direcInfo);

	//�X�V�̒ǉ�
	UI_DIRECTION_GROUP group = GetDirectionGroup(_type);
	if (group == UI_DIRECTION_GROUP::MOVE) {
		updates_[_name].push_back(&UIManager2d::Move);
	}
	if (group == UI_DIRECTION_GROUP::ZOOM) {
		updates_[_name].push_back(&UIManager2d::Zoom);
	}
	if (group == UI_DIRECTION_GROUP::ROTATION) {
		updates_[_name].push_back(&UIManager2d::Rotation);
	}
	if (group == UI_DIRECTION_GROUP::GRADUALLY) {
		updates_[_name].push_back(&UIManager2d::AlphaAcc);
	}
}

void UIManager2d::PopUIDirection(const std::string& _name)
{
	updates_[_name].pop_back();
	direcInfoes_[_name].pop_back();
}

void UIManager2d::SetUIInfo(const std::string& _name, const VECTOR _pos, const float _scale, const float _deg, const float _alpha)
{
	infoes_[_name].pos = _pos;
	infoes_[_name].scl = _scale;
	infoes_[_name].deg = _deg;
	infoes_[_name].alpha = _alpha;
}

void UIManager2d::SetUIDirectionPram(const std::string& _name, const UI_DIRECTION_GROUP _group, const float _acc, const float _max, const float _min)
{
	for (auto& info:direcInfoes_[_name]) {
		
		auto group = GetDirectionGroup(info.type);
		//�ݒ肷��p�����[�^�̈Ӗ����������
		if (group != _group)continue;
		info.acc = _acc;
		info.max = _max;
		info.min = _min;
		info.isFinish = false;

		//���������̕����̕������͐�ɐݒ肵�Ă���
		auto direcType = info.type;
		if (direcType == UI_DIRECTION_2D::MOVE_UP ||
			direcType == UI_DIRECTION_2D::MOVE_LEFT ||
			direcType == UI_DIRECTION_2D::ROT_RIGHT ||
			direcType == UI_DIRECTION_2D::ZOOM_OUT ||
			direcType == UI_DIRECTION_2D::GRAD_DISAP) {
			info.acc *= -1.0f;
		}
	}
}

void UIManager2d::SetPos(const std::string& _name, const VECTOR& _pos)
{
	//�����̃G�t�F�N�g�����邩�����o
	bool isMoveDirec = false;
	UI_DIRECTION_2D moveType = UI_DIRECTION_2D::NOMAL;
	for (auto& info : direcInfoes_[_name]) {
		if (GetDirectionGroup(info.type) == UI_DIRECTION_GROUP::MOVE) {
			isMoveDirec = true;
			moveType = info.type;
			break;
		}
	}
	
	//����������ꍇ
	if (isMoveDirec) {
		//�����̕����ɂ͓������Ȃ�
		//�㉺�ړ��Ȃ��
		if (moveType == UI_DIRECTION_2D::MOVE_DOWN ||
			moveType == UI_DIRECTION_2D::MOVE_UP ||
			moveType == UI_DIRECTION_2D::UP_DOWN) {
			//Y�ȊO�𓯊�
			infoes_[_name].pos.x = _pos.x;
			infoes_[_name].pos.z = _pos.z;
		}
		//���E�ړ��Ȃ��
		if (moveType == UI_DIRECTION_2D::MOVE_LEFT ||
			moveType == UI_DIRECTION_2D::MOVE_RIGHT ||
			moveType == UI_DIRECTION_2D::LEFT_RIGHT) {
			//X���͓������Ȃ�
			infoes_[_name].pos.y = _pos.y;
			infoes_[_name].pos.z = _pos.z;
		}
	}
	else {
		//���ׂē���
		infoes_[_name].pos = _pos;
	}	
}

void UIManager2d::SetImage(const std::string& _name, const int _imageHndl)
{
	//�摜�̍����ւ�
	images_[_name] = _imageHndl;
}

void UIManager2d::Update(const std::string _name)
{
	int cnt = 0;
	for (auto& update : updates_[_name]) {
		(this->*update)(_name, direcInfoes_[_name][cnt]);
		cnt++;
	}
}

void UIManager2d::Update(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		int cnt = 0;
		for (auto& update : updates_[name]) {
			(this->*update)(name,direcInfoes_[name][cnt]);
			cnt++;
		}
	}
}

void UIManager2d::Draw(const std::string _name)
{
	auto info = infoes_[_name];
	//�������獕������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(info.alpha));
	if (info.dimension == UI_DRAW_DIMENSION::DIMENSION_2) {
		DrawRotaGraph(static_cast<int>(info.pos.x), static_cast<int>(info.pos.y), info.scl, info.deg / 180.0f, images_[_name], true);
	}
	else {
		DrawBillboard3D(info.pos, 0.5f, 0.5f, info.scl, info.deg*DX_PI_F / 180.0f,  images_[_name],  false);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIManager2d::Draw(const std::vector<std::string> _names)
{
	for (auto& name : _names) {
		Draw(name);	//�P�̕`��ɓn���B
	}
}

void UIManager2d::Relese(void)
{
	images_.clear();
	infoes_.clear();
	direcInfoes_.clear();
	updates_.clear();
}

void UIManager2d::Destroy(void)
{
	Relese();
	delete instance_;
}

void UIManager2d::ResetUpdate(const std::string _name, const UI_DIRECTION_GROUP _group)
{
	//�X�V�̐�����
	for (auto& direction : direcInfoes_[_name]) {
		//�Y������X�V����������
		if (GetDirectionGroup(direction.type) == _group) {
			//���̍X�V���I���������̃t���O��Ԃ��B
			direction.pow = 0.0f;
		}
	}
}

const bool UIManager2d::IsFinishDirection(const std::string _name, const UI_DIRECTION_GROUP _group)
{
	//���[�v����Ȃ����Ƃ��O��
	if (!IsLoopUpdate(_name, _group)) 
	{
		//�X�V�̐�����
		for (auto& direction : direcInfoes_[_name]) {
			//�Y������X�V����������
			if (GetDirectionGroup(direction.type) == _group) {
				//���̍X�V���I���������̃t���O��Ԃ��B
				return direction.isFinish;
			}
		}
	}
	//���[�v�E���������Y��������̂��Ȃ��������p
	return false;
}

const bool UIManager2d::IsLoopUpdate(const std::string _name, const UI_DIRECTION_GROUP _group)
{
	//��񕪉�
	for (auto& direction : direcInfoes_[_name]) {
		//�Y���̍X�V�̂Ƃ������������B
		if (GetDirectionGroup(direction.type) == _group) {
			//���[�v�Ȃ�true��Ԃ��B
			if (direction.type == UI_DIRECTION_2D::UP_DOWN ||
				direction.type == UI_DIRECTION_2D::LEFT_RIGHT ||
				direction.type == UI_DIRECTION_2D::ZOOM_INOUT ||
				direction.type == UI_DIRECTION_2D::ROT_CRADLE ||
				direction.type == UI_DIRECTION_2D::FLASHING)
			{
				return true;
			}
		}
	}
	return false;
}

const VECTOR UIManager2d::GetDrawPos(const std::string _name) const
{
	return infoes_.at(_name).pos;
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const std::string _name)
{
	return GetDirectionGroup(direcInfoes_[_name][0].type);
}

const UIManager2d::UI_DIRECTION_GROUP UIManager2d::GetDirectionGroup(const UI_DIRECTION_2D _type)
{
	UI_DIRECTION_GROUP ret = UI_DIRECTION_GROUP::NONE;

	//�ړ�
	if (_type == UI_DIRECTION_2D::MOVE_UP	||
		_type == UI_DIRECTION_2D::MOVE_DOWN ||
		_type == UI_DIRECTION_2D::MOVE_LEFT ||
		_type == UI_DIRECTION_2D::MOVE_RIGHT||
		_type == UI_DIRECTION_2D::UP_DOWN	||
		_type == UI_DIRECTION_2D::LEFT_RIGHT) {

		ret = UI_DIRECTION_GROUP::MOVE;
	}

	//�g��k��
	if (_type == UI_DIRECTION_2D::ZOOM_IN || _type == UI_DIRECTION_2D::ZOOM_OUT || _type == UI_DIRECTION_2D::ZOOM_INOUT) {
		ret = UI_DIRECTION_GROUP::ZOOM;
	}

	//��]
	if (_type == UI_DIRECTION_2D::ROT_LEFT || _type == UI_DIRECTION_2D::ROT_RIGHT || _type == UI_DIRECTION_2D::ROT_CRADLE) {
		ret = UI_DIRECTION_GROUP::ROTATION;
	}

	//�����x
	if (_type == UI_DIRECTION_2D::GRAD_AP || _type == UI_DIRECTION_2D::GRAD_DISAP || _type == UI_DIRECTION_2D::FLASHING) {
		ret = UI_DIRECTION_GROUP::GRADUALLY;
	}

	return ret;
}

void UIManager2d::Move(const std::string& _name, DirectionInfo& _direcInfo)
{
	VECTOR afterPos = infoes_[_name].pos;
	auto direcType = _direcInfo.type;

	//�㉺
	if (direcType == UI_DIRECTION_2D::MOVE_UP || 
		direcType == UI_DIRECTION_2D::MOVE_DOWN || 
		direcType == UI_DIRECTION_2D::UP_DOWN) {

		//�ړ��ʉ��Z
		afterPos.y += _direcInfo.acc;
		_direcInfo.pow += _direcInfo.acc;

		//�ړ�����E�����̐ݒ�
		if (_direcInfo.pow > _direcInfo.max ||
			_direcInfo.pow < _direcInfo.min) {
			
			//�㉺�ړ��J��Ԃ��̂Ƃ�
			if (direcType == UI_DIRECTION_2D::UP_DOWN) {
				//���Z�������t��
				_direcInfo.acc *= -1.0f;
			}
			else {
				//�I��
				_direcInfo.isFinish = true;
				//�������Ȃ�
				return;
			}
			
		}
	}

	//���E
	if (direcType == UI_DIRECTION_2D::MOVE_LEFT ||
		direcType == UI_DIRECTION_2D::MOVE_RIGHT ||
		direcType == UI_DIRECTION_2D::LEFT_RIGHT) {

		//�ړ��ʉ��Z
		afterPos.x += _direcInfo.acc;
		_direcInfo.pow += _direcInfo.acc;

		//�ړ�����E�����̐ݒ�
		if (_direcInfo.pow > _direcInfo.max ||
			_direcInfo.pow < _direcInfo.min) {

			//�㉺�ړ��J��Ԃ��̂Ƃ�
			if (direcType == UI_DIRECTION_2D::LEFT_RIGHT) {
				//���Z�������t��
				_direcInfo.acc *= -1.0f;
			}
			else {
				//�I��
				_direcInfo.isFinish = true;
				//�������Ȃ�
				return;
			}

		}
	}

	//�ړ�����
	infoes_[_name].pos = afterPos;
}

void UIManager2d::Zoom(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterScl = infoes_[_name].scl;
	auto direcType = _direcInfo.type;

	afterScl += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//����
	//�ړ�����E�����̐ݒ�
	if (afterScl > _direcInfo.max ||
		afterScl < _direcInfo.min) {
		//�J��Ԃ������Ȃ�Ή��Z�������t��
		if (_direcInfo.type == UI_DIRECTION_2D::ZOOM_INOUT) {
			_direcInfo.acc *= -1.0f;
		}
		else {
			//�I��
			_direcInfo.isFinish = true;
			//����ȏ�ύX�������Ȃ�
			return;
		}
	}

	//�ړ�����
	infoes_[_name].scl = afterScl;
}

void UIManager2d::Rotation(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterDeg = infoes_[_name].deg;
	auto direcType = _direcInfo.type;

	afterDeg += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//����
	//�ړ�����E�����̐ݒ�
	if (_direcInfo.pow > _direcInfo.max ||
		_direcInfo.pow < _direcInfo.min) {
		//�J��Ԃ������Ȃ�Ή��Z�������t��
		if (_direcInfo.type == UI_DIRECTION_2D::ROT_CRADLE) {
			_direcInfo.acc *= -1.0f;
		}
	}

	//�ړ�����
	infoes_[_name].deg = afterDeg;
}

void UIManager2d::AlphaAcc(const std::string& _name, DirectionInfo& _direcInfo)
{
	float afterAlpha = infoes_[_name].alpha;
	auto direcType = _direcInfo.type;

	afterAlpha += _direcInfo.acc;
	_direcInfo.pow += _direcInfo.acc;

	//����
	//�ړ�����E�����̐ݒ�
	if (afterAlpha > _direcInfo.max ||
		afterAlpha < _direcInfo.min) {
		//�J��Ԃ������Ȃ�Ή��Z�������t��
		if (_direcInfo.type == UI_DIRECTION_2D::FLASHING) {
			_direcInfo.acc *= -1.0f;
		}
		else {
			//�I��
			_direcInfo.isFinish = true;
			//����ȏ�ύX�������Ȃ�
			return;
		}
	}

	//�ړ�����
	infoes_[_name].alpha = afterAlpha;
}

