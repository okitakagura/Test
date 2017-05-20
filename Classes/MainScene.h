//
//  MainScene.hpp
//  Argo
//
//  Created by yameng on 17/5/13.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
	cocos2d::LabelTTF* label;

	static cocos2d::Scene* createScene();

	virtual bool init();

	void startCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void introCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(MainScene);
};

#endif /* MainScene_hpp */#pragma once
