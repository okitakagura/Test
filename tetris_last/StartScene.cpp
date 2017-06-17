#include "StartScene.h"

#include "GameLay.h"

#include "AboutScene.h"

#include <ui\UIImageView.h>





Scene* StartScene::createScene()

{

	// 'scene' is an autorelease object

	auto scene = Scene::create();



	// 'layer' is an autorelease object

	auto layer = StartScene::create();



	// add layer as a child to scene

	scene->addChild(layer);



	// return the scene

	return scene;

}



// on "init" you need to initialize your instance

bool StartScene::init()

{

	//////////////////////////////

	// 1. super init first

	if (!Layer::init())

	{

		return false;

	}



	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto imageView = ui::ImageView::create("startbg.png");

	imageView->setPosition(Vec2(0, 0));

	imageView->setAnchorPoint(Vec2(0, 0));

	this->addChild(imageView, 0);





	//ÉèÖÃ°´Å¥

	auto StartItem = MenuItemImage::create(

		"start.png",

		"start.png",

		CC_CALLBACK_1(StartScene::menuStartCallback, this));

	StartItem->setPosition(Vec2(380, 400));



	auto AboutItem = MenuItemImage::create(

		"About.png",

		"About.png",

		CC_CALLBACK_1(StartScene::menuAboutCallback, this));

	AboutItem->setPosition(Vec2(380, 250));



	auto QuitItem = MenuItemImage::create(

		"quit.png",

		"quit.png",

		CC_CALLBACK_1(StartScene::menuQuitCallback, this));

	QuitItem->setPosition(Vec2(380, 100));



	auto menu = Menu::create(StartItem, AboutItem, QuitItem, NULL);

	menu->setPosition(Point::ZERO);

	this->addChild(menu, 1);



	//ÉèÖÃ±êÌâ

	auto label = Label::createWithTTF("Tetris", "fonts/Marker Felt.ttf", 100);

	label->setColor(Color3B(245, 146, 71));

	// position the label on the center of the screen

	label->setPosition(Vec2(350, 500));



	// add the label as a child to this layer

	this->addChild(label, 1);

	return true;

}

void StartScene::menuStartCallback(cocos2d::Ref * pSender)

{

	Director::getInstance()->replaceScene(TransitionSplitRows::create(1.2f, GameLay::createScene()));



}

void StartScene::menuAboutCallback(cocos2d::Ref * pSender)

{

	Director::getInstance()->replaceScene(AboutScene::createScene());

}

void StartScene::menuQuitCallback(cocos2d::Ref * pSender)

{

	Director::getInstance()->end();



#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	exit(0);

#endif

}