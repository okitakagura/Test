#ifndef __ABOUT_SCENE_H__
#define __ABOUT_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class AboutScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void menuReturnCallback(cocos2d::Ref* pSender);


	// implement the "static create()" method manually
	CREATE_FUNC(AboutScene);
};

#endif 
