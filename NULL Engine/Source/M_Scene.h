#ifndef __M_SCENE_H__
#define __M_SCENE_H__

#include <map>

#include "Module.h"

class ParsonNode;
class Primitive;
class GameObject;
class R_Texture;

class M_Scene : public Module
{
public:
	M_Scene(bool is_active = true);
	~M_Scene();

	bool			Init				(ParsonNode& config) override;
	bool			Start				() override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			SaveConfiguration	(ParsonNode& root) const override;
	bool			LoadConfiguration	(ParsonNode& root) override;

public:																														// --- GAME OBJECTS METHODS ---
	bool			SaveScene								() const;														// 
	bool			LoadScene								(const char* path);												// 

	std::vector<GameObject*>* GetGameObjects				();
	
	GameObject*		CreateGameObject						(const char* name = nullptr, GameObject* parent = nullptr);		// 
	void			DeleteGameObject						(GameObject* game_object, uint index = -1);						// 
	
public:																														// --- MASTER ROOT & SCENE ROOT METHODS ---
	void			CreateMasterRoot						();																// 
	void			DeleteMasterRoot						();																// 
	GameObject*		GetMasterRoot							() const;														// 

	void			CreateMasterCamera						();
	void			DeleteMasterCamera						();
	GameObject*		GetMasterCamera							() const;

	void			CreateSceneRoot							(const char* scene_name);										//
	GameObject*		GetSceneRoot							() const;														//
	void			SetSceneRoot							(GameObject* game_object);										//
	void			ChangeSceneName							(const char* new_name);											//

	void			CreateSceneCamera						(const char* camera_name);
	bool			GameObjectIsInsideSceneCamera			(GameObject* game_object);

public:																														// --- SELECTED GAME OBJECT METHODS ---
	void			DeleteSelectedGameObject				();																// 
	GameObject*		GetSelectedGameObject					() const;														// 
	void			SetSelectedGameObject					(GameObject* game_object);										// 
	bool			ApplyNewTextureToSelectedGameObject		(R_Texture* path);												//

private:
	void			HandleDebugInput();
	void			DebugSpawnPrimitive(Primitive* p);

private:
	std::vector<GameObject*>	game_objects;																				// 

	GameObject*					master_root;																				// Root of everything. Parent of all scenes.
	GameObject*					master_camera;																				// Main camera of the engine.
	GameObject*					scene_root;																					// Root of the current scene.
	GameObject*					scene_camera;																				// Camera that is currently used in the scene. (Game Camera)
	GameObject*					selected_game_object;																		// Represents the game object that's currently being selected.

	std::vector<Primitive*>		primitives;
};

#endif // !__M_SCENE_H__
