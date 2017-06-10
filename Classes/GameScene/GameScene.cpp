#include "GameScene.h"
#include "GameLayer.h"
enum GameZOrder
{
	GAME_BACKGROUND_Z,
	GAME_LAYER_Z,
	GAME_MENU_Z
};

Scene * GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//���Ѱ�ť
	auto divideItem = CheckBox::create(
		"dividebtn.png",
		"dividebtn.png");
	divideItem->setPosition(Vec2(720, 60));
	divideItem->setZoomScale(-0.1f);//һ��������ָ����ǰ�����ű���
	divideItem->addEventListener(CC_CALLBACK_2(GameScene::menuDivideCallback, this));
	this->addChild(divideItem, GAME_MENU_Z);

	auto gameLayer = GameLayer::create();
	this->addChild(gameLayer, GAME_LAYER_Z);
	return true;
}
void GameScene::menuDivideCallback(Ref * pSender, CheckBox::EventType type)
{
	_eventDispatcher->dispatchCustomEvent("Divide");
}
