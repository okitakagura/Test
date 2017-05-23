#include "joystick.h"


joystick::joystick()
{
	_thumb = nullptr;
	_joystickradius = 0.0;
	_joystickradius_sq = 0.0;
	_deadradius = 0.0;
	_deadradius_sq = 0.0;
	_autocenter = false;
	_hasdead = false;
	_velocity = Vec2::ZERO;//相当于VEC2（0，0）
}

joystick::~joystick()
{

}

joystick*joystick::create(const std::string&backgroundpath, const std::string thumbpath)
{
	joystick*_joystick = new joystick();
	_joystick->init(backgroundpath, thumbpath);
	return _joystick;
}

void joystick::init(const std::string&backgroundpath, const std::string&thumbpath)
{
	auto _background = Sprite::create(backgroundpath.c_str());//创建控制杆的圆盘背景
	this->addChild(_background, 0);

	_thumb = Sprite::create(thumbpath.c_str());
	this->addChild(_thumb, 1);
}

Vec2 joystick::getVelocity()//关于速度
{
	return _velocity;
}

void joystick::updateVelocity(Vec2 point)
{
	float length = point.length();//计算向量的长度
	float length_sq = length*length;

	if (length_sq <= _deadradius_sq)//触碰无效的区域
	{
		_velocity = Vec2::ZERO;
		_thumb->setPosition(point);
		return;
	}

	float angle = atan2f(point.y, point.x);
	//atan2 返回的是方位角，也可以理解为计算复数 x+yi 的辐角,取值范围为（-π，π]
	if (angle < 0)//转换为正数
	{
		angle = angle + 3.14 * 2;
	}

	float anglePerSector = 2 * 3.14 / _directionnum;
	angle = round(angle / anglePerSector)*anglePerSector;//划分大致方向

	float cos = cosf(angle);
	float sin = sinf(angle);

	float dx, dy;

	if (length_sq > _joystickradius_sq)
	{
		dx = _joystickradius*cos;
		dy = _joystickradius*sin;
	}
	else
	{
		dx = length*cos;
		dy = length*sin;
	}

	_velocity = Vec2(dx, dy);
	_velocity.normalize();//归一化

	_thumb->setPosition(dx, dy);
}

void joystick::setdeadradius(float r)
{
	_deadradius = r;
	_deadradius = r*r;
	_hasdead = true;
}

void joystick::setautocenter(bool b)
{
	_autocenter = b;
}

float joystick::round(float r)
{
	if (floor(r + 0.5) != floor(r))
		return (floor(r + 0.5));
	else
		return floor(r);
}

//touch
bool joystick::onTouchBegan(Touch*touch, Event*event)
{
	Vec2 position = this->convertToNodeSpace(touch->getLocation);//转化为世界坐标
	if (position.lengthSquared() <= _joystickradius_sq)
	{
		this->updateVelocity(position);
		return true;
	}
	return false;
}

void joystick::ontouchMoved(Touch*touch, Event*event)
{
	Vec2 position = this->convertToNodeSpace(touch->getLocation);//转化为世界坐标
	this->updateVelocity(position);
}

void joystick::onTouchEnded(Touch*touch, Event*event)
{
	Vec2 position = Vec2::ZERO;
	if (!_autocenter)
	{
		position = this->convertToNodeSpace(touch->getLocation);//转化为世界坐标
	}
	this->updateVelocity(position);
}

void joystick::setdirectionnum(int num)
{
	_directionnum = num;
}