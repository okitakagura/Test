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
	//分裂按钮
	auto divideItem = CheckBox::create(
		"dividebtn.png",
		"dividebtn.png");
	divideItem->setPosition(Vec2(720, 60));
	divideItem->setZoomScale(-0.1f);//一个浮点数指定当前的缩放比例
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
