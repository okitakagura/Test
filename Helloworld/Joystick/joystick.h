#pragma once

#ifndef __JOYSTICK__
#define __JOYSTICK__

#include "cocos2d.h"

USING_NS_CC;

class joystick :public Layer
{
public:
	joystick();
	~joystick();

	//CCtouch
	bool onTouchBegan(Touch*touch, Event*event);
	void ontouchMoved(Touch*touch, Event*event);
	void onTouchEnded(Touch*touch, Event*event);

	static joystick*create(const std::string&backgroundpath, const std::string thumbpath);

	//初始化
	virtual void init(const std::string&backgroundpath, const std::string&thumbpath);

	Vec2 getVelocity();//速度
	void updateVelocity(Vec2 point);

	void setdeadradius(float r);//中心不可触摸区域的半径,超出半径摇杆失效
	void setautocenter(bool b);//控制杆居中
	void setdirectionnum(int num);

	float round(float r);//四舍五入

protected:
	float _radius;//有效半径
	float _deadradius;//无效区域
	float _deadradius_sq;
	float _joystickradius;
	float _joystickradius_sq;
	int _directionnum;
	bool _autocenter;
	bool _direction;
	bool _hasdead;
	Vec2 _velocity;//速度

	Sprite* _thumb;//触点

};

#endif