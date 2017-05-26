#include "Player.h"
#include "PlayerDivision.h"
#include "Header/macro.h"
Player::Player()
{
	_divisionNum = 0;     //玩家分身数量为零
}

Player::~Player()
{
	_divisionList.clear(); //玩家分身列表清空
}
Player * Player::create(Vec2 position, Node * map)
{
	Player * player = new Player();
	if (player && player->init(position, map))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(Vec2 position, Node * map)
{
	if (!Node::init())
	{
		return false;
	}
	int nameIndex = rand() % 10 + 1;
	_playerName = "Hello";//需要修改，可以在Excel表格里随机名字

	_map = map;
	_vestmentID = 2;
	_keywordID = 1;
	_state = State::NORMAL;
	_combineEnable = true;

	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	return true;
}

Player * Player::create(std::string & name, int vestmentID, int keywordID, Vec2 velocity, Node * map)
{
	Player * player = new Player();
	if (player && player->init(name, vestmentID, keywordID, velocity, map))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(std::string & name, int vestmentID, int keywordID, Vec2 velocity, Node * map)
{
	if (!Node::init())
	{
		return false;
	}

	_playerName = name;
	_map = map;
	_vestmentID = vestmentID;
	_keywordID = keywordID;
	_velocity = velocity;
	_state = State::NORMAL;
	_combineEnable = true;

	return true;
}
PlayerDivision * Player::createDivision(Vec2 position, Vec2 velocity, int score)
{
	//创建一个玩家分身
	auto division = PlayerDivision::create(_playerName, _vestmentID, _keywordID, score);
	division->setPosition(position);  //矢量位置
	division->setPlayerName(_playerName); //名字
	division->setVelocity(velocity);//矢量速度

	_divisionList.pushBack(division);//增加进玩家分身列表
	_divisionNum++;//增加分身数量

	return division;
}
void Player::setVelocity(Vec2 v)
{
	//设置速度矢量
	_state = State::NORMAL;

	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			division->setVelocity(v);//设置每一个分身的速度相同
		}
	}
	_velocity = v;
}
Vec2 Player::getVelocity()
{   //返回矢量速度
	return _velocity;
}
void Player::dividePlayer()
{
	bool divideFlag = false;
	int size = _divisionList.size();
	for (int i = 0; i<size; i++)
	{
		auto division = _divisionList.at(i);//用at会检查是否越界
		int score = division->getScore();
		if (score >= PLAYER_MIN_DIVIDE_SCORE)//达到分裂的条件
		{  
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION_NUM)
			{   //已达到分裂的最多数
				break;
			}

			this->unscheduleAllCallbacks();//移除所有schedule的回调函数  
			_state = State::DIVIDE;
			_combineEnable = false;

			divideFlag = true;

			division->divide();//分数减半，半径和速度改变
			float radius = division->getRadius();
			int score = division->getScore();
			Vec2 position = division->getPosition();
			auto newDivision = this->createDivision(position, _velocity, score);
			_map->addChild(newDivision);

			float angle = _velocity.getAngle();//求速度矢量的夹角
			float divideDistance = radius + PLAYER_MIN_DIVIDE_DISTANCE;//分身间的距离
			Vec2 newPosition = Vec2(divideDistance*cosf(angle), divideDistance*sinf(angle));//设置新的位置矢量

			auto sequence = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newPosition), 1.8f),//创建一个由快变慢的移动动作
				CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),//适配一个函数，并返回一个回调函数
				NULL);
			newDivision->runAction(sequence);
		}
	}

	if (divideFlag)
	{
		this->scheduleOnce(schedule_selector(Player::setCombine), 15);//15s之后为分身可以合体的状态
	}
}
Rect Player::getPlayerRect()
{   //获取玩家包围盒，Rect是一个矩形类
	float minX, minY, maxX, maxY;

	auto division = _divisionList.at(0);
	if (division != NULL)
	{
		Vec2 divisionPosition = division->getPosition();
		float radius = division->getRadius();
		minX = divisionPosition.x - radius;
		maxX = divisionPosition.x + radius;
		minY = divisionPosition.y - radius;
		maxY = divisionPosition.y + radius;
	}

	for (auto item : _divisionList)
	{

		if (item != NULL)
		{
			Vec2 divisionPosition = item->getPosition();
			float radius = item->getRadius();
			if (minX > divisionPosition.x - radius) { minX = divisionPosition.x - radius; }
			if (maxX <divisionPosition.x + radius) { maxX = divisionPosition.x + radius; }
			if (minY >divisionPosition.y - radius) { minY = divisionPosition.y - radius; }
			if (maxY < divisionPosition.y + radius) { maxY = divisionPosition.y + radius; }
		}
	}// 找出最大范围

	Rect rect;
	rect.size = Size(maxX - minX, maxY - minY);//尺寸大小
	rect.origin = Vec2((minX + maxX) / 2, (minY + maxY) / 2);//起始坐标，中心
	return rect;
}
bool Player::collideBean(Bean * bean)
{   //与豆子碰撞
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			if (division->collideBean(bean))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));//根据分数确定Z轴上的排序，ceil是向上取整数
				return true;
			}
		}
	}
	return false;
}
void Player::updateDivision()//玩家信息更新
{
	if (_state == State::CONCENTRATE)
	{   //合体
		this->concentrate();
	}

	auto rect = this->getPlayerRect();

	//根据速度改变每个分身的位置
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			float speed = division->getSpeed();
			if (_state == State::CONCENTRATE)
			{
				speed = PLAYER_CONCENTRATE_SPEED;//玩家中和速度
			}
			Vec2 velocity = division->getVelocity();
			float dx = velocity.x*speed;
			float dy = velocity.y*speed;
			Vec2 divisionVec = Vec2(dx, dy); //分身移动方向
			Vec2 oldPosition = division->getPosition();//前一位置
			Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);//下一位置
			if (_state != State::CONCENTRATE && _state != State::DIVIDE) //不在主动合体状态，也不在分身状态，分身会往中心靠拢
			{
				Vec2 centerVec = rect.origin - oldPosition; // 指向中心方向
				centerVec.normalize();//标准化使之模长为1
				centerVec.x = centerVec.x*PLAYER_CONCENTRATE_SPEED*0.2;
				centerVec.y = centerVec.y*PLAYER_CONCENTRATE_SPEED*0.2;
				Vec2 moveVec = Vec2(centerVec.x + divisionVec.x, centerVec.y + divisionVec.y);//将指向中心的方向矢量与分身的移动方向矢量相加
				float angle = Vec2::angle(centerVec, divisionVec);
				float cosAngle = cosf(angle);

				if (cosAngle > 0 && cosAngle < 1) //只处理夹角小于90度的情况
					newPosition = Vec2(oldPosition.x + moveVec.x, oldPosition.y + moveVec.y);
			}

			float radius1 = division->getRadius();
			if (!_combineEnable)//不在合体、分身状态，分身会进行碰撞检测
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL&&division2 != division)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float oldDistance = position2.distance(oldPosition);
						if (oldDistance <= radius1 + radius2)//处理分身时可能产生碰撞的情况
						{
							//	将两个圆以圆心连接线分别向两边移动
							float distance = ((radius1 + radius2) - oldDistance) / 2;
							Vec2 vec = oldPosition - position2;
							float angle = vec.getAngle();
							newPosition = Vec2(oldPosition.x + cosf(angle), oldPosition.y + sinf(angle));
							division2->setPosition(Vec2(position2.x - cosf(angle), position2.y - sinf(angle)));
							break;
						}
					}
				}
			}
			//检查是否超出边界
			if (newPosition.x <= radius1)
			{
				newPosition.x = radius1;
			}
			if (newPosition.x >= MAP_WIDTH - radius1)
			{
				newPosition.x = MAP_WIDTH - radius1;
			}
			if (newPosition.y <= radius1)
			{
				newPosition.y = radius1;
			}
			if (newPosition.y >= MAP_HEIGHT - radius1)
			{
				newPosition.y = MAP_HEIGHT - radius1;
			}
			division->setPrePosition(oldPosition);//前一帧位置
			division->setPosition(newPosition);//现在位置
		}
	}
	if (!_combineEnable)//分身会进行碰撞检测，移动后如果产生碰撞则取消移动
	{
		for (auto division1 : _divisionList)
		{
			if (division1 != NULL)
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL&&division2 != division1)
					{
						Vec2 position1 = division1->getPosition();
						Vec2 position2 = division2->getPosition();
						float radius1 = division1->getRadius();
						float radius2 = division2->getRadius();
						float distance = position1.distance(position2);
						if (distance < radius1 + radius2)
						{
							Vec2 oldPosition1 = division1->getPrePosition();
							Vec2 oldPosition2 = division2->getPrePosition();
							float oldDistance = oldPosition1.distance(oldPosition2);
							float fixDistance = 0;
							if (oldDistance <= radius1 + radius2)
							{
								fixDistance = 2;
							}
							else
							{
								fixDistance = (radius1 + radius2 - distance);
							}

							Vec2 v = position2 - position1;
							float angle = v.getAngle();
							Vec2 newPosition1 = Vec2(position1.x - cosf(angle)*fixDistance, position1.y - sinf(angle)*fixDistance);
							Vec2 newPosition2 = Vec2(position2.x + cosf(angle)*fixDistance, position2.y + sinf(angle)*fixDistance);
							division1->setPosition(newPosition1);
							division2->setPosition(newPosition2);
							break;
						}
					}
				}
			}
		}
	}
	if (_combineEnable)  //处理分身合体
	{
		for (int i = 0; i < _divisionList.size(); i++)
		{
			auto division1 = _divisionList.at(i);
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (int j = i + 1; j < _divisionList.size(); j++)
				{
					auto division2 = _divisionList.at(j);
					if (division2 != NULL)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float distance = position2.distance(position1);
						if (distance<abs(radius2 - radius1))     //距离小于半径之差，则两个分身可以合体
						{
							_divisionNum--;//减少分身数量

							_combineEnable = false;//关闭合体状态
							this->scheduleOnce(schedule_selector(Player::setCombine), 8);//又开启一次合体状态

							if (radius1>radius2)
							{   //1吃了2，1变大，2被清除
								division1->eatRival(division2->getScore());
								_divisionList.eraseObject(division2);
								division2->removeFromParentAndCleanup(true);
								return;
							}
							else
							{   //2吃了1，2变大，1被清除
								division2->eatRival(division1->getScore());
								_divisionList.eraseObject(division1);
								division1->removeFromParentAndCleanup(true);
								return;
							}
						}
					}

				}
			}
		}
	}
}

void Player::setCombine(float dt)
{   //开启合体状态
	_combineEnable = true;
}

void Player::divideFinish()
{   //分裂状态结束
	_state = State::NORMAL;
}

void Player::concentrate() 
{     //主动向中心靠拢
	auto rect = this->getPlayerRect();//玩家包围范围
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			Vec2 divisionPosition = division->getPosition();
			Vec2 velocity = rect.origin - divisionPosition;//方向指向圆心
			velocity.normalize();//标准化向量使它的模为1
			division->setVelocity(velocity);//速度方向指向圆心
		}
	}
}
void Player::setConcentrate()//设置可合体状态
{
	_state = State::CONCENTRATE;
}
bool Player::collideRival(Player * rival)//与敌人的碰撞
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // 玩家分身被吃
		{
			_divisionList.eraseObject(division1);//移除一个分身
			division1->removeFromParentAndCleanup(true);//从父节点移除此子节点
			_divisionNum--;
			i--;
			bRet = true;
		}
		else if (result == 1) // 玩家分身吃了敌人
		{
			bRet = true;
		}
	}

	return bRet;
}

int Player::collideDivision(PlayerDivision * division)//参数是玩家分身
{    //与对手分身的碰撞
	int flag = 0;
	Vec2 playerPosition = division->getPosition();//玩家位置
	float playerRadius = division->getRadius();//玩家半径

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		Vec2 rivalPosition = division2->getPosition();//敌人位置
		float rivalRadius = division2->getRadius();//敌人半径
		float distance = playerPosition.distance(rivalPosition);
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*MIN_EAT_MULTIPLE)  //玩家分身吃了对手
			{
				division->eatRival(rivalScore);
				return 1;
			}
			else if (rivalScore > playerScore*MIN_EAT_MULTIPLE)  //玩家分身被吃
			{
				division2->eatRival(playerScore);
				return 2;
			}
		}
	}
	return flag;
}

float Player::getTotalWeight()//总重量
{
	float weight = 0;
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			weight += division->getWeight();
		}
	}
	return weight;
}

std::string  Player::getPlayerName()//获取玩家名
{
	return _playerName;
}
void Player::resetPlayer()//玩家复活（随机位置）
{
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	Vec2 position = Vec2(xPosition, yPosition);

	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	_velocity = Vec2::ZERO;
}
int Player::getTotalScore()//获取总得分
{
	int score = 0;

	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			score += division->getScore();
		}
	}
	return score;
}

int Player::getDivisionNum()//获取分身数量
{
	return _divisionNum;
}
Vector<PlayerDivision *> & Player::getDivisionList()//获取分身列表
{
	return _divisionList;
}

void Player::onExit()//结束
{
	Node::onExit();
}

void Player::clearDivisionList()//清空玩家分身列表
{
	for (auto division : _divisionList)
	{
		division->removeFromParentAndCleanup(true);//从父节点中移除子节点
	}
	_divisionList.clear();
	_divisionNum = 0;
}
