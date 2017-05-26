#include "PlayerDivision.h"
#include "Header\macro.h"
#include "Bean.h"
//玩家的分身
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

bool PlayerDivision::init(const std::string &name, int vestmentID, int keywordID, int score)//有名字，有皮肤
{
	_name = name;
	_vestmentID = vestmentID;
	_keywordID = keywordID;

	if (!Entity::initWithFile(StringUtils::format("public/keyword_%d.png", _keywordID)))//连接字符串
	{
		return false;
	}

	_score = score;
	calculateData();

	return true;
}

//与豆子碰撞，吃掉豆子
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

//速度
float PlayerDivision::getSpeed()
{
	return _speed;
}

//吃掉豆子
void PlayerDivision::eatBean(Bean * bean)
{
	_score += bean->getScore();
	calculateData();
}
//吃掉敌方
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

//分裂操作
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
{   //计算半径（与分数成正比）和速度（与分数成反比）
	_radius = sqrt(_score * PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS / PLAYER_INITIAL_SCORE);
	_speed = (PLAYER_INITIAL_RADIUS / _radius) * (PLAYER_INITIAL_SPEED - PLAYER_MIN_SPEED) + PLAYER_MIN_SPEED;

	this->setLocalZOrder(_score);//节点在z轴上的排序
	scaleSprite();//设置精灵
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