#ifndef __C_AudioSource__H__
#define __C_AudioSource__H__

#include "Component.h"
#include <Wwise/AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject;

class C_AudioSource : public Component
{
public:
	C_AudioSource(GameObject* owner);
	~C_AudioSource();

	void DrawInspector();
	bool Update();

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	void SetID(AkGameObjectID id);

public:

	WwiseGameObject* wwiseGO = nullptr;
	AkGameObjectID id = 0;
	bool isPaused = false;
	bool isPlaying = false;
};

#endif // !__C_AudioSource__H__
