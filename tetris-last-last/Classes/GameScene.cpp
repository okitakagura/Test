#include "GameScene.h"
#include "StartScene.h"
#include "GameLay.h"
#include <ui\UIImageView.h>
USING_NS_CC;

Scene* GameScene::createScene()
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
	Size visibleSize = Director::getInstance()->getVisibleSize();//界面大小
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	bSize = 31;//每个小方块大小
	width = visibleSize.width;
	height = visibleSize.height;
	shape = -1;
	color = -1;
	level = 1;
	score = 0;


	// 注册捕捉监听 键盘
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	this->view();

	extern int pattern;//难度的判断
	int i = pattern;
	if (i == 1) {
		time = 1.0f;
		this->schedule(schedule_selector(GameScene::downMove), 1.1f);
	}
	else if (i == 2) {
		time = 0.6f;
		this->schedule(schedule_selector(GameScene::downMove), 0.7f);
	}
	else if (i == 3) {
		time = 0.4f;
		this->schedule(schedule_selector(GameScene::downMove), 0.5f);
	}

	createBlocks();//创建方块

	return true;

}

//初始化场景
void GameScene::view()

{
	auto imageView = ui::ImageView::create("bg.png");
	imageView->setPosition(Vec2(0, 0));
	imageView->setAnchorPoint(Vec2(0, 0));
	float ix = imageView->getContentSize().width;
	float iy = imageView->getContentSize().height;
	imageView->setScaleX(480 / ix);
	imageView->setScaleY(600 / iy);
	this->addChild(imageView, 0);


	CCSprite *image = CCSprite::create("picture1.png");
	image->setPosition(Vec2(375, 125));
	image->setAnchorPoint(Vec2(0, 0));
	image->setScale(0.8f);
	this->addChild(image, 0);
	image->setTag(888);

	isSoundOpen = false;
	//添加声音按钮
	CCMenuItemImage *soundbtnOn = CCMenuItemImage::create("soundon2.png", "soundon2.png");
	CCMenuItemImage *soundbtnOff = CCMenuItemImage::create("soundoff2.png", "soundoff2.png");
	CCMenuItemToggle *toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(GameScene::SoundOnOff), soundbtnOn, soundbtnOff, NULL);
	toggle->setPosition(CCPointMake(0, 0));
	CCMenu *menu1 = CCMenu::create(toggle, NULL);
	menu1->setPosition(ccp(400, 30));
	this->addChild(menu1);


	// NEXT标签
	Label* NextLabel = Label::createWithTTF("NEXT", "fonts/arial.ttf", 29);
	NextLabel->setColor(Color3B(255, 255, 255));
	NextLabel->setAnchorPoint(Vec2(0, 1));
	NextLabel->setPosition(380, 540);
	this->addChild(NextLabel, 1);

	// 等级标签
	Label* levelTitleLabel = Label::createWithTTF("Level", "fonts/arial.ttf", 25);
	levelTitleLabel->setColor(Color3B(255, 255, 255));
	levelTitleLabel->setAnchorPoint(Vec2(0, 1));
	levelTitleLabel->setPosition(5, 120);
	this->addChild(levelTitleLabel, 1);

	// 等级数字
	Label* levelLabel = Label::createWithTTF("1", "fonts/arial.ttf", 28);
	levelLabel->setColor(Color3B(255, 255, 255));
	levelLabel->setAnchorPoint(Vec2(0, 1));
	levelLabel->setPosition(100, 120);
	this->addChild(levelLabel, 1);
	levelLabel->setTag(95);

	// 分数标签
	Label* scoreTitleLabel = Label::createWithTTF("Score", "fonts/arial.ttf", 25);
	scoreTitleLabel->setColor(Color3B(255, 255, 255));
	scoreTitleLabel->setAnchorPoint(Vec2(0, 1));
	scoreTitleLabel->setPosition(5, 80);
	this->addChild(scoreTitleLabel, 1);

	// 分数数字
	Label* scoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	scoreLabel->setColor(Color3B(255, 255, 255));
	scoreLabel->setAnchorPoint(Vec2(0, 1));
	scoreLabel->setPosition(100, 80);
	this->addChild(scoreLabel, 1);
	scoreLabel->setTag(96);

	//最高纪录
	Label* hscoreTitleLabel = Label::createWithTTF("HighScore", "fonts/arial.ttf", 19);
	hscoreTitleLabel->setColor(Color3B(255, 255, 255));
	hscoreTitleLabel->setAnchorPoint(Vec2(0, 1));
	hscoreTitleLabel->setPosition(5, 40);
	this->addChild(hscoreTitleLabel, 1);

	int high = UserDefault::getInstance()->getIntegerForKey("HighScore", 0);
	Label* highscoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	auto highScore = Label::create(String::createWithFormat("%d", high)->getCString(), "fonts/arial.ttf", 28);
	highScore->setColor(Color3B(255, 255, 255));
	highScore->setAnchorPoint(Vec2(0, 1));
	highScore->setPosition(100, 40);
	this->addChild(highScore, 1);

}

//声音的开启与关闭
void GameScene::SoundOnOff(cocos2d::Ref* pSender)
{
	if (isSoundOpen)
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		isSoundOpen = false;
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		isSoundOpen = true;
	}
}

//创建方块
void GameScene::createBlocks()

{
	//方块的7种模式
	int boxes[7][4][2] = {
		{ { 0,0 },{ 1,0 },{ 0,1 },{ 1,1 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 3,0 } },
		{ { 1,1 },{ 0,0 },{ 1,0 },{ 2,1 } },
		{ { 1,1 },{ 0,1 },{ 1,0 },{ 2,0 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 1,1 } },
		{ { 1,0 },{ 0,0 },{ 0,1 },{ 2,0 } },
		{ { 1,0 },{ 0,0 },{ 2,0 },{ 2,1 } }
	};
	//判断随机，以及与上一步判断的未来方块能一致
	if (shape != -1)
		shape = shape_pre;
	else
		shape = rand() % 7;

	if (color != -1)
	{
		color = color_pre;
	}
	else
		color = rand() % 7;

	for (int i = 0; i<4; i++)
	{
		auto box = Sprite::create(String::createWithFormat("hlw%d.png", color + 1)->_string);//根据color显示不同的图片

		box->setAnchorPoint(Vec2(0, 0));
		box->setPosition(Vec2(getX(4 + boxes[shape][i][0]), getY(13 + boxes[shape][i][1])));
		box->setScale(0.75, 0.75);
		this->addChild(box);
		box->setTag(110 + i);
	}


	shape_pre = rand() % 7;
	color_pre = rand() % 7;

	createPreShape(boxes[shape_pre]);
}

//创建未来方块
void GameScene::createPreShape(int shape[4][2])//参数为行数（shape）
{
	int s, zx, zy;
	s = 27;
	zx = 380;
	zy = 460;

	for (int i = 0; i<shapeList.size(); i++)//消除刚才显示的未来方块，不然会重叠覆盖
	{
		auto box = shapeList.at(i);
		shapeList.eraseObject(box);
		box->removeFromParent();
		i--;
	}

	for (int i = 0; i<4; i++) {
		auto box = Sprite::create(String::createWithFormat("hlw%d.png", color_pre + 1)->_string);
		box->setScale(0.65, 0.65);
		box->setPosition(Vec2(zx + shape[i][0] * s, zy + shape[i][1] * s));//位置
		this->addChild(box, 1);
		this->setTag(90 + i);
		shapeList.pushBack(box);
	}
}

// 数字转换坐标

float GameScene::getX(int x)
{
	return 50 + x * 31;
}

float GameScene::getY(int y)
{

	return 130 + y * 31;

}

// 返回整形坐标

int GameScene::getIx(float x)
{
	return (x - 50) / 31;
}

int GameScene::getIy(float y)
{
	return (y - 130) / 31;
}

//判断最高点
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

// 检测是否可以清除
void GameScene::checkBox()

{
	// 数据转换:
	int del[4];
	int num = 0;
	int check;
	bool square[10][14];
	for (int i = 0; i < boxList.size(); i++)//转化为棋盘
	{
		cocos2d::Sprite*box = boxList.at(i);
		square[getIx(box->getPositionX())][getIy(box->getPositionY())] = true;
	}
	for (int i = 0; i<13; i++)//判断是否达成一行是个都为true的情况，存在这个情况就应当消去记录
	{
		check = 0;
		for (int j = 0; j < 10; j++)
		{
			if (square[j][i] == true)
			{
				check++;
			}
		}
		if (check == 10)
		{
			del[num] = i;
			num++;//可消去的行数，一次性最多可消4行
		}
	}

	clean(del, num);

	if (highest() >= 8)
	{
		//更换图片 
		Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("timg.png");
		CCSprite *image = (CCSprite *)this->getChildByTag(888);
		image->setTexture(texture);
	}

	
	// 计算分数

	switch (num)
	{
	case 1:score += 10; break;
	case 2:score += 25; break;
	case 3:score += 40; break;
	case 4:score += 60; break;
	}

	// 改变分数

	Label* scoreLabel = (Label*)this->getChildByTag(96);

	scoreLabel->setString(String::createWithFormat("%d", score)->_string);

	// 改变等级

	Label* levelLabel = (Label*)this->getChildByTag(95);

	// 100 250 500 950 1300 1850 

	int upScore = level * 100 + (level - 1) * (level - 1) * 50; // 分数由等级决定

	if (score >= upScore) {

		if (level < 5)
		{
			level++;
		}

		levelLabel->setString(String::createWithFormat("%d", level)->_string);

		this->schedule(schedule_selector(GameScene::downMove), (time / level) + 0.1f);//改变速度

	}

}

//清除
void GameScene::clean(int a[], int number)
{
	for (int n = 0; n < boxList.size(); n++)
	{
		cocos2d::Sprite*box = boxList.at(n);
		int y = getIy(box->getPositionY());
		int x = getIx(box->getPositionX());

		for (int h = number - 1; h>-1; h--)
		{
			if (y == a[h])
			{
				boxList.eraseObject(box);
				box->removeFromParent();//将这个节点从这个体系里面完全消除
				n--;
			}
		}
	}
	for (int i = 0; i < boxList.size(); i++)

	{

		auto box = boxList.at(i);

		int y = getIy(box->getPositionY());

		int x = getIx(box->getPositionX());

		// 对一个方格进行多次移动

		for (int j = number - 1; j >= 0; j--)

		{

			if (y > a[j]) {

				y--; // y的值发生了改变

				box->setPositionY(getY(y));

			}

		}

	}


}

// 按键事件
void GameScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)  //下
	{
		down(1);
		// 键盘加速定时器 延迟 0.1s 执行
		this->schedule(schedule_selector(GameScene::down), 0.1f, 12, 0.1f);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		down(1);
		// 键盘加速定时器 延迟 0.1s 执行
		this->schedule(schedule_selector(GameScene::down), 0.01f, 12, 0.2f);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)  //左
	{
		if (highest() < 13 && change_position(-1))
		{
			moveBlocks(-1);
		}
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)  //右
	{
		if (highest() < 13 && change_position(1))
		{
			moveBlocks(1);
		}
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW)  //上
	{
		down(1);
		// 在这里改变形态
		changeShape();
	}
	else if(keycode == EventKeyboard::KeyCode::KEY_Q) //声音开启与关闭
	{ 
		SoundOnOff_Key();
	}
}

//Q键开启关闭声音
void GameScene::SoundOnOff_Key() {
	if (isSoundOpen)
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		isSoundOpen = false;
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		isSoundOpen = true;
	}
}

//放开↑键
void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)  //下
	{
		down(1);
		// 键盘加速定时器 延迟 0.5s 执行
		this->unschedule(schedule_selector(GameScene::down));
	}
}

//改变形状
void GameScene::changeShape()
{
	if (highest() < 14) {
		int ctime[7] = { 0,1,1,1,3,3,3 }; // 对应着可以变换的次数
		int flag = 1;//1为右转，-1为左转
		int bx[4], by[4], fx[4], fy[4];
		Sprite* boxes[4];
		for (int i = 0; i<4; i++) {
			boxes[i] = (Sprite *)this->getChildByTag(110 + i);
			bx[i] = getIx(boxes[i]->getPositionX());
			by[i] = getIy(boxes[i]->getPositionY());
		}
		// 得到了方块的四个坐标
		if (ctime[shape] == 0) {
			// 不用变换
		}
		else if (ctime[shape] == 1) {
			// 两次变换
			for (int i = 0; i<4; i++) {
				fx[i] = bx[0] - (by[0] - by[i])*flag;
				fy[i] = by[0] + (bx[0] - bx[i])*flag;
			}
			flag = -flag;
			// 旋转
			if (in_out_x(fx) && in_out_y(fy)) {
				for (int i = 0; i<4; i++) {
					boxes[i]->setPositionX(getX(fx[i]));
					boxes[i]->setPositionY(getY(fy[i]));
				}
			}
			else {
				flag = -flag; // 取反
			}
		}
		else if (ctime[shape] == 3) {
			// 四次变换 单向旋转
			for (int i = 0; i<4; i++) {
				fx[i] = bx[0] - (by[0] - by[i]);
				fy[i] = by[0] + (bx[0] - bx[i]);
			}
			// 得到四个坐标,判断四个坐标是否可以存在
			if (in_out_x(fx) && in_out_y(fy)) {
				for (int i = 0; i<4; i++) {
					boxes[i]->setPositionX(getX(fx[i]));
					boxes[i]->setPositionY(getY(fy[i]));
				}
			}
		}
	}
}

//可以旋转，返回1，不可以则返回0，主要检测左右是否超出游戏范围
bool GameScene::in_out_x(int x[])
{
	int check = 1;
	for (int i = 0; i < 4; i++)
	{
		if (x[i] < 0 || x[i]>9)
		{
			check = 0;
		}
	}
	return check;
}

//可以旋转，返回1，不可以则返回0，主要检测是否会触碰到最底层
bool GameScene::in_out_y(int y[])
{
	int check = 1;
	for (int i = 0; i < 4; i++)
	{
		if (y < 0)
		{
			check = 0;
		}
	}
	return check;
}

//判断是否可以左右移动
bool GameScene::change_position(int flag)
{
	bool square[10][13];
	for (int i = 0; i < boxList.size(); i++)//转化为棋盘
	{
		cocos2d::Sprite*box = boxList.at(i);
		square[getIx(box->getPositionX())][getIy(box->getPositionY())] = true;
	}

	Sprite* boxes[4];
	int bx[4], by[4];
	for (int i = 0; i<4; i++) {
		boxes[i] = (Sprite *)this->getChildByTag(110 + i);
		bx[i] = getIx(boxes[i]->getPositionX()) + flag;//假设如果可以移动之后的坐标
		by[i] = getIy(boxes[i]->getPositionY());
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (square[i][j] == true)
			{
				for (int bi = 0; bi < 4; bi++)
				{
					if (bx[bi] == i&&by[bi] == j)//若有方块与之重合，那么不成立，不能变化
						return false;
				}
			}
		}
	}
	return true;
}

void GameScene::moveBlocks(float direction)//left(-1)；right(1)；
{
	int beforeX[4], afterX[4];
	cocos2d::Sprite*box[4];
	for (int i = 0; i < 4; i++)
	{
		box[i] = (cocos2d::Sprite *)this->getChildByTag(110 + i);
		beforeX[i] = getIx(box[i]->getPositionX());
		afterX[i] = beforeX[i] + direction;
	}
	if (in_out_x(afterX))
	{
		for (int i = 0; i < 4; i++)
		{
			box[i]->setPositionX(getX(afterX[i]));
		}
	}
}

void GameScene::downMove(float y)
{
	if (highest() < 13)
	{
		cocos2d::Sprite*box[4];
		for (int i = 0; i < 4; i++)
		{
			box[i] = (cocos2d::Sprite *)this->getChildByTag(110 + i); // 节点类型
		}

		//不能下移
		if (!in_out_s(110))
		{
			for (int i = 0; i < 4; i++)
			{
				box[i]->setTag(40);
				this->boxList.pushBack(box[i]);
			}
			//判断是否消除，并创建新的方块
			if (highest() < 13)
			{
				checkBox();
				createBlocks();
			}
			else
			{
				GameOver();
			}
			this->unschedule(schedule_selector(GameScene::down));
		}
		else//可以下移
		{
			for (int i = 0; i < 4; i++)
			{
				box[i]->setPositionY(getY(getIy(box[i]->getPositionY()) - 1));
			}
		}
	}
}

//实现downMove(1)
void GameScene::down(float y)
{
	downMove(1);
}

//检测是否可以下移。0表示不可以，1表示可以
bool GameScene::in_out_s(int tag)
{
	cocos2d::Sprite*box;
	for (int i = 0; i < 4; i++)
	{
		box = (cocos2d::Sprite*)this->getChildByTag(tag + i);
		int afterX = getIx(box->getPositionX());
		int afterY = getIy(box->getPositionY());

		if (afterY <= 0)
			return 0;

		for (int j = 0; j < boxList.size(); j++)
		{
			cocos2d::Sprite*box_vec = boxList.at(j);
			int beforeX = getIx(box_vec->getPositionX());
			int beforeY = getIy(box_vec->getPositionY());

			if (beforeX - afterX == 0 && beforeY - afterY == -1)//4个方块的下端是否有方块
			{
				return 0;
			}
		}
	}
	return 1;
}

void GameScene::GameOver()
{
	// 停止背景音乐
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto overlabel = Label::createWithTTF("GAME OVER", "fonts/Marker Felt.ttf", 90);
	overlabel->setColor(Color3B(245, 146, 71));
	// position the label on the center of the screen
	overlabel->setPosition(Vec2(240, 500));
	// add the label as a child to this layer
	this->addChild(overlabel, 1);
	std::string printfscore = StringUtils::format("score:%d", score);
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
	// 重新开始
	auto restartItemImage = MenuItemImage::create("restart.png", "restart.png",
		CC_CALLBACK_1(GameScene::menuRestartCallback, this));
	auto restartMenu = Menu::create(restartItemImage, NULL);
	restartMenu->setAnchorPoint(Vec2(0.5, 0.5));
	restartMenu->setPosition(Vec2(240, 300));
	this->addChild(restartMenu, 1);
	// 返回
	auto returnItemImage = MenuItemImage::create("returnhome.png", "returnhome.png",
		CC_CALLBACK_1(GameScene::menuReturnCallback, this));
	auto returnMenu = Menu::create(returnItemImage, NULL);
	returnMenu->setAnchorPoint(Vec2(0.5, 0.5));
	returnMenu->setPosition(Vec2(240, 200));
	this->addChild(returnMenu, 1);
}

void GameScene::menuRestartCallback(Ref *pSender)
{
	Director::getInstance()->replaceScene(GameLay::createScene());
}

void GameScene::menuReturnCallback(Ref *pSender)
{
	Director::getInstance()->replaceScene(StartScene::createScene());
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);
}
