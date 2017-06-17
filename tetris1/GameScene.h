#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "cocos2d.h"
using namespace CocosDenshion;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);//按住键
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);//松开键
	void view();//界面
	void createBlocks();//创建方块
	void createPreShape(int shape[4][2]);// 创建未来方块
	void changeShape();//改变形状

	//坐标转换				
	float getX(int x);//转化为像素坐标
	float getY(int y);
	int getIx(float x);//转换为棋盘坐标
	int getIy(float y);

	int highest();//最高高度

	void checkBox();//检查是否可以消除

	void GameOver();//游戏结束

	cocos2d::Vector<cocos2d::Sprite *> boxList;//游戏界面里面的方块
	cocos2d::Vector<cocos2d::Sprite *> shapeList;//未来方块

	void menuRestartCallback(cocos2d::Ref* pSender);
	void menuReturnCallback(cocos2d::Ref* pSender);

	friend class GameLay;

	CREATE_FUNC(GameScene);


	bool in_out_x(int x[]);//判断是否超过界限
	bool in_out_y(int y[]);
	bool in_out_s(int tag);
	bool GameScene::change_position(int flag);//判断是否左右移动

	void clean(int a[], int number);//用来消去
	void downMove(float y);//向下移动
	void down(float y);
	void moveBlocks(float direction);//左右移动

private:
	int bSize;//方块的大小
	float width;
	float height;
	int shape;
	int shape_pre;
	int color_pre;
	int color;
	int level;
	int score;
	int dir;
	int dir_pre;
	float time;//刷新时间
};



#endif // __GameScene_SCENE_H__