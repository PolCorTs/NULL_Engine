#ifndef __C_AudioListener__H__
#define __C_AudioListener__H__

#include "Component.h"
#include "M_Audio.h"
#include <AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject;

class C_AudioListener : public Component
{
public:
	C_AudioListener(COMPONENT_TYPE type, GameObject* parent, bool active);
	~C_AudioListener();

	void DrawInspector();
	void Update();

	void Load(const char* gameObject, const json& file);
	void Save(const char* gameObject, json& file);

private:

	WwiseGameObject* wwiseGO = nullptr;
	AkGameObjectID id = 0;
};

#endif // !__C_AudioListener__H__
