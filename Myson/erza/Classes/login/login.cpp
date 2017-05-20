#include "login.h"
#include "SimpleAudioEngine.h"

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

	
	//����˺ſ�
	auto pEditBox_name = EditBox::create(CCSizeMake(250, 50), Scale9Sprite::create("button.png"));
	pEditBox_name->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 3 / 4));
	pEditBox_name->setFontColor(Color3B(0, 0, 0));//������ɫ
	pEditBox_name->setPlaceHolder("Name:");//Ԥ���ı�
	pEditBox_name->setMaxLength(8);//�����
	pEditBox_name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);//���������κΣ�����������
	pEditBox_name->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD);//���������־λ
	pEditBox_name->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);//���÷�������
	pEditBox_name->setDelegate(this);
	pEditBox_name->setTag(101);
	this->addChild(pEditBox_name);


	//��������
	auto pEditBox_password = EditBox::create(CCSizeMake(250, 50), Scale9Sprite::create("button.png"));
	pEditBox_password->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 2 / 4));
	pEditBox_password->setFontColor(Color3B(0, 0, 0));//������ɫ
	pEditBox_password->setPlaceHolder("Password:");//Ԥ���ı�
	pEditBox_password->setMaxLength(6);//�����
	pEditBox_password->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
	pEditBox_password->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);//���������־λ
	pEditBox_password->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);//���÷�������
	pEditBox_password->setDelegate(this);
	pEditBox_password->setTag(102);
	this->addChild(pEditBox_password);

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