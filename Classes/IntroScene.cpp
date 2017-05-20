#include "IntroScene.h" 
#include "SimpleAudioEngine.h"
#include "MainScene.h"

USING_NS_CC;

Scene* IntroScene::createScene() {
	return IntroScene::create();
}

bool IntroScene::init() {
	//////////////////////////////
	// copyHW
	if (!Scene::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto mainlabel = Label::createWithTTF(
	"Agar.io is the worlds most popular game.\nGame played with huge number of other players in time.\n You need to eat colored dots as much as you can.\n You have to eat smaller dots.\nLargeer dots will kill you.\nThe game Agar.io provides great entertainment for you."
,
		"fonts/Marker Felt.ttf", 35);

	// position the label on the center of the screen
	mainlabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	// add the label as a child to this layer
	this->addChild(mainlabel, 1);

	auto makerlabel = Label::createWithTTF("producer:1652...", "fonts/Marker Felt.ttf", 35);

	// position the label on the center of the screen
	makerlabel->setPosition(Vec2(origin.x + makerlabel->getContentSize().width,
		origin.y + makerlabel->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(makerlabel, 1);
	
	auto backItem = MenuItemImage::create(
		"back.jpg",
		"back.jpg",
		CC_CALLBACK_1(IntroScene::backmainCallback, this));

	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2,
		origin.y + backItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	return true;
}

void IntroScene::backmainCallback(cocos2d::Ref* pSender)

{
	Director::getInstance()->replaceScene(TransitionFlipX::create(1.2f, MainScene::createScene()));
}