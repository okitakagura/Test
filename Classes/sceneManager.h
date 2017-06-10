#ifndef _SceneManager_H_
#define _SceneManager_H_

#include "cocos2d.h"
USING_NS_CC;

//场景管理器，负责各个场景跳转

class SceneManager : public Ref
{
public:
	enum SceneType
	{
		en_GameScene,		//游戏场景
		en_GameOverScene,	//游戏结束场景	
	};

	static SceneManager* getInstance();
	virtual bool init();

	void changeScene(SceneType enSceneType);
private:
	static SceneManager * s_SceneManager;

};


#endif
