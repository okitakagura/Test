
#include "MainScene.h" // 名字需要更改
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h" //名字需要更改
#include "IntroScene.h" 
USING_NS_CC;


Scene* MainScene::createScene() {
	return MainScene::create();
}

bool MainScene::init() {
	//////////////////////////////
	// copyHW
	if (!Scene::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 背景 引用图片文件名
	auto background = Sprite::create("Background.jpeg");
	// 背景位置居中
	background->setPosition(Point(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);

	/////////////////////////////
	// 按钮 图片文件名1=常态时 图片文件名2=点击时
	auto startItem = MenuItemImage::create(
		"StartNormal.png",
		"StartClick.png",
		CC_CALLBACK_1(MainScene::startCallback, this));
	// 按钮位置居中
	startItem->setPosition(Point(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height * 3 / 4));

	
	// close button
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	//introduction button
	auto introItem = MenuItemImage::create(
		"IntroNormal.png",
		"IntroClick.png",
		CC_CALLBACK_1(MainScene::introCallback, this));

	introItem->setPosition(Point(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 4));
	
	// create menu, it's an autorelease object
	auto menu = Menu::create(startItem,closeItem,introItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
	return true;
}

void MainScene::startCallback(cocos2d::Ref* pSender) 
	// 切换到HW 改变createscene前的引用类即改变切换到哪个页面
{
	Director::getInstance()->replaceScene(TransitionSplitRows::create(1.2f, HelloWorld::createScene()));
}
void MainScene::menuCloseCallback(Ref* pSender)
    // Close the cocos2d-x game scene and quit the application.
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void MainScene::introCallback(cocos2d::Ref* pSender)
     //游戏说明
{
	Director::getInstance()->replaceScene(TransitionFlipX::create(1.2f, IntroScene::createScene()));
}
