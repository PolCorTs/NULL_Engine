#ifndef __M_Audio__H__
#define __M_Audio__H__

#include "Module.h"
#include "glmath.h"
#include "globals.h"
#include <Wwise/AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject
{
	WwiseGameObject(unsigned __int64 id, const char* name);
	~WwiseGameObject();

public:
	void SetPosition(float posX = 0, float posY = 0, float posZ = 0, float frontX = 1, float frontY = 0, float frontZ = 0, float topX = 0, float topY = 1, float topZ = 0);
	void PlayEvent(uint id);
	void PauseEvent(uint id);
	void ResumeEvent(uint id);
	void StopEvent(uint id);
	void SetVolume(uint id, float volume);
	WwiseGameObject* CreateAudioSource(uint id, const char* name, vec3 position);
	WwiseGameObject* CreateAudioListener(uint id, const char* name, vec3 position);
	void SetAuxSends();

public:
	uint GetID();

public:
	float volume = 1.0f;

private:
	const char* name = nullptr;
	AkGameObjectID id;

	AkVector position = { 0,0,0 };
	AkVector orientationFront = { 0,0,0 };
	AkVector orientationTop = { 0,0,0 };
};

class M_Audio : public Module
{
public:
	M_Audio(bool start_enabled = true);
	virtual ~M_Audio();

	bool Init();
	bool Start();
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();
	void Tests(AkGameObjectID id);

private:

	void InitWwise();
	void TerminateWwise();
	void LoadSoundBank(const char* path);

public:
	AkGameObjectID currentListenerID;
	unsigned long g_envMAP[255];

};

#endif __M_Audio__H__
