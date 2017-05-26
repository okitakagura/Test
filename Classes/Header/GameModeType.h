#pragma once
#ifndef _GAMEMODETYPE_H_
#define _GAMEMODETYPE_H_

namespace GameMode
{
	enum eMode
	{
		eMode_SINGLE = 1,		
		eMode_TEAM = 4,			
		eMode_SURVIVAL = 8,		
		eMode_CUSTOM = 16		
	};
}
#endif