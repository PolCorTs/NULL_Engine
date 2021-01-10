#ifndef __M_Audio__H__
#define __M_Audio__H__

#include "Module.h"

class M_Audio : public Module
{
public:
	M_Audio(bool start_enabled = true);
	virtual ~M_Audio();

private:

	bool InitWwise();
};

#endif __ModuleAudio__H__
