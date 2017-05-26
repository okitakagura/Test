#pragma once
#ifndef __MESSAGETYPE_H__
#define __MESSAGETYPE_H__

namespace MessageType
{
	enum eMsgType
	{
		eMsg_LOGIN = 1,					
		eMsg_LOGIN_RESULT,				
		eMsg_REGISTER,					
		eMsg_REGISTER_RESULT,			
		eMsg_ACCOUNT_INFO,				
		eMsg_ACCOUNT_INFO_RESULT,		
		eMsg_PLAYER_INFO,				
		eMsg_PLAYER_INFO_RESULT,		
		eMsg_START_GAME_SINGLE,			
		eMsg_START_GAME_SINGLE_RESULT,	
		eMsg_START_GAME_TEAM,			
		eMsg_START_GAME_SURVIVAL,		
		eMsg_CHAT_SEND,					
		eMsg_CHAT_RECEIVE,				
		eMsg_MOVE,						
		eMsg_UPDATE_POSITION,			
		eMsg_DIVIDE,									
		eMsg_EAT_SPORE,					
		eMsg_ADD_PRICK,					
		eMsg_EAT_PRICK,					
		eMsg_EAT_BEAN,					
		eMsg_UPDATE_SPORE,			
		eMsg_ENTER_PLAYER,				
		eMsg_PLAYER_CONCENTRATE,		
		eMsg_UPDATE_TIME,			

		eMsg_MAX = 9999
	};
}


#endif