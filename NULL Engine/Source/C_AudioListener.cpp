#include "C_AudioListener.h"
#include "Application.h"
#include "M_Camera3D.h"
#include "M_Audio.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "M_Scene.h"

C_AudioListener::C_AudioListener(GameObject* owner) : Component(owner, COMPONENT_TYPE::AUDIO_LISTENER)
{
	

	vec3 position;
	position.x = owner->transform->local_position.x;
	position.y = owner->transform->local_position.y;
	position.z = owner->transform->local_position.z;

	wwiseGO = wwiseGO->CreateAudioListener(owner->uid, owner->name.c_str(), position);
}

C_AudioListener::~C_AudioListener() {}

void C_AudioListener::DrawInspector()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_Leaf))
	{

	}
}

bool C_AudioListener::Update()
{
	bool ret = true;
	C_Camera* cam = App->camera->current_camera;
	vec pos = cam->frustum.pos;
	vec front = cam->frustum.front;
	vec top = cam->frustum.up;

	wwiseGO->SetPosition(pos.x, pos.y, pos.z, front.x, front.y, front.z, top.x, top.y, top.z);

	return ret;
}

bool C_Transform::SaveState(ParsonNode& root) const
{
	bool ret = true;

	return ret;
}

bool C_Transform::LoadState(ParsonNode& root)
{
	bool ret = true;

	return ret;
}