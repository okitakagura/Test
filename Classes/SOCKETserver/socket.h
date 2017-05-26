#pragma once
#include "cocos2d.h"
#include "network\WebSocket.h"

using namespace cocos2d;
using namespace cocos2d::network;

class ball_websocket :public Layer, WebSocket::Delegate//回调监听函数
{
public:
	ball_websocket();
	~ball_websocket();

	virtual void onOpen(WebSocket*ws);
	/*init会触发WebSocket链接服务器，如果成功，WebSocket就会调用onOpen，
	告诉调用者，客户端到服务器的通讯链路已经成功建立，可以收发消息了。*/
	virtual void onMessage(WebSocket*ws, const WebSocket::Data&data);
	/*network::WebSocket::Data对象存储客户端接收到的数据， 
	isBinary属性用来判断数据是二进制还是文本，len说明数据长度，bytes指向数据。*/
	virtual void onClose(WebSocket*ws);
	/*不管是服务器主动还是被动关闭了WebSocket，客户端将收到这个请求后，
	需要释放WebSocket内存，并养成良好的习惯：置空指针。*/
	virtual void onError(WebSocket*ws, const WebSocket::ErrorCode&data);
	/*客户端发送的请求，如果发生错误，就会收到onError消息，
	游戏针对不同的错误码，做出相应的处理。*/
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

