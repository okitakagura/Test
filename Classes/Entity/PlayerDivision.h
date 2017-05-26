#ifndef _PlayDivision_H_
#define _PlayDivision_H_

#include"Entity.h"

class Bean;

class PlayerDivision : public Entity {
public:
	PlayerDivision();
	~PlayerDivision();
	//��ʼ��
	static PlayerDivision * create(const std::string& filename);
	bool init(const std::string& filename);

	static PlayerDivision * create(const std::string &name, int vestmentID, int keywordID, int score);
	bool init(const std::string &name, const int vestmentID, int keywordID, int score);

	bool collideBean(Bean * bean);//�붹����ײ


	float getSpeed();//�õ��ٶ�
	void calculateData();//����뾶���ٶ�

	void eatBean(Bean * bean);//�Զ���
	void eatRival(int score);//�Եз�

	virtual void setScore(int score);//���÷���
	
	
	void divide();//����һ��������������
	void scaleSprite();//�����鸽ͼ
	void setVestmentID(int id);
	void setPlayerName(const std::string name);
	void addWeight(float weight);
	void setWeight(float weight);
	float getWeight();
	void setVelocity(Vec2 v);   //(x,y)ʸ���ϵ��ٶ�
	Vec2 getVelocity();  //����ʸ���ٶ�

	void setPrePosition(const Vec2 & position);//������һ֡λ��ʸ��
	Vec2 getPrePosition();//������һ֡λ��ʸ��
private:
	Label * _nameLabel;
	std::string _name;
	int _vestmentID;
	int _keywordID;

	float _speed;
	float _weight;
	Vec2 _velocity;

	bool _showVestment;

	Vec2 _prePosition;  //��һ֡��λ��
};

#endif