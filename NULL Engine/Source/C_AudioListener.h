#ifndef __C_AudioListener__H__
#define __C_AudioListener__H__

#include "Component.h"
#include "M_Audio.h"
#include <Wwise/AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject;

class C_AudioListener : public Component
{
public:
	C_AudioListener(GameObject* owner);
	~C_AudioListener();

	void DrawInspector();
	bool Update();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

private:

	WwiseGameObject* wwiseGO = nullptr;
	AkGameObjectID id = 0;
};

#endif __C_AudioListener__H__
