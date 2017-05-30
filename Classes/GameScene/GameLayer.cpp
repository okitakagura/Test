#include "GameLayer.h"
#include "Joystick/joystick.h"
#include "Entity/Entity.h"
#include "Entity/Bean.h"
#include "Entity/Player.h"
#include "Entity/PlayerDivision.h"
#include "../SceneManager.h"

enum GameLayerZOrder//排序
{
	GAME_LAYER_BACKGROUND_Z,
	GAME_LAYER_MAP_Z,
	GAME_LAYER_JOYSTICK_Z
};
GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
	_rivalMap.clear();
	_beanList.clear();
	//注销自定义事件监听器
	_eventDispatcher->removeCustomEventListeners("Divide");
	_eventDispatcher->removeCustomEventListeners("PlayerMove");
	_eventDispatcher->removeCustomEventListeners("PlayerDivide");
	_eventDispatcher->removeCustomEventListeners("EnterPlayer");
	_eventDispatcher->removeCustomEventListeners("PlayerConcentrate");
	_eventDispatcher->removeCustomEventListeners("UpdatePlayer");
}
bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto colorLayer = LayerColor::create(Color4B(105, 105, 105, 255), DESIGN_SCREEN_WIDTH, DESIGN_SCREEN_HEIGHT);
	this->addChild(colorLayer, GAME_LAYER_BACKGROUND_Z);

	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, GAME_LAYER_MAP_Z);

	initData();

	_joystick = joystick::create("base.png", "joystick.png");
	_joystick->setPosition(Vec2::ZERO);
	_joystick->setVisible(false);//隐藏
	this->addChild(_joystick, GAME_LAYER_JOYSTICK_Z);

	auto listener = EventListenerTouchOneByOne::create();//创建一个单点触摸事件监听器对象
	listener->setSwallowTouches(true);//设置吞没事件，在onTouchBegan返回true时吞没事件，事件不会传递给下一个Node对象
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);//设置触摸优先级

	_eventDispatcher->addCustomEventListener("Divide", CC_CALLBACK_0(GameLayer::dividePlayer, this));
	_eventDispatcher->addCustomEventListener("PlayerMove", CC_CALLBACK_1(GameLayer::playerMoveEvent, this));
	_eventDispatcher->addCustomEventListener("PlayerDivide", CC_CALLBACK_1(GameLayer::playerDivideEvent, this));;
	_eventDispatcher->addCustomEventListener("EnterPlayer", CC_CALLBACK_1(GameLayer::enterPlayerEvent, this));
	_eventDispatcher->addCustomEventListener("PlayerConcentrate", CC_CALLBACK_1(GameLayer::playerConcentrateEvent, this));
	_eventDispatcher->addCustomEventListener("UpdatePlayer", CC_CALLBACK_1(GameLayer::updatePlayerEvent, this));

	this->scheduleUpdate();

	return true;
}
bool GameLayer::onTouchBegan(Touch * touch, Event * event)
{   //开启触摸事件
	auto position = touch->getLocation();

	_joystick->setPosition(position);
	_joystick->setVisible(true);//显示摇杆

	_joystick->onTouchBegan(touch, event);

	return true;
}

void GameLayer::onTouchMoved(Touch * touch, Event * event)
{    //移动
	_joystick->onTouchMoved(touch, event);
	_player->setVelocity(_joystick->getVelocity());
}

void GameLayer::onTouchEnded(Touch * touch, Event * event)
{   //停止
	_joystick->onTouchEnded(touch, event);
	_joystick->setVisible(false);
	Vec2 velocity = _joystick->getVelocity();
	if (velocity.x == 0 && velocity.y == 0)
	{    //中和操作
		_player->setConcentrate();
		synPlayerConcentrate();
	}
}

void GameLayer::onTouchCancelled(Touch * touch, Event * event)
{    //撤销触摸事件
	_joystick->onTouchCancelled(touch, event);
}
void GameLayer::update(float dt)
{   //信息更新
	updateBean();
	_player->updateDivision();
	updateRival();     
	updateView();
	collide();

	synPlayerInfo();
	synPlayerMove();
}

void GameLayer::initData()
{   //初始化地图数据
	std::string config;
	ball_websocket::getInstance()->getGameConfig(GameMode::eMode_SINGLE, config);
	if (config != "")
	{
		rapidjson::Document doc;
		doc.Parse<0>(config.c_str());
		if (doc.HasParseError())
		{  //检错
			log("GetParseError %d\n", doc.GetParseError());
			return;
		}
		if (doc.IsObject())
		{
			if (doc.HasMember("RoomID"))
			{
				_roomID = doc["RoomID"].GetInt();
			}
			if (doc.HasMember("RoomSeed"))
			{
				int seed = doc["RoomSeed"].GetInt();
				initBean(seed);
			}
			if (doc.HasMember("RivalList"))
			{
				rapidjson::Value & value = doc["RivalList"];
				initRival(value);
			}

			if (doc.HasMember("Player"))
			{
				rapidjson::Value & value = doc["Player"];
				initPlayer(value);
			}
		}
	}
}
void GameLayer::initRival()
{
	int div[MAP_DIVISION_X * MAP_DIVISIOIN_Y] = { 0 };
	int mapDivision = MAP_DIVISION_X * MAP_DIVISIOIN_Y;
	int max_enemy = ceil(double(MAX_RIVAL_NUM) / mapDivision);//返回大于等于参数的最小整数
	for (int i = 0; i < MAX_RIVAL_NUM; i++)
	{   //随机分布数量在地图的不同块上
		int index = rand() % mapDivision;
		if (div[index] + 1 <= max_enemy)
		{
			div[index]++;
		}
		else
		{
			while (true)
			{
				if (++index >= mapDivision)
				{
					index = 0;
				}
				if (div[index] + 1 <= max_enemy)
				{
					div[index]++;
					break;
				}
			}
		}
	}

	for (int i = 0; i < mapDivision; i++)
	{   
		int divisionX = i%MAP_DIVISION_X;
		int divisionY = i / MAP_DIVISION_X;

		for (int j = 0; j < div[i]; j++)
		{
			float positionX = rand() % DESIGN_SCREEN_WIDTH + divisionX*DESIGN_SCREEN_WIDTH;
			float positionY = rand() % DESIGN_SCREEN_HEIGHT + divisionY*DESIGN_SCREEN_HEIGHT;
			auto rival = Player::create(Vec2(positionX, positionY), _map);
			rival->setLocalZOrder(rival->getTotalScore());
			_map->addChild(rival);
			_rivalMap.insert(StringUtils::format("%d", i * 100 + j), rival);

		}
	}
}
void GameLayer::initRival(rapidjson::Value &value)
{

	int size = value.Size();
	for (int i = 0; i < size; i++)
	{
		std::string accountName = value[i]["AccountName"].GetString();
		std::string gameName = value[i]["GameName"].GetString();
		int vestmentID = value[i]["VestmentID"].GetInt();
		int keywordID = value[i]["KeywordID"].GetInt();
		double vx = value[i]["vx"].GetDouble();
		double vy = value[i]["vy"].GetDouble();
		auto player = Player::create(gameName, vestmentID, keywordID, Vec2(vx, vy), _map);
		_map->addChild(player);
		_rivalMap.insert(accountName, player);

		int divisionNum = value[i]["DivisionList"].Size();
		for (int j = 0; j < divisionNum; j++)
		{
			double x = value[i]["DivisionList"][j][0].GetDouble();
			double y = value[i]["DivisionList"][j][1].GetDouble();
			int score = value[i]["DivisionList"][j][2].GetInt();
			double vx = value[i]["DivisionList"][j][3].GetDouble();
			double vy = value[i]["DivisionList"][j][4].GetDouble();
			auto division = player->createDivision(Vec2(x, y), Vec2(vx, vy), score);
			_map->addChild(division, score);
		}
	}
}
void GameLayer::initPlayer()//初始玩家
{
	float xPosition = rand() % MAP_WIDTH;
	float yPosition = rand() % MAP_HEIGHT;
	_player = Player::create(Vec2(xPosition, yPosition), _map);
	_player->setLocalZOrder(_player->getTotalScore());
	_map->addChild(_player);
}
void GameLayer::initPlayer(rapidjson::Value &value)
{
	std::string accountName = value["AccountName"].GetString();
	std::string gameName = value["GameName"].GetString();
	int vestmentID = value["VestmentID"].GetInt();
	int keywordID = value["KeywordID"].GetInt();
	double vx = value["vx"].GetDouble();
	double vy = value["vy"].GetDouble();
	_player = Player::create(gameName, vestmentID, keywordID, Vec2(vx, vy), _map);
	_map->addChild(_player);
	int divisionNum = value["DivisionList"].Size();
	for (int j = 0; j < divisionNum; j++)
	{
		double x = value["DivisionList"][j][0].GetDouble();
		double y = value["DivisionList"][j][1].GetDouble();
		int score = value["DivisionList"][j][2].GetInt();
		double vx = value["DivisionList"][j][3].GetDouble();
		double vy = value["DivisionList"][j][4].GetDouble();
		auto division = _player->createDivision(Vec2(x, y), Vec2(vx, vy), score);
		_map->addChild(division, score);
	}

	_player->updateDivision();
	updateView();
}

//初始化豆子
void GameLayer::initBean()
{
	for (int i = 0; i < MAP_DIVISIOIN_Y; i++)
	{
		for (int j = 0; j < MAP_DIVISION_X; j++)
		{
			for (int m = 0; m < MAP_DIVISION_BEAN_NUM; m++)
			{
				int type = rand() % 4 + 3;//3-6
				int color = rand() % 6 + 1;//1-6
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", type, color);
				auto bean = Bean::create(path.c_str());

				bean->setPosition(Vec2(DESIGN_SCREEN_WIDTH*(j + r1), DESIGN_SCREEN_HEIGHT*(i + r2)));
				bean->setLocalZOrder(bean->getScore());
				_map->addChild(bean);
				_beanList.pushBack(bean);
			}

		}
	}
}

void GameLayer::initBean(int seed)
{
	srand(seed);
	for (int i = 0; i < MAP_DIVISIOIN_Y; i++)
	{
		for (int j = 0; j < MAP_DIVISION_X; j++)
		{
			for (int m = 0; m < MAP_DIVISION_BEAN_NUM; m++)
			{
				int type = rand() % 4 + 3;
				int color = rand() % 6 + 1;
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", type, color);
				auto bean = Bean::create(path.c_str());

				bean->setPosition(Vec2(DESIGN_SCREEN_WIDTH*(j + r1), DESIGN_SCREEN_HEIGHT*(i + r2)));
				bean->setLocalZOrder(bean->getScore());
				_map->addChild(bean);
				_beanList.pushBack(bean);
			}

		}
	}
}

void GameLayer::updateView()//更新游戏视图
{
	auto rect = _player->getPlayerRect();

	float scaleX = DESIGN_SCREEN_WIDTH / (DESIGN_SCREEN_WIDTH + rect.size.width);
	float scaleY = DESIGN_SCREEN_HEIGHT / (DESIGN_SCREEN_HEIGHT + rect.size.height);
	_mapScale = (scaleX < scaleY) ? scaleX : scaleY;

	float dx = rect.origin.x*_mapScale - DESIGN_SCREEN_WIDTH / 2;
	float dy = rect.origin.y*_mapScale - DESIGN_SCREEN_HEIGHT / 2;

	Vec2 newPosition = Vec2(-dx, -dy);

	_map->setScale(_mapScale);
	_map->setPosition(newPosition);
}

void GameLayer::updateBean()//更新豆子
{
	collideBean(_player);

	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			collideBean(rival);
		}
	}
}
//与豆子的碰撞检测
void GameLayer::collideBean(Player * player)
{
	Rect rect = player->getPlayerRect();
	Vec2 position = player->getPosition();

	Vec2 point1 = Vec2(rect.origin.x - rect.size.width / 2, rect.origin.y - rect.size.height / 2);//玩家左下角
	int divisionX1 = floor(point1.x / DESIGN_SCREEN_WIDTH);//向下取整
	int divisionY1 = floor(point1.y / DESIGN_SCREEN_HEIGHT);
	int division1 = divisionY1*MAP_DIVISION_X + divisionX1;
	Vec2 point2 = Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);//玩家右上角
	int divisionX2 = floor(point2.x / DESIGN_SCREEN_WIDTH);
	int divisionY2 = floor(point2.y / DESIGN_SCREEN_HEIGHT);
	int division2 = divisionY2*MAP_DIVISION_X + divisionX2;

	//处理超出界限问题
	if (divisionX1 < 0)
	{
		divisionX1 = 0;
	}
	if (divisionY1 < 0)
	{
		divisionY1 = 0;
	}
	if (divisionX2 == MAP_DIVISION_X)
	{
		divisionX2 = MAP_DIVISION_X - 1;
	}
	if (divisionY2 == MAP_DIVISIOIN_Y)
	{
		divisionY2 = MAP_DIVISIOIN_Y - 1;
	}
	int xSpan = divisionX2 - divisionX1;//x跨度
	int ySpan = divisionY2 - divisionY1;//y跨度
	int max_ball = MAP_DIVISION_BEAN_NUM;

	for (int i = 0; i <= ySpan; i++)
	{
		for (int j = 0; j <= xSpan; j++)
		{
			division1 = (divisionY1 + i)*MAP_DIVISION_X + divisionX1 + j;
			for (int index = division1*max_ball, m = 0; m < max_ball; m++)
			{
				auto bean = _beanList.at(index);
				if (bean->isVisible())
				{
					if (player->collideBean(bean))
					{   //碰撞了豆子，则豆子消失
						bean->setVisible(false);
						float time = rand() % 10 + 10;

						auto sequence = Sequence::create(
							DelayTime::create(time),
							CallFuncN::create(CC_CALLBACK_0(GameLayer::resetBean, this, bean)),
							NULL);
						bean->runAction(sequence);
						//return;
					}
				}
				index++;
			}
		}
	}
}


void GameLayer::updateRival()//更新对手
{
	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			rival->updateDivision();
		}
	}
}

void GameLayer::collide()
{
	for (auto item : _rivalMap)        //检测玩家与其他对手的碰撞
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			if (_player->collideRival(rival))
			{
				float playerDivisionCount = _player->getDivisionNum();
				if (playerDivisionCount == 0)
				{
					_player->resetPlayer();
					break;
				}
			}
		}

	}

}

void GameLayer::resetBean(Node * node)
{   //重置豆子
	node->setVisible(true);
}

void GameLayer::resetPlayer()
{

}

void GameLayer::onExit()
{
	this->unscheduleAllCallbacks();//停止所有计时器
	Layer::onExit();
}

void GameLayer::initDataDefault()
{   //数据默认初始值
	initRival();
	initPlayer();
	initBean();
}

void GameLayer::dividePlayer()
{   //分身操作
	_player->dividePlayer();

	std::string accountName = ball_websocket::getInstance()->getAccountName();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_DIVIDE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	ball_websocket::getInstance()->sendMsg(msg);
}


void GameLayer::synPlayerMove()
{   //同步玩家移动信息
	std::string accountName = ball_websocket::getInstance()->getAccountName();
	Vec2 v = _player->getVelocity();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_MOVE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);
	//doc.AddMember("vx", rapidjson::Value(StringUtils::format("%.2f",v.x).c_str(),allocator), allocator);
	//doc.AddMember("vy", rapidjson::Value(StringUtils::format("%.2f", v.y).c_str(), allocator), allocator);
	doc.AddMember("vx", v.x, allocator);
	doc.AddMember("vy", v.y, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();

	ball_websocket::getInstance()->sendMsg(msg);
}


void GameLayer::synPlayerConcentrate()
{   //同步玩家中合操作
	std::string accountName = ball_websocket::getInstance()->getAccountName();

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();

	doc.AddMember("MsgType", MessageType::eMsg_PLAYER_CONCENTRATE, allocator);
	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	ball_websocket::getInstance()->sendMsg(msg);
}


void GameLayer::synPlayerInfo()
{   //同步玩家信息
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();
	rapidjson::Value divisionArr(rapidjson::kArrayType);

	Vector<PlayerDivision *> divisionList = _player->getDivisionList();
	for (auto division : divisionList)
	{
		rapidjson::Value arr(rapidjson::kArrayType);
		Vec2 position = division->getPosition();
		int score = division->getScore();
		arr.PushBack(position.x, allocator);
		arr.PushBack(position.y, allocator);
		arr.PushBack(score, allocator);
		divisionArr.PushBack(arr, allocator);
	}

	std::string accountName = ball_websocket::getInstance()->getAccountName();
	std::string gameName = _player->getPlayerName();
	Vec2 v = _player->getVelocity();

	doc.AddMember("AccountName", rapidjson::Value(accountName.c_str(), allocator), allocator);
	doc.AddMember("GameName", rapidjson::Value(gameName.c_str(), allocator), allocator);
	doc.AddMember("vx", v.x, allocator);
	doc.AddMember("vy", v.y, allocator);
	doc.AddMember("DivisionList", divisionArr, allocator);
	doc.AddMember("RoomID", _roomID, allocator);
	doc.AddMember("MsgType", MessageType::eMsg_UPDATE_POSITION, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
	doc.Accept(write);

	std::string msg = buffer.GetString();
	//log("sysPlayerInfo msg::%s", msg.c_str());
	ball_websocket::getInstance()->sendMsg(msg);
}


void GameLayer::playerMoveEvent(EventCustom * event)
{  //处理服务器返回的移动消息
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}

	if (doc.IsObject())
	{
		std::string accountName = doc["AccountName"].GetString();
		double vx = doc["vx"].GetDouble();
		double vy = doc["vy"].GetDouble();
		auto rival = _rivalMap.at(accountName);
		if (rival != NULL)
		{
			rival->setVelocity(Vec2(vx, vy));
		}
	}
}

void GameLayer::playerDivideEvent(EventCustom * event)
{   //处理服务器返回的玩家分裂消息
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}

	if (doc.IsObject())
	{
		std::string accountName = doc["AccountName"].GetString();
		auto rival = _rivalMap.at(accountName);
		if (rival != NULL)
		{
			rival->dividePlayer();
		}
	}
}

void GameLayer::enterPlayerEvent(EventCustom * event)
{   //处理服务器返回的玩家进入游戏的消息
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}


	if (doc.IsObject())
	{
		std::string accountName = doc["PlayerInfo"]["AccountName"].GetString();
		log("Player : %s enter room", accountName.c_str());

		std::string gameName = doc["PlayerInfo"]["GameName"].GetString();
		int vestmentID = doc["PlayerInfo"]["VestmentID"].GetInt();
		int keywordID = doc["PlayerInfo"]["KeywordID"].GetInt();
		double vx = doc["PlayerInfo"]["vx"].GetDouble();
		double vy = doc["PlayerInfo"]["vy"].GetDouble();
		auto player = Player::create(gameName, vestmentID, keywordID, Vec2(vx, vy), _map);
		_map->addChild(player);
		_rivalMap.insert(accountName, player);

		int divisionNum = doc["PlayerInfo"]["DivisionList"].Size();
		for (int j = 0; j < divisionNum; j++)
		{
			double x = doc["PlayerInfo"]["DivisionList"][j][0].GetDouble();
			double y = doc["PlayerInfo"]["DivisionList"][j][1].GetDouble();
			int score = doc["PlayerInfo"]["DivisionList"][j][2].GetInt();
			double vx = doc["PlayerInfo"]["DivisionList"][j][3].GetDouble();
			double vy = doc["PlayerInfo"]["DivisionList"][j][4].GetDouble();
			auto division = player->createDivision(Vec2(x, y), Vec2(vx, vy), score);
			_map->addChild(division, score);
		}
	}
}

void GameLayer::playerConcentrateEvent(EventCustom * event)
{    //处理服务器返回的玩家中和的消息
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}

	if (doc.IsObject())
	{
		std::string accountName = doc["AccountName"].GetString();
		auto rival = _rivalMap.at(accountName);
		if (rival != NULL)
		{
			rival->concentrate();
		}
	}
}

void GameLayer::updatePlayerEvent(EventCustom * event)
{  //处理服务器返回的玩家更新的消息
	char * msg = (char *)event->getUserData();
	rapidjson::Document doc;
	doc.Parse<0>(msg);
	if (doc.HasParseError())
	{
		log("GetParseError %d\n", doc.GetParseError());
		return;
	}

	if (doc.IsObject())
	{
		std::string accountName = doc["AccountName"].GetString();
		auto rival = _rivalMap.at(accountName);
		if (rival != NULL)
		{
			int oldSize = rival->getDivisionNum();
			int newSize = doc["DivisionList"].Size();
			double vx = doc["vx"].GetDouble();
			double vy = doc["vy"].GetDouble();
			rival->setVelocity(Vec2(vx, vy));

			if (oldSize != newSize)
			{
				rival->clearDivisionList();
				for (int i = 0; i < newSize; i++)
				{
					double x = doc["DivisionList"][i][0].GetDouble();
					double y = doc["DivisionList"][i][1].GetDouble();
					int score = doc["DivisionList"][i][2].GetInt();
					auto division = rival->createDivision(Vec2(x, y), Vec2(0, 0), score);
					_map->addChild(division, score);
				}

			}
			else
			{
				Vector<PlayerDivision *> divisionList = rival->getDivisionList();
				int i = 0;
				for (auto division : divisionList)
				{
					double x = doc["DivisionList"][i][0].GetDouble();
					double y = doc["DivisionList"][i][1].GetDouble();
					int score = doc["DivisionList"][i][2].GetInt();
					if (division != NULL)
					{
						division->setPosition(Vec2(x, y));
						division->setScore(score);

					}
					i++;
				}
			}

		}
	}
}

