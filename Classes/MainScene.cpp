
#include "MainScene.h" // ������Ҫ����
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h" //������Ҫ����
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

	// ���� ����ͼƬ�ļ���
	auto background = Sprite::create("Background.jpeg");
	// ����λ�þ���
	background->setPosition(Point(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(background);

	/////////////////////////////
	// ��ť ͼƬ�ļ���1=��̬ʱ ͼƬ�ļ���2=���ʱ
	auto startItem = MenuItemImage::create(
		"StartNormal.png",
		"StartClick.png",
		CC_CALLBACK_1(MainScene::startCallback, this));
	// ��ťλ�þ���
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
	// �л���HW �ı�createsceneǰ�������༴�ı��л����ĸ�ҳ��
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
     //��Ϸ˵��
{
	Director::getInstance()->replaceScene(TransitionFlipX::create(1.2f, IntroScene::createScene()));
}
