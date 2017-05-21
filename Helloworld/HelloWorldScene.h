#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public CCLayer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	virtual void onTouchesMoved(__Set *pTouches,Event *pEvent);

private:
	//定义精灵指针
	CCSprite* m_pSprite;
};


#endif // __HELLOWORLD_SCENE_H__
