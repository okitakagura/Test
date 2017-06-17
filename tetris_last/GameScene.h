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
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);//��ס��
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);//�ɿ���
	void view();//����
	void createBlocks();//��������
	void createPreShape(int shape[4][2]);// ����δ������
	void changeShape();//�ı���״

	//����ת��				
	float getX(int x);//ת��Ϊ��������
	float getY(int y);
	int getIx(float x);//ת��Ϊ��������
	int getIy(float y);

	int highest();//��߸߶�

	void checkBox();//����Ƿ��������

	void GameOver();//��Ϸ����

	cocos2d::Vector<cocos2d::Sprite *> boxList;//��Ϸ��������ķ���
	cocos2d::Vector<cocos2d::Sprite *> shapeList;//δ������

	void menuRestartCallback(cocos2d::Ref* pSender);
	void menuReturnCallback(cocos2d::Ref* pSender);

	friend class GameLay;

	CREATE_FUNC(GameScene);


	bool in_out_x(int x[]);//�ж��Ƿ񳬹�����
	bool in_out_y(int y[]);
	bool in_out_s(int tag);
	bool GameScene::change_position(int flag);//�ж��Ƿ������ƶ�

	void clean(int a[], int number);//������ȥ
	void downMove(float y);//�����ƶ�
	void down(float y);
	void moveBlocks(float direction);//�����ƶ�

private:
	int bSize;//����Ĵ�С
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
	float time;//ˢ��ʱ��
};



#endif // __GameScene_SCENE_H__