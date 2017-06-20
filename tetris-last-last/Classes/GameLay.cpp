#include "GameLay.h"

#include "GameScene.h"



USING_NS_CC;

int pattern;

Scene* GameLay::createScene()

{

	// 'scene' is an autorelease object

	auto scene = Scene::create();



	// 'layer' is an autorelease object

	auto layer = GameLay::create();



	// add layer as a child to scene

	scene->addChild(layer);



	// return the scene

	return scene;

}



// on "init" you need to initialize your instance

bool GameLay::init()

{

	//////////////////////////////

	// 1. super init first

	if (!Layer::init())

	{

		return false;

	}



	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();



	//设置模式

	auto easyItem = MenuItemImage::create(

		"easy.png",

		"easy.png",

		CC_CALLBACK_1(GameLay::easyStartCallback, this));

	easyItem->setPosition(Vec2(240, 400));

	easyItem->setScale(1.2f);



	auto medItem = MenuItemImage::create(

		"challenge.png",

		"challenge.png",

		CC_CALLBACK_1(GameLay::medicalStartCallback, this));

	medItem->setPosition(Vec2(240, 250));

	medItem->setScale(1.2f);



	auto hardItem = MenuItemImage::create(

		"hard.png",

		"hard.png",

		CC_CALLBACK_1(GameLay::hardStartCallback, this));

	hardItem->setPosition(Vec2(240, 100));

	hardItem->setScale(1.2f);



	auto menu = Menu::create(easyItem, medItem, hardItem, NULL);

	menu->setPosition(Point::ZERO);

	this->addChild(menu, 1);





	return true;

}



void GameLay::easyStartCallback(cocos2d::Ref* pSender)

{

	pattern = 1;

	Director::getInstance()->replaceScene(TransitionFade::create(1.2f, GameScene::createScene()));

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("music2.mp3", true);

}



void GameLay::medicalStartCallback(cocos2d::Ref* pSender)

{

	pattern = 2;

	Director::getInstance()->replaceScene(TransitionFade::create(1.2f, GameScene::createScene()));

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("music2.mp3", true);

}

void GameLay::hardStartCallback(cocos2d::Ref* pSender)

{

	pattern = 3;

	Director::getInstance()->replaceScene(TransitionFade::create(1.2f, GameScene::createScene()));

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("music2.mp3", true);

}