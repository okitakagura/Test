#include "login.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"

USING_NS_CC;
CCScene* LoginScene::scene()
{
	CCScene *scene = CCScene::create();
	LoginScene *layer = LoginScene::create();

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	//添加账号框
	auto pEditBox_name = EditBox::create(CCSizeMake(250, 50), Scale9Sprite::create("button.png"));
	pEditBox_name->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 3 / 4));
	pEditBox_name->setFontColor(Color3B(0, 0, 0));//字体颜色
	pEditBox_name->setPlaceHolder("Name:");//预置文本
	pEditBox_name->setMaxLength(8);//最长长度
	pEditBox_name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，不包括换行
	pEditBox_name->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD);//设置输入标志位
	pEditBox_name->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);//设置返回类型
	pEditBox_name->setDelegate(this);
	pEditBox_name->setTag(101);
	this->addChild(pEditBox_name);


	//添加密码框
	auto pEditBox_password = EditBox::create(CCSizeMake(250, 50), Scale9Sprite::create("button.png"));
	pEditBox_password->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 2 / 4));
	pEditBox_password->setFontColor(Color3B(0, 0, 0));//字体颜色
	pEditBox_password->setPlaceHolder("Password:");//预置文本
	pEditBox_password->setMaxLength(6);//最长长度
	pEditBox_password->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
	pEditBox_password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);//设置输入标志位
	pEditBox_password->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);//设置返回类型
	pEditBox_password->setDelegate(this);
	pEditBox_password->setTag(102);
	this->addChild(pEditBox_password);


	//添加确认键
	auto yesItem = MenuItemImage::create(
		"yes.png",
		"yes.png",
		CC_CALLBACK_1(LoginScene::menuYesCallback, this));

	yesItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 4));

	// create menu, it's an autorelease object
	auto menu = Menu::create(yesItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	return true;
}

void LoginScene::editBoxEditingDidBegin(EditBox * editBox)
{
	CCLOG("start edit");
}
void LoginScene::editBoxEditingDidEnd(EditBox* editbox)
{
	CCLOG("end edit");
}
void LoginScene::editBoxReturn(EditBox* editBox)
{
	CCLOG("editbox return");
}
void LoginScene::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	auto aditbox = (EditBox*)editBox;
	switch (editBox->getTag())
	{
	case 101:
		CCLOG("EditBox_name changed");
		break;
	case 102:
		CCLOG("EditBox_password changed");
		break;
	default:
		break;
	}
}
void LoginScene::menuYesCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainScene::createScene()));
}