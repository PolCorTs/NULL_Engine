#include "C_AudioSource.h"
#include "Application.h"
#include "M_Camera3D.h"
#include "M_Audio.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "M_Scene.h"
#include "ImGui/include/imgui.h"
#include <string.h>

C_AudioSource::C_AudioSource(GameObject* owner) : Component(owner, COMPONENT_TYPE::AUDIO_SOURCE)
{
	vec3 position;
	position.x = owner->transform->local_position.x;
	position.y = owner->transform->local_position.y;
	position.z = owner->transform->local_position.z;

	wwiseGO = wwiseGO->CreateAudioSource(owner->uid, owner->name.c_str(), position);
}

C_AudioSource::~C_AudioSource() {}

void C_AudioSource::DrawInspector()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (id != 0)
		{
			string state = "Pause";

			if (ImGui::ArrowButton("Play", ImGuiDir_Right))
			{
				if (!isPlaying)
				{
					wwiseGO->PlayEvent(id);
					isPlaying = true;
				}
				isPaused = false;
			}

			ImGui::SameLine();


			if (isPaused)
				state = "Resume";

			else
				state = "Pause";

			if (ImGui::Button(state.c_str()))
			{
				if (isPlaying)
				{
					isPlaying = false;
					isPaused = true;
					wwiseGO->PauseEvent(id);
				}
				else if (isPaused)
				{
					isPlaying = true;
					isPaused = false;
					wwiseGO->ResumeEvent(id);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("STOP"))
			{
				isPlaying = false;
				isPaused = false;
				wwiseGO->StopEvent(id);
			}

			if (ImGui::SliderFloat("Volume", &wwiseGO->volume, 0, 10))
			{
				wwiseGO->SetVolume(id, wwiseGO->volume);
			}
		}
	}
}

bool C_AudioSource::Update()
{
	bool ret = true;
	vec3 position;

	position.x = owner->transform->local_position.x;
	position.y = owner->transform->local_position.y;
	position.z = owner->transform->local_position.z;

	wwiseGO->SetPosition(position.x, position.y, position.z);
	return ret;
}
bool C_AudioSource::SaveState(ParsonNode& root) const
{
	bool ret = true;

	return ret;
}

bool C_AudioSource::LoadState(ParsonNode& root)
{
	bool ret = true;

	return ret;
}
void C_AudioSource::SetID(AkGameObjectID id)
{
	this->id = id;
}