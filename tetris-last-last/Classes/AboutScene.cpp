#include "AboutScene.h"

#include "StartScene.h"

#include <ui\UIImageView.h>



Scene* AboutScene::createScene()

{

	// 'scene' is an autorelease object

	auto scene = Scene::create();



	// 'layer' is an autorelease object

	auto layer = AboutScene::create();



	// add layer as a child to scene

	scene->addChild(layer);



	// return the scene

	return scene;

}



// on "init" you need to initialize your instance

bool AboutScene::init()

{

	//////////////////////////////

	// 1. super init first

	if (!Layer::init())

	{

		return false;

	}



	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//设置背景

	auto imageView = ui::ImageView::create("aboutbg.png");

	imageView->setPosition(Vec2(0, 0));

	imageView->setAnchorPoint(Vec2(0, 0));

	imageView->setScale(1.4f);

	this->addChild(imageView, 0);

	//设置返回键

	auto ReturnItem = MenuItemImage::create(

		"return.png",

		"return.png",

		CC_CALLBACK_1(AboutScene::menuReturnCallback, this));



	ReturnItem->setPosition(Vec2(400, 30));



	// create menu, it's an autorelease object

	auto menu = Menu::create(ReturnItem, NULL);

	menu->setPosition(Vec2::ZERO);

	this->addChild(menu, 1);


	


	return true;

}

void AboutScene::menuReturnCallback(cocos2d::Ref * pSender)

{

	Director::getInstance()->replaceScene(StartScene::createScene());

}