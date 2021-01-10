#ifndef __TIME_H__
#define __TIME_H__
#include "T_Timer.h"

typedef unsigned int uint;

namespace Time
{
	namespace Real
	{

			static T_Timer real_time;

			static bool pause;

			void	InitRealTime();
			float	GetDt();
			void	UpdateFrameData();
		
	}
}

#endif // !__TIME_H__