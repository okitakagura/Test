#ifndef __START_SCENE_H__

#define __START_SCENE_H__



#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#include "AudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;



class StartScene : public cocos2d::Layer

{

public:

	static cocos2d::Scene* createScene();



	virtual bool init();



	void menuStartCallback(cocos2d::Ref * pSender);

	void menuQuitCallback(cocos2d::Ref * pSender);

	void menuAboutCallback(cocos2d::Ref * pSender);

	// implement the "static create()" method manually

	CREATE_FUNC(StartScene);
	
	//¿ª¹ØÉùÒô
	void SoundOnOffClick(cocos2d::Ref* pSender);

	bool isSoundOpen;

};



#endif // __START_SCENE_H__

