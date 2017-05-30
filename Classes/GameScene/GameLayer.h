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

	virtual void update(float dt);		//游戏中每帧更新

	void initData();			//初始化地图数据
	void initRival(rapidjson::Value &value);		//初始化对手
	void initPlayer(rapidjson::Value &value);		//初始化玩家
	void initBean(int seed);						//初始化豆子

	//单机调试函数
	void initDataDefault();  //数据默认初始值
	void initRival();       //初始化对手
	void initPlayer();      //初始化玩家
	void initBean();        //初始化豆子

	void updateView();		//更新游戏视图
	void updateBean();		//更新豆子
	void updateRival();		//更新对手



	void collideBean(Player * player);		//与豆子的碰撞检测
	void collide();			//检测玩家与其他对手的碰撞

	void dividePlayer();		//分身操作
	void resetBean(Node * node);		//重置豆子
	void resetPlayer();        //玩家复活

	void synPlayerMove();		//同步玩家移动
	void synPlayerInfo();		//同步玩家信息
	void synPlayerConcentrate();		//同步玩家中合操作

		//处理服务器返回消息
	void playerMoveEvent(EventCustom * event);
	void playerDivideEvent(EventCustom * event);
	void enterPlayerEvent(EventCustom * event);
	void playerConcentrateEvent(EventCustom * event);
	void updatePlayerEvent(EventCustom * event);

private:
	joystick * _joystick;		//虚拟摇杆
	Node * _map;				//地图
	Player * _player;			//玩家
	Map<std::string, Player *> _rivalMap;		//对手列表
	Vector<Bean *> _beanList;			//豆子列表
	
	float _mapScale;			//地图缩放因子
	float _timeCount;
	int _mode;			//游戏模式
	int _roomID;		//房间id
};

#endif