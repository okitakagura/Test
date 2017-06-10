#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "Header\common.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class joystick;
class Player;
class Bean;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void onExit();

	bool onTouchBegan(Touch * touch, Event * event);
	void onTouchMoved(Touch * touch, Event * event);
	void onTouchEnded(Touch * touch, Event * event);
	void onTouchCancelled(Touch * touch, Event * event);

	virtual void update(float dt);		//��Ϸ��ÿ֡����

	void initData();			//��ʼ����ͼ����
	void initRival(rapidjson::Value &value);		//��ʼ������
	void initPlayer(rapidjson::Value &value);		//��ʼ�����
	void initBean(int seed);						//��ʼ������

	//�������Ժ���
	void initDataDefault();  //����Ĭ�ϳ�ʼֵ
	void initRival();       //��ʼ������
	void initPlayer();      //��ʼ�����
	void initBean();        //��ʼ������

	void updateView();		//������Ϸ��ͼ
	void updateBean();		//���¶���
	void updateRival();		//���¶���



	void collideBean(Player * player);		//�붹�ӵ���ײ���
	void collide();			//���������������ֵ���ײ

	void dividePlayer();		//�������
	void resetBean(Node * node);		//���ö���
	void resetPlayer();        //��Ҹ���

	void synPlayerMove();		//ͬ������ƶ�
	void synPlayerInfo();		//ͬ�������Ϣ
	void synPlayerConcentrate();		//ͬ������кϲ���

		//���������������Ϣ
	void playerMoveEvent(EventCustom * event);
	void playerDivideEvent(EventCustom * event);
	void enterPlayerEvent(EventCustom * event);
	void playerConcentrateEvent(EventCustom * event);
	void updatePlayerEvent(EventCustom * event);

private:
	joystick * _joystick;		//����ҡ��
	Node * _map;				//��ͼ
	Player * _player;			//���
	Map<std::string, Player *> _rivalMap;		//�����б�
	Vector<Bean *> _beanList;			//�����б�
	
	float _mapScale;			//��ͼ��������
	float _timeCount;
	int _mode;			//��Ϸģʽ
	int _roomID;		//����id
};

#endif