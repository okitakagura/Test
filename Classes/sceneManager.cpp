#include "sceneManager.h"
#include "GameScene\GameScene.h"

SceneManager * SceneManager::s_SceneManager = NULL;

SceneManager* SceneManager::getInstance()
{
	if (s_SceneManager == NULL)
	{
		s_SceneManager = new SceneManager();
		if (s_SceneManager && s_SceneManager->init())
		{
			s_SceneManager->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(s_SceneManager);
			s_SceneManager = NULL;
		}
	}
	return s_SceneManager;
}

bool SceneManager::init()
{
	return true;
}

void SceneManager::changeScene(SceneType enSceneType)
{
	Scene * scene = NULL;
	TransitionScene * ccts = NULL;

	if (enSceneType == en_GameScene)
	{
		scene = GameScene::createScene();
	}

	if (scene == NULL)
		return;

	auto pDirector = Director::getInstance();
	auto curScene = pDirector->getRunningScene();//获取当前场景的指针

	if (ccts == NULL)
		ccts = CCTransitionFadeTR::create(1.0f, scene);//跳转场景

	if (curScene == NULL)
		pDirector->runWithScene(scene);//启动第一个场景
	else
		pDirector->replaceScene(scene);//切换到下一个场景
}
