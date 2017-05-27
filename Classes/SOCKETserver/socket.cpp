#pragma execution_character_set("utf-8")
#include "socket.h"
#include "Header/Common.h"

ball_websocket::ball_websocket()
{
	_socket = nullptr;
	_accountName = " ";
}

ball_websocket::~ball_websocket()
{

}

ball_websocket * ball_websocket::getInstance()
{
	if (!_ball_websocket)
	{
		_ball_websocket = new ball_websocket();
		if (_ball_websocket && _ball_websocket->init())
		{
			_ball_websocket->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(_ball_websocket);
		}
	}
	return _ball_websocket;
}

bool ball_websocket::init()
{
	if (!Layer::init())
		return false;

	_socket = new WebSocket();
	if (!_socket->init(*this, "ws://echo.websocket.org"))
	{
		CC_SAFE_DELETE(_socket);//����ڴ�
		return false;
	}
	return true;
}

void ball_websocket::onOpen(WebSocket*ws)
{
	//������ӣ�����Ϣ
	CCLOG("Websocket(%p)opened", ws);
}

void ball_websocket::onMessage(WebSocket*ws, const WebSocket::Data&data)//��������
{
	if (!data.isBinary)//�ж��Ƿ�Ϊ�����ƣ�trueΪ�ַ�����falseΪ������
	{
		log("response text message");

		const std::string&msg = data.bytes;//char* bytes;
		rapidjson::Document doc;
		doc.Parse<0>(std::string(data.bytes).c_str());
		/*����һ�������ݿ⼰�洢�����ݿ���ļ��������ǰ���������ݿ���ļ��и������ݿ�?????*/

		if (doc.HasParseError())//�жϽ�������
		{
			log("GetParseError %d\n", doc.GetParseError);
			return;
		}

		if (doc.IsObject() && doc.HasMember("MsgType"))
			//Checks whether a number can be losslessly converted to a double
			//return Whether a member with that name exists.
		{
			int msgType = doc["MsgType"].GetInt();//getint��������ַ����ֽ����������ÿ�ε��õõ�һ������

			if (msgType == MessageType::eMsg_LOGIN_RESULT)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("LoginResult", (void *)msg.c_str());//����
			}
			else if (msgType == MessageType::eMsg_ACCOUNT_INFO_RESULT)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("AccountInfoResult", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_CHAT_RECEIVE)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ChatMsgReceive", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_MOVE)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("PlayerMove", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_DIVIDE)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("PlayerDivide", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_ADD_PRICK)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("AddPrick", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_ENTER_PLAYER)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EnterPlayer", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_PLAYER_CONCENTRATE)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("PlayerConcentrate", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_UPDATE_POSITION)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("UpdatePlayer", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_UPDATE_TIME)
			{
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("UpdateTime", (void *)msg.c_str());
			}
			else if (msgType == MessageType::eMsg_START_GAME_SINGLE_RESULT)
			{
				_gameConfig[GameMode::eMode_SINGLE] = msg;
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("GameStart", NULL);
			}
		}
	}
}

void ball_websocket::sendMsg(const std::string & msg)
{
	if (!_socket)
	{
		return;
	}
	if (_socket->getReadyState() == WebSocket::State::OPEN)
		//����getReadyState()��ȡ����״̬
	{
		CCLOG("send message:%s", msg.c_str());
		_socket->send(msg);
	}
	else
	{
		CCLOG("Client not yet ready");
	}
}

void ball_websocket::onClose(WebSocket *ws)
{
	CCLOG("websocket instance closed");
	if (ws == _socket)
	{
		_socket = nullptr;
	}
	CC_SAFE_DELETE(ws);
}

void ball_websocket::onError(WebSocket*ws, const WebSocket::ErrorCode&data)
{
	CCLOG("Error:%d", data);
}

void ball_websocket::getGameConfig(int type, std::string & config)
{
	std::map<int, std::string>::iterator it = _gameConfig.find(type);//����������ָ�����ʽƥ���Ԫ��

	if (it != _gameConfig.end())//it first�ǵ�����ָ���ֵ��it second�ǵ�����ָ���Ӧ��ֵ
	{
		config = it->second;
		return;
	}
	config = "";
}

void ball_websocket::setAccountName(const std::string & name)
{
	_accountName = name;
}

std::string &  ball_websocket::getAccountName()
{
	return _accountName;
}