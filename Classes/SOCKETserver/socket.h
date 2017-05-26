#pragma once
#include "cocos2d.h"
#include "network\WebSocket.h"

using namespace cocos2d;
using namespace cocos2d::network;

class ball_websocket :public Layer, WebSocket::Delegate//�ص���������
{
public:
	ball_websocket();
	~ball_websocket();

	virtual void onOpen(WebSocket*ws);
	/*init�ᴥ��WebSocket���ӷ�����������ɹ���WebSocket�ͻ����onOpen��
	���ߵ����ߣ��ͻ��˵���������ͨѶ��·�Ѿ��ɹ������������շ���Ϣ�ˡ�*/
	virtual void onMessage(WebSocket*ws, const WebSocket::Data&data);
	/*network::WebSocket::Data����洢�ͻ��˽��յ������ݣ� 
	isBinary���������ж������Ƕ����ƻ����ı���len˵�����ݳ��ȣ�bytesָ�����ݡ�*/
	virtual void onClose(WebSocket*ws);
	/*�����Ƿ������������Ǳ����ر���WebSocket���ͻ��˽��յ���������
	��Ҫ�ͷ�WebSocket�ڴ棬���������õ�ϰ�ߣ��ÿ�ָ�롣*/
	virtual void onError(WebSocket*ws, const WebSocket::ErrorCode&data);
	/*�ͻ��˷��͵���������������󣬾ͻ��յ�onError��Ϣ��
	��Ϸ��Բ�ͬ�Ĵ����룬������Ӧ�Ĵ���*/
	virtual bool init();
	void sendMsg(const std::string & msg);
	void getGameConfig(int type, std::string & config);
	void setAccountName(const std::string & name);
	std::string & getAccountName();
private:
	WebSocket *_socket;
	std::map<int, std::string>_gameConfig;
	std::string _accountName;
};

