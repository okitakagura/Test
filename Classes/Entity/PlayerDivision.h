#ifndef _PlayDivision_H_
#define _PlayDivision_H_

#include"Entity.h"

class Bean;

class PlayerDivision : public Entity {
public:
	PlayerDivision();
	~PlayerDivision();
	//初始化
	static PlayerDivision * create(const std::string& filename);
	bool init(const std::string& filename);

	static PlayerDivision * create(const std::string &name, int vestmentID, int keywordID, int score);
	bool init(const std::string &name, const int vestmentID, int keywordID, int score);

	bool collideBean(Bean * bean);//与豆子碰撞


	float getSpeed();//得到速度
	void calculateData();//计算半径和速度

	void eatBean(Bean * bean);//吃豆子
	void eatRival(int score);//吃敌方

	virtual void setScore(int score);//设置分数
	
	
	void divide();//分裂一半操作，分数变半
	void scaleSprite();//给精灵附图
	void setVestmentID(int id);
	void setPlayerName(const std::string name);
	void addWeight(float weight);
	void setWeight(float weight);
	float getWeight();
	void setVelocity(Vec2 v);   //(x,y)矢量上的速度
	Vec2 getVelocity();  //返回矢量速度

	void setPrePosition(const Vec2 & position);//设置上一帧位置矢量
	Vec2 getPrePosition();//返回上一帧位置矢量
private:
	Label * _nameLabel;
	std::string _name;
	int _vestmentID;
	int _keywordID;

	float _speed;
	float _weight;
	Vec2 _velocity;

	bool _showVestment;

	Vec2 _prePosition;  //上一帧的位置
};

#endif