#ifndef __M_Audio_H__
#define __M_Audio_H__

#include <vector>
#include "Module.h"

class M_Audio : public Module
{
public:
	M_Audio(bool start_enabled = true);
	virtual ~M_Audio();

private:

	bool InitWwise();
};

#endif // !__M_Audio_H__
