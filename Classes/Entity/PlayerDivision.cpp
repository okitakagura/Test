#include "PlayerDivision.h"
#include "Header\macro.h"
#include "Bean.h"
//��ҵķ���
PlayerDivision::PlayerDivision()
{

}

PlayerDivision::~PlayerDivision()
{

}
PlayerDivision * PlayerDivision::create(const std::string& filename)
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(filename))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
	return nullptr;
}

bool PlayerDivision::init(const std::string& filename)
{
	if (!Entity::initWithFile(filename))
	{
		return false;
	}

	return true;
}

PlayerDivision * PlayerDivision::create(const std::string &name, int vestmentID, int keywordID, int score)
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(name, vestmentID, keywordID, score))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
	return nullptr;
}

bool PlayerDivision::init(const std::string &name, int vestmentID, int keywordID, int score)//�����֣���Ƥ��
{
	_name = name;
	_vestmentID = vestmentID;
	_keywordID = keywordID;

	if (!Entity::initWithFile(StringUtils::format("public/keyword_%d.png", _keywordID)))//�����ַ���
	{
		return false;
	}

	_score = score;
	calculateData();

	return true;
}

//�붹����ײ���Ե�����
bool PlayerDivision::collideBean(Bean * bean)
{
	Vec2 position = this->getPosition();
	Vec2 beanPosition = bean->getPosition();
	if (beanPosition.distance(position) <= _radius)
	{
		eatBean(bean);
		return true;
	}

	return false;
}

//�ٶ�
float PlayerDivision::getSpeed()
{
	return _speed;
}

//�Ե�����
void PlayerDivision::eatBean(Bean * bean)
{
	_score += bean->getScore();
	calculateData();
}
//�Ե��з�
void PlayerDivision::eatRival(int score)
{
	_score += score;
	calculateData();
}

void PlayerDivision::scaleSprite()
{
	if (_score >= PLAYER_MIN_SHOW_VESTMENT_SCORE && !_showVestment)
	{
		_showVestment = true;
		std::string path = StringUtils::format("public/keyword_%d.png", _vestmentID);
		this->setTexture(path);
	}
	else if (_score<200 && _showVestment)
	{
		_showVestment = false;
		std::string path = StringUtils::format("public/keyword_%d.png", _keywordID);
		this->setTexture(path);
	}

	Size size = this->getContentSize();
	float scale = float(_radius * 2) / size.width;

	this->setScale(scale);
}

//���Ѳ���
void PlayerDivision::divide()
{
	_score /= 2;
	calculateData();

}


void PlayerDivision::addWeight(float w)
{
	_weight += w;

}

void PlayerDivision::setWeight(float w)
{
	_weight = w;

}

float PlayerDivision::getWeight()
{
	return _weight;
}

void PlayerDivision::setVelocity(Vec2 v)
{
	_velocity = v;
}

Vec2 PlayerDivision::getVelocity()
{
	return _velocity;
}



void PlayerDivision::setVestmentID(int id)
{
	_vestmentID = id;
}
void PlayerDivision::setPlayerName(const std::string name)
{
	_nameLabel = Label::createWithTTF(name.c_str(), "fonts/Marker Felt.ttf", 25);
	Size size = this->getContentSize();
	_nameLabel->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_nameLabel);
}

void PlayerDivision::calculateData()
{   //����뾶������������ȣ����ٶȣ�������ɷ��ȣ�
	_radius = sqrt(_score * PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS / PLAYER_INITIAL_SCORE);
	_speed = (PLAYER_INITIAL_RADIUS / _radius) * (PLAYER_INITIAL_SPEED - PLAYER_MIN_SPEED) + PLAYER_MIN_SPEED;

	this->setLocalZOrder(_score);//�ڵ���z���ϵ�����
	scaleSprite();//���þ���
}

void PlayerDivision::setScore(int score)
{
	_score = score;
	calculateData();
}

void PlayerDivision::setPrePosition(const Vec2 & position)
{
	_prePosition = position;
}

Vec2 PlayerDivision::getPrePosition()
{
	return _prePosition;
}