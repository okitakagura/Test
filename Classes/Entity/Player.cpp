#include "Player.h"
#include "PlayerDivision.h"
#include "Header/macro.h"
Player::Player()
{
	_divisionNum = 0;     //��ҷ�������Ϊ��
}

Player::~Player()
{
	_divisionList.clear(); //��ҷ����б����
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
	_playerName = "Hello";//��Ҫ�޸ģ�������Excel������������

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
	//����һ����ҷ���
	auto division = PlayerDivision::create(_playerName, _vestmentID, _keywordID, score);
	division->setPosition(position);  //ʸ��λ��
	division->setPlayerName(_playerName); //����
	division->setVelocity(velocity);//ʸ���ٶ�

	_divisionList.pushBack(division);//���ӽ���ҷ����б�
	_divisionNum++;//���ӷ�������

	return division;
}
void Player::setVelocity(Vec2 v)
{
	//�����ٶ�ʸ��
	_state = State::NORMAL;

	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			division->setVelocity(v);//����ÿһ��������ٶ���ͬ
		}
	}
	_velocity = v;
}
Vec2 Player::getVelocity()
{   //����ʸ���ٶ�
	return _velocity;
}
void Player::dividePlayer()
{
	bool divideFlag = false;
	int size = _divisionList.size();
	for (int i = 0; i<size; i++)
	{
		auto division = _divisionList.at(i);//��at�����Ƿ�Խ��
		int score = division->getScore();
		if (score >= PLAYER_MIN_DIVIDE_SCORE)//�ﵽ���ѵ�����
		{  
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION_NUM)
			{   //�Ѵﵽ���ѵ������
				break;
			}

			this->unscheduleAllCallbacks();//�Ƴ�����schedule�Ļص�����  
			_state = State::DIVIDE;
			_combineEnable = false;

			divideFlag = true;

			division->divide();//�������룬�뾶���ٶȸı�
			float radius = division->getRadius();
			int score = division->getScore();
			Vec2 position = division->getPosition();
			auto newDivision = this->createDivision(position, _velocity, score);
			_map->addChild(newDivision);

			float angle = _velocity.getAngle();//���ٶ�ʸ���ļн�
			float divideDistance = radius + PLAYER_MIN_DIVIDE_DISTANCE;//�����ľ���
			Vec2 newPosition = Vec2(divideDistance*cosf(angle), divideDistance*sinf(angle));//�����µ�λ��ʸ��

			auto sequence = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newPosition), 1.8f),//����һ���ɿ�������ƶ�����
				CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),//����һ��������������һ���ص�����
				NULL);
			newDivision->runAction(sequence);
		}
	}

	if (divideFlag)
	{
		this->scheduleOnce(schedule_selector(Player::setCombine), 15);//15s֮��Ϊ������Ժ����״̬
	}
}
Rect Player::getPlayerRect()
{   //��ȡ��Ұ�Χ�У�Rect��һ��������
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
	}// �ҳ����Χ

	Rect rect;
	rect.size = Size(maxX - minX, maxY - minY);//�ߴ��С
	rect.origin = Vec2((minX + maxX) / 2, (minY + maxY) / 2);//��ʼ���꣬����
	return rect;
}
bool Player::collideBean(Bean * bean)
{   //�붹����ײ
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			if (division->collideBean(bean))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));//���ݷ���ȷ��Z���ϵ�����ceil������ȡ����
				return true;
			}
		}
	}
	return false;
}
void Player::updateDivision()//�����Ϣ����
{
	if (_state == State::CONCENTRATE)
	{   //����
		this->concentrate();
	}

	auto rect = this->getPlayerRect();

	//�����ٶȸı�ÿ�������λ��
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			float speed = division->getSpeed();
			if (_state == State::CONCENTRATE)
			{
				speed = PLAYER_CONCENTRATE_SPEED;//����к��ٶ�
			}
			Vec2 velocity = division->getVelocity();
			float dx = velocity.x*speed;
			float dy = velocity.y*speed;
			Vec2 divisionVec = Vec2(dx, dy); //�����ƶ�����
			Vec2 oldPosition = division->getPosition();//ǰһλ��
			Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);//��һλ��
			if (_state != State::CONCENTRATE && _state != State::DIVIDE) //������������״̬��Ҳ���ڷ���״̬������������Ŀ�£
			{
				Vec2 centerVec = rect.origin - oldPosition; // ָ�����ķ���
				centerVec.normalize();//��׼��ʹ֮ģ��Ϊ1
				centerVec.x = centerVec.x*PLAYER_CONCENTRATE_SPEED*0.2;
				centerVec.y = centerVec.y*PLAYER_CONCENTRATE_SPEED*0.2;
				Vec2 moveVec = Vec2(centerVec.x + divisionVec.x, centerVec.y + divisionVec.y);//��ָ�����ĵķ���ʸ���������ƶ�����ʸ�����
				float angle = Vec2::angle(centerVec, divisionVec);
				float cosAngle = cosf(angle);

				if (cosAngle > 0 && cosAngle < 1) //ֻ����н�С��90�ȵ����
					newPosition = Vec2(oldPosition.x + moveVec.x, oldPosition.y + moveVec.y);
			}

			float radius1 = division->getRadius();
			if (!_combineEnable)//���ں��塢����״̬������������ײ���
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL&&division2 != division)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float oldDistance = position2.distance(oldPosition);
						if (oldDistance <= radius1 + radius2)//�������ʱ���ܲ�����ײ�����
						{
							//	������Բ��Բ�������߷ֱ��������ƶ�
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
			//����Ƿ񳬳��߽�
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
			division->setPrePosition(oldPosition);//ǰһ֡λ��
			division->setPosition(newPosition);//����λ��
		}
	}
	if (!_combineEnable)//����������ײ��⣬�ƶ������������ײ��ȡ���ƶ�
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
	if (_combineEnable)  //����������
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
						if (distance<abs(radius2 - radius1))     //����С�ڰ뾶֮�������������Ժ���
						{
							_divisionNum--;//���ٷ�������

							_combineEnable = false;//�رպ���״̬
							this->scheduleOnce(schedule_selector(Player::setCombine), 8);//�ֿ���һ�κ���״̬

							if (radius1>radius2)
							{   //1����2��1���2�����
								division1->eatRival(division2->getScore());
								_divisionList.eraseObject(division2);
								division2->removeFromParentAndCleanup(true);
								return;
							}
							else
							{   //2����1��2���1�����
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
{   //��������״̬
	_combineEnable = true;
}

void Player::divideFinish()
{   //����״̬����
	_state = State::NORMAL;
}

void Player::concentrate() 
{     //���������Ŀ�£
	auto rect = this->getPlayerRect();//��Ұ�Χ��Χ
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			Vec2 divisionPosition = division->getPosition();
			Vec2 velocity = rect.origin - divisionPosition;//����ָ��Բ��
			velocity.normalize();//��׼������ʹ����ģΪ1
			division->setVelocity(velocity);//�ٶȷ���ָ��Բ��
		}
	}
}
void Player::setConcentrate()//���ÿɺ���״̬
{
	_state = State::CONCENTRATE;
}
bool Player::collideRival(Player * rival)//����˵���ײ
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // ��ҷ�����
		{
			_divisionList.eraseObject(division1);//�Ƴ�һ������
			division1->removeFromParentAndCleanup(true);//�Ӹ��ڵ��Ƴ����ӽڵ�
			_divisionNum--;
			i--;
			bRet = true;
		}
		else if (result == 1) // ��ҷ�����˵���
		{
			bRet = true;
		}
	}

	return bRet;
}

int Player::collideDivision(PlayerDivision * division)//��������ҷ���
{    //����ַ������ײ
	int flag = 0;
	Vec2 playerPosition = division->getPosition();//���λ��
	float playerRadius = division->getRadius();//��Ұ뾶

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		Vec2 rivalPosition = division2->getPosition();//����λ��
		float rivalRadius = division2->getRadius();//���˰뾶
		float distance = playerPosition.distance(rivalPosition);
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*MIN_EAT_MULTIPLE)  //��ҷ�����˶���
			{
				division->eatRival(rivalScore);
				return 1;
			}
			else if (rivalScore > playerScore*MIN_EAT_MULTIPLE)  //��ҷ�����
			{
				division2->eatRival(playerScore);
				return 2;
			}
		}
	}
	return flag;
}

float Player::getTotalWeight()//������
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

std::string  Player::getPlayerName()//��ȡ�����
{
	return _playerName;
}
void Player::resetPlayer()//��Ҹ�����λ�ã�
{
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	Vec2 position = Vec2(xPosition, yPosition);

	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	_velocity = Vec2::ZERO;
}
int Player::getTotalScore()//��ȡ�ܵ÷�
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

int Player::getDivisionNum()//��ȡ��������
{
	return _divisionNum;
}
Vector<PlayerDivision *> & Player::getDivisionList()//��ȡ�����б�
{
	return _divisionList;
}

void Player::onExit()//����
{
	Node::onExit();
}

void Player::clearDivisionList()//�����ҷ����б�
{
	for (auto division : _divisionList)
	{
		division->removeFromParentAndCleanup(true);//�Ӹ��ڵ����Ƴ��ӽڵ�
	}
	_divisionList.clear();
	_divisionNum = 0;
}
