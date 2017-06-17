#ifndef _GAMELAY_H 

#define _GAMELAY_H



#include "cocos2d.h"

class GameLay : public cocos2d::Layer

{

public:

	static cocos2d::Scene* createScene();

	virtual bool init();



	// a selector callback

	void easyStartCallback(cocos2d::Ref* pSender);

	void medicalStartCallback(cocos2d::Ref* pSender);

	void hardStartCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually

	CREATE_FUNC(GameLay);



};



#endif