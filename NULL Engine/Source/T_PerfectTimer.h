#ifndef __T_PERFECT_TIMER_H__
#define __T_PERFECT_TIMER_H__

typedef unsigned __int64 uint64;

class T_PerfectTimer
{
public:
	T_PerfectTimer();							// Constructor

	void	Start		();
	void	Stop		();

	double	ReadMs		() const;
	uint64	ReadTicks	() const;

private:
	bool			running;				// Will keep track of whether or not the timer is still active.
	uint64			started_at;				// Will keep track of the exact moment at which the timer started running.
	uint64			stopped_at;				// Will keep track of the exact moment at which the timer stopped running.
	static uint64	frequency;				// As the frequency will be the same across all timers, it will be declared as static.
};

#endif // !__T_PERFECT_TIMER_H__