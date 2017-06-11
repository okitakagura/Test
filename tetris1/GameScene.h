#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "SimpleAudioEngine.h"

#include "AudioEngine.h"

#include "cocos2d.h"

using namespace CocosDenshion;

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	// ��ʼ������
	void view();

	void createBlocks();//��������
	void createPreShape(int shape[4][2]);// ����δ������
	void changeShape();//�ı���״
	// ��ȡ����					
	float getX(int x);
	float getY(int y);
	int getIx(float x);
	int getIy(float y);
	
	// �Ƿ���Դ���
	bool isExist(int x, int y);


	void quickMove(float f);//�����ƶ�
	void downMove(float f);
	void moveBlocks(int num);//�����ƶ�

	// ��߸߶�
	int highest();

	bool isBoxStopDown(int tag);
	bool isBoxStopLeft(int tag);
	bool isBoxStopRight(int tag);

	// ����Ƿ��������
	void checkBox();

	// ��Ϸ����
	void GameOver();

	cocos2d::Vector<cocos2d::Sprite *> boxList;
	cocos2d::Vector<cocos2d::Sprite *> shapeList;

	void menuRestartCallback(cocos2d::Ref* pSender);
	void menuReturnCallback(cocos2d::Ref* pSender);
	friend class GameLay;
	CREATE_FUNC(GameScene);

private:
	int bHigh;
	float bSize;
	float width;
	float height;
	int shape;
	int preShape;
	int preColor;
	int level;
	int score;
	cocos2d::__Array * array1;

};

#endif // __GameScene_SCENE_H__
