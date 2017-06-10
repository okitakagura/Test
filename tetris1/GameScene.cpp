#include "GameScene.h"
#include "StartScene.h"
#include <ui\UIImageView.h>

USING_NS_CC;

// tag ����Ķ���
/*
100 101 x y ����ָʾ
102 103 104 105 �ĸ���ť

*/

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bHigh = 65;
	width = visibleSize.width;
	height = visibleSize.height;
	preShape = -1;
	preColor = -1;
	bSize = 31;
	level = 1;
	score = 0;

	this->view();

	// ע�Ჶ׽���� ����
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	// �ƶ���ʱ��	
	this->schedule(schedule_selector(GameScene::quickMove), 1.0f);

	// ��������
	createBlocks();
	return true;
}
void GameScene::view()
{
	// ���ñ���
	auto imageView = ui::ImageView::create("bg.png");
	imageView->setPosition(Vec2(0, 0));
	imageView->setAnchorPoint(Vec2(0, 0));
	float ix = imageView->getContentSize().width;
	float iy = imageView->getContentSize().height;
	imageView->setScaleX(480 / ix);
	imageView->setScaleY(600 / iy);
	this->addChild(imageView, 0);

	// �ȼ���ǩ
	Label* levelTitleLabel = Label::createWithTTF("Level", "fonts/arial.ttf", 25);
	levelTitleLabel->setColor(Color3B(240, 190, 95));
	levelTitleLabel->setAnchorPoint(Vec2(0, 1));
	levelTitleLabel->setPosition(5, height - 50);
	this->addChild(levelTitleLabel, 1);

	// �ȼ�����
	Label* levelLabel = Label::createWithTTF("1", "fonts/arial.ttf", 28);
	levelLabel->setColor(Color3B(240, 190, 95));
	levelLabel->setAnchorPoint(Vec2(0, 1));
	levelLabel->setPosition(10, height - 75);
	this->addChild(levelLabel, 1);
	levelLabel->setTag(95);

	// ������ǩ
	Label* scoreTitleLabel = Label::createWithTTF("Score", "fonts/arial.ttf", 25);
	scoreTitleLabel->setColor(Color3B(240, 190, 95));
	scoreTitleLabel->setAnchorPoint(Vec2(0, 1));
	scoreTitleLabel->setPosition(5, height - 110);
	this->addChild(scoreTitleLabel, 1);

	// ��������
	Label* scoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	scoreLabel->setColor(Color3B(240, 190, 95));
	scoreLabel->setAnchorPoint(Vec2(0, 1));
	scoreLabel->setPosition(10, height - 135);
	this->addChild(scoreLabel, 1);
	scoreLabel->setTag(96);

	//��߼�¼
	Label* hscoreTitleLabel = Label::createWithTTF("HighScore", "fonts/arial.ttf", 19);
	hscoreTitleLabel->setColor(Color3B(240, 190, 95));
	hscoreTitleLabel->setAnchorPoint(Vec2(0, 1));
	hscoreTitleLabel->setPosition(1, height - 170);
	this->addChild(hscoreTitleLabel, 1);

	int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
	Label* highscoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	auto highScore = Label::create(String::createWithFormat("%d", high)->getCString(), "fonts/arial.ttf", 28);
	highScore->setColor(Color3B(240, 190, 95));
	highScore->setAnchorPoint(Vec2(0, 1));
	highScore->setPosition(10, height - 210);
	this->addChild(highScore, 1);
}


void GameScene::createBlocks()
{
	int color;
	int ctime[7] = { 0,1,1,1,3,3,3 }; // �任����
	auto sframe = SpriteFrame::create("box", Rect(0, 0, bSize - 2, bSize - 2));
	int boxes[7][4][2] = {
		{ { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 3,0 } },
		{ { 1,1 },{ 0,0 },{ 1,0 },{ 2,1 } },
		{ { 1,1 },{ 0,1 },{ 1,0 },{ 2,0 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 1,1 } },
		{ { 1,0 },{ 0,0 },{ 0,1 },{ 2,0 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 2,1 } }
	};
	Color3B colors[6];
	colors[0] = Color3B(255, 0, 0); // ��
	colors[1] = Color3B(0, 255, 0); // ��
	colors[2] = Color3B(0, 0, 255); // ��
	colors[3] = Color3B(255, 255, 0); // ��
	colors[4] = Color3B(255, 0, 255); // ��
	colors[5] = Color3B(0, 255, 255); // ��

									  // ȡ����ɫ
	if (preColor != -1)
		color = preColor;
	else
		color = rand() % 6;
	// ȡ����״
	if (preShape != -1)
		shape = preShape;
	else
		shape = rand() % 7;


	// ��ȡ��״ʵ��

	for (int i = 0; i<4; i++)
	{
		Sprite* box = Sprite::createWithSpriteFrame(sframe);
		box->setColor(colors[color]);
		box->setAnchorPoint(Vec2(0, 0));
		box->setPosition(Vec2(getX(4 + boxes[shape][i][0]), getY(13 + boxes[shape][i][1])));
		this->addChild(box);
		box->setTag(110 + i);
	}

	preShape = rand() % 7;
	preColor = rand() % 6;
	createPreShape(boxes[preShape]);
	printf("preShape = %d", preShape);

}
void GameScene::createPreShape(int shape[4][2])
{
	int s, zx, zy;
	s = 16;
	zx = 410;
	zy = 460;
	auto sframe = SpriteFrame::create("box", Rect(0, 0, s - 1, s - 1));
	Color3B colors[6];
	colors[0] = Color3B(255, 0, 0); // ��
	colors[1] = Color3B(0, 255, 0); // ��
	colors[2] = Color3B(0, 0, 255); // ��
	colors[3] = Color3B(255, 255, 0); // ��
	colors[4] = Color3B(255, 0, 255); // ��
	colors[5] = Color3B(0, 255, 255); // ��

	for (int i = 0; i<shapeList.size(); i++)
	{
		auto box = shapeList.at(i);
		shapeList.eraseObject(box);
		box->removeFromParent();
		i--;
	}

	for (int i = 0; i<4; i++) {
		auto box = Sprite::createWithSpriteFrame(sframe);
		box->setColor(colors[preColor]);
		box->setAnchorPoint(Vec2(0, 0));
		box->setPosition(Vec2(zx + shape[i][0] * s, zy + shape[i][1] * s));
		this->addChild(box, 1);
		this->setTag(90 + i);
		shapeList.pushBack(box);
	}

}
// ����ת������
float GameScene::getX(int x)
{
	return 85 + x * 31;
}

float GameScene::getY(int y)
{
	return 130 + y * 31;
}
// ������������
int GameScene::getIx(float x)
{
	return (x - 85) / 31;
}

int GameScene::getIy(float y)
{
	return (y - 130) / 31;
}


void GameScene::quickMove(float f)
{
	moveBlocks(2);
}
void GameScene::downMove(float f)
{
	moveBlocks(2);
}

void GameScene::moveBlocks(int num)
{
	if (highest() < 13) { // ���߶ȳ���13 ���򲻿����ٻ�ȡ����

		Sprite* box[4];

	
		for (int i = 0; i<4; i++)
		{
			box[i] = (Sprite *)this->getChildByTag(110 + i); // �ڵ�����
		}

		int x = getIx(box[0]->getPositionX());
		int y = getIy(box[0]->getPositionY());
		if (num == 1) // �����ƶ�
		{
			// �ж��Ƿ��������
			if (!isBoxStopLeft(110) && !isBoxStopLeft(111) && !isBoxStopLeft(112) && !isBoxStopLeft(113)) {
				for (int i = 0; i<4; i++)
				{
					box[i]->setPositionX(getX(getIx(box[i]->getPositionX()) - 1));
				}
			}
		}
		else if (num == 3)
		{
			// �ж��Ƿ��������
			if (!isBoxStopRight(110) && !isBoxStopRight(111) && !isBoxStopRight(112) && !isBoxStopRight(113)) {
				for (int i = 0; i<4; i++)
				{
					box[i]->setPositionX(getX(getIx(box[i]->getPositionX()) + 1));
				}

			}
		}
		else if (num == 4)
		{
			//if(y < 10){}
		}
		else if (num == 2) // �����ƶ�
		{

			// �ж��Ƿ񵽵ף����·��Ƿ����ϰ���
			if (isBoxStopDown(110) || isBoxStopDown(111) || isBoxStopDown(112) || isBoxStopDown(113)) {
				for (int i = 0; i<4; i++)
				{
					box[i]->setTag(120); // ʹ֮�����ƶ�
					this->boxList.pushBack(box[i]); // ��ӵ������ƶ���������
				}
				// �����µķ���
				// ����box�������鵱��
				// �ڴ�������֮���ж��Ƿ��������һ��

				//printf("High is %d\n",highest()); // 14 �䲻��������Ȼ����ΪcheckBox()��ԭ��
				if (highest() < 13) { // ĳ��ʱ�̣�hightest() == 12
					checkBox(); // Game Over ֮����ִ�еĻ��ᱨ��
					createBlocks();
				}
				else {

					GameOver();
				}


				// ֹͣ��������
				this->unschedule(schedule_selector(GameScene::downMove));
			}
			else {
				for (int i = 0; i<4; i++)
				{
					box[i]->setPositionY(getY(getIy(box[i]->getPositionY()) - 1));
				} // �ƶ�
			}
		}

	}
}

int GameScene::highest()
{
	int h = 0;
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* box = boxList.at(i);
		int y = getIy(box->getPositionY());
		if (y > h)
			h = y;
	}
	return h;
}

// �Ƿ�ֹͣ����
bool GameScene::isBoxStopDown(int tag)
{
	Sprite* box = (Sprite *)this->getChildByTag(tag); // �ڵ�����
	Point pa;
	bool flag = false;
	pa.x = getIx(box->getPositionX());
	pa.y = getIy(box->getPositionY());
	// ����boxList
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* mbox = boxList.at(i);
		Point pb;
		pb.x = getIx(mbox->getPositionX());
		pb.y = getIy(mbox->getPositionY());
		if (pa.y - pb.y == 1 && pa.x == pb.x)
		{
			flag = true;
		}
	}
	if (pa.y <= 0)
		flag = true;
	return flag;
}

// �Ƿ�ֹͣ����
bool GameScene::isBoxStopLeft(int tag)
{
	Sprite* box = (Sprite *)this->getChildByTag(tag); // �ڵ�����
	Point pa;
	bool flag = false;
	pa.x = getIx(box->getPositionX());
	pa.y = getIy(box->getPositionY());
	// ����boxList
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* mbox = boxList.at(i);
		Point pb;
		pb.x = getIx(mbox->getPositionX());
		pb.y = getIy(mbox->getPositionY());
		if (pa.x - pb.x == 1 && pa.y == pb.y)
		{
			flag = true;
		}
	}
	if (pa.x <= 0)
		flag = true;
	return flag;
}

// �Ƿ�ֹͣ����
bool GameScene::isBoxStopRight(int tag)
{
	Sprite* box = (Sprite *)this->getChildByTag(tag); // �ڵ�����
	Point pa;
	bool flag = false;
	pa.x = getIx(box->getPositionX());
	pa.y = getIy(box->getPositionY());
	// ����boxList
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* mbox = boxList.at(i);
		Point pb;
		pb.x = getIx(mbox->getPositionX());
		pb.y = getIy(mbox->getPositionY());
		if (pa.x - pb.x == -1 && pa.y == pb.y)
		{
			flag = true;
		}
	}
	if (pa.x >= 9)
		flag = true;
	return flag;
}


// ����Ƿ�������
void GameScene::checkBox()
{
	// 10�����飬������ֵ��ȣ�������Ϊ1-10
	// ����ת��:
	bool fk[13][10] = { false }; // ʮ���У�ʮ��
	int  del[4];
	int  dx = 0;
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* box = boxList.at(i);
		int x = getIx(box->getPositionX());
		int y = getIy(box->getPositionY());
		fk[y][x] = true;
	} // ѭ������

	for (int i = 0; i<12; i++)
	{
		int flag = 0;
		for (int j = 0; j<10; j++)
		{
			if (fk[i][j] == true)
			{
				flag++;
			}
			else {
			}
		}
		// �� flag �ۼӵ�10����ɾ����i��
		if (flag == 10) {

			del[dx] = i; // ��¼��Ҫ�Ƴ���������
			dx++; // ��¼�����������
		}
	}

	// �Ƴ�boxList �е� box
	//int size = boxList.size(); �������鳬��
	for (int i = 0; i < boxList.size(); i++)
	{
		auto box = boxList.at(i);
		int y = getIy(box->getPositionY());
		int x = getIx(box->getPositionX());
		for (int j = 0; j<dx; j++)
		{
			if (y == del[j]) {
				boxList.eraseObject(box);
				box->removeFromParent();
				i--;
					 // Ҫ���ʵ���һ��Ԫ��ǰ����                                                                                                                                                                                                                                                                                                                                                                                                                                                              
			}
		}
	}
	// ѭ������
	int temp;
	for (int i = 0; i<dx - 1; i++) {
		for (int j = 0; j<dx - 1 - i; j++) {
			if (del[j + 1] > del[j]) {
				temp = del[j];
				del[j] = del[j + 1];
				del[j + 1] = temp;
			}
		}
	}

	// ����box
	for (int i = 0; i < boxList.size(); i++)
	{
		auto box = boxList.at(i);
		int y = getIy(box->getPositionY());
		int x = getIx(box->getPositionX());
		// ��һ��������ж���ƶ�
		for (int j = 0; j<dx; j++)
		{
			if (y > del[j]) {
				y--; // y��ֵ�����˸ı�
				box->setPositionY(getY(y));
			}
		}
	}

	// �������
	if (dx == 1) {
		score += 10;
	}
	else if (dx == 2) {
		score += 30 + (level - 1) * 10;
	}
	else if (dx == 3) {
		score += 60 + (level - 1) * 20;
	}
	else if (dx == 4) {
		score += 100 + (level - 1) * 30;
	}
	// �ı����
	Label* scoreLabel = (Label*)this->getChildByTag(96);
	scoreLabel->setString(String::createWithFormat("%d", score)->_string);
	// �ı�ȼ�
	Label* levelLabel = (Label*)this->getChildByTag(95);
	// 100 250 500 950 1300 1850 
	int upScore = level * 100 + (level - 1) * (level - 1) * 50; // �����ɵȼ�����
	if (score > upScore) {
		level++;
		levelLabel->setString(String::createWithFormat("%d", level)->_string);
		this->schedule(schedule_selector(GameScene::quickMove), 1.0f / level);
	}
}
// �����¼�
void GameScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)  //��
	{
		moveBlocks(2);
		// ���̼��ٶ�ʱ�� �ӳ� 0.5s ִ��
		this->schedule(schedule_selector(GameScene::downMove), 0.005f, 12, 0.5f);

	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)  //��
	{
		moveBlocks(1);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)  //��
	{
		moveBlocks(3);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW)  //��
	{
		moveBlocks(4);
		// ������ı���̬
		changeShape();
	}

}
void GameScene::changeShape()
{
	if (highest() < 13) {
		int ctime[7] = { 0,1,1,1,3,3,3 }; // �任����
		static int flag = 0; // ��ת
		int bx[4], by[4], fx[4], fy[4];
		Sprite* boxes[4];
		for (int i = 0; i<4; i++) {
			boxes[i] = (Sprite *)this->getChildByTag(110 + i);
			bx[i] = getIx(boxes[i]->getPositionX());
			by[i] = getIy(boxes[i]->getPositionY());
		}
		// �õ��˷�����ĸ�����
		if (ctime[shape] == 0) {
			// ���ñ任
		}
		else if (ctime[shape] == 1) {
			// ���α任
			if (flag == 0) { // ��ת
				for (int i = 0; i<4; i++) {
					fx[i] = bx[0] - (by[0] - by[i]);
					fy[i] = by[0] + (bx[0] - bx[i]);
				}
				flag = 1;
			}
			else if (flag == 1) { // ��ת
				for (int i = 0; i<4; i++) {
					fx[i] = bx[0] + (by[0] - by[i]);
					fy[i] = by[0] - (bx[0] - bx[i]);
				}
				flag = 0;
			}
			// ��ת
			if (isExist(fx[0], fy[0]) && isExist(fx[1], fy[1]) && isExist(fx[2], fy[2]) && isExist(fx[3], fy[3])) {
				for (int i = 0; i<4; i++) {
					boxes[i]->setPositionX(getX(fx[i]));
					boxes[i]->setPositionY(getY(fy[i]));
				}
			}
			else {
				flag = (flag == 1) ? 0 : 1; // ȡ��
			}
		}
		else if (ctime[shape] == 3) {
			// �Ĵα任 ������ת
			for (int i = 0; i<4; i++) {
				fx[i] = bx[0] - (by[0] - by[i]);
				fy[i] = by[0] + (bx[0] - bx[i]);
			}
			// �õ��ĸ�����,�ж��ĸ������Ƿ���Դ���
			if (isExist(fx[0], fy[0]) && isExist(fx[1], fy[1]) && isExist(fx[2], fy[2]) && isExist(fx[3], fy[3])) {
				for (int i = 0; i<4; i++) {
					boxes[i]->setPositionX(getX(fx[i]));
					boxes[i]->setPositionY(getY(fy[i]));
				}
			}
		}

	}
}

// �ص����
bool GameScene::isExist(int x, int y)
{
	bool flag = true;
	for (int i = 0; i<boxList.size(); i++)
	{
		Sprite* mbox = boxList.at(i);
		int bx = getIx(mbox->getPositionX());
		int by = getIy(mbox->getPositionY());
		if ((bx == x && by == y))
		{
			flag = false;
		}
	}
	if (x < 0 || x > 9 || y < 0)
		flag = false;
	printf("x = %d y = %d\n", x, y);
	return flag;
}

void GameScene::GameOver()
{
	// ֹͣ��������
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	auto overlabel = Label::createWithTTF("GAME OVER", "fonts/Marker Felt.ttf", 90);
	overlabel->setColor(Color3B(245, 146, 71));
	// position the label on the center of the screen
	overlabel->setPosition(Vec2(240, 500));
	// add the label as a child to this layer
	this->addChild(overlabel, 1);

	std::string printfscore = StringUtils::format("score:%d",score);
	auto scorelabel = Label::createWithTTF(printfscore, "fonts/Marker Felt.ttf", 50);
	scorelabel->setColor(Color3B(245, 146, 71));
	// position the label on the center of the screen
	scorelabel->setPosition(Vec2(240, 405));
	// add the label as a child to this layer
	this->addChild(scorelabel, 1);

	UserDefault * userDefault = UserDefault::getInstance();
	int lastScore = userDefault->getIntegerForKey("HighScore");
	if (score > lastScore)
	{
		userDefault->setIntegerForKey("HighScore", score);
		userDefault->getInstance()->flush();
	}
	// ���¿�ʼ
	auto restartItemImage = MenuItemImage::create("restart.png", "restart.png",
		CC_CALLBACK_1(GameScene::menuRestartCallback, this));
	auto restartMenu = Menu::create(restartItemImage, NULL);
	restartMenu->setAnchorPoint(Vec2(0.5, 0.5));
	restartMenu->setPosition(Vec2(240, 300));
	this->addChild(restartMenu, 1);

	// ����
	auto returnItemImage = MenuItemImage::create("returnhome.png", "returnhome.png",
		CC_CALLBACK_1(GameScene::menuReturnCallback, this));
	auto returnMenu = Menu::create(returnItemImage, NULL);
	returnMenu->setAnchorPoint(Vec2(0.5, 0.5));
	returnMenu->setPosition(Vec2(240, 200));
	this->addChild(returnMenu, 1);
}
void GameScene::menuRestartCallback(Ref *pSender)
{

	Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameScene::menuReturnCallback(Ref *pSender)
{

	Director::getInstance()->replaceScene(StartScene::createScene());
}
