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

	//��ʼ��
	virtual void init(const std::string&backgroundpath, const std::string&thumbpath);

	Vec2 getVelocity();//�ٶ�
	void updateVelocity(Vec2 point);

	void setdeadradius(float r);//���Ĳ��ɴ�������İ뾶,�����뾶ҡ��ʧЧ
	void setautocenter(bool b);//���Ƹ˾���
	void setdirectionnum(int num);

	float round(float r);//��������

protected:
	float _radius;//��Ч�뾶
	float _deadradius;//��Ч����
	float _deadradius_sq;
	float _joystickradius;
	float _joystickradius_sq;
	int _directionnum;
	bool _autocenter;
	bool _direction;
	bool _hasdead;
	Vec2 _velocity;//�ٶ�

	Sprite* _thumb;//����

};

#endif