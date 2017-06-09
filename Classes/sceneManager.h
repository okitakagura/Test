#ifndef _SceneManager_H_
#define _SceneManager_H_

#include "cocos2d.h"
USING_NS_CC;

//�������������������������ת

class SceneManager : public Ref
{
public:
	enum SceneType
	{
		en_GameScene,		//��Ϸ����
		en_GameOverScene,	//��Ϸ��������	
	};

	static SceneManager* getInstance();
	virtual bool init();

	void changeScene(SceneType enSceneType);
private:
	static SceneManager * s_SceneManager;

};


#endif
