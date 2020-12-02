#include "Log.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Light.h"
#include "C_Camera.h"

#include "GameObject.h"

GameObject::GameObject() :
id(0),
name("GameObject"),
is_active(true),
is_static(false),
parent(nullptr),
is_root_object(false),
to_delete(false)
{
	transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
}

GameObject::GameObject(uint id, std::string name, bool is_active, bool is_static) :
id(id),
name(name),
is_active(is_active),
is_static(is_static),
parent(nullptr),
is_root_object(false),
to_delete(false)
{
	if (name.empty())
	{
		name = "GameObject";
	}

	transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
}

GameObject::~GameObject()
{

}

bool GameObject::Update()
{
	bool ret = true;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsActive())
		{
			components[i]->Update();
		}
	}

	return ret;
}

bool GameObject::CleanUp()
{
	bool ret = true;

	FreeComponents();

	FreeChilds();

	return ret;
}

// --- GAMEOBJECT METHODS ---
void GameObject::FreeComponents()
{
	transform = nullptr;

	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->CleanUp();

		delete components[i];
		components[i] = nullptr;
	}

	components.clear();
}

void GameObject::FreeChilds()
{
	if (parent != nullptr)
	{
		parent->DeleteChild(this);											// Deleting this GameObject from the childs list of its parent.
	}

	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i] != nullptr)
		{
			childs[i]->to_delete = true;									// Will set the children of the GameObject being deleted to be deleted too in M_Scene's game_objects vector.
			//childs[i]->CleanUp();											// Recursively cleaning up the the childs.
		}
	}

	childs.clear();
}

bool GameObject::AddChild(GameObject* child)
{
	bool ret = true;
	
	if (child->is_root_object)
	{
		LOG("[ERROR] Could not add child %s to %s: %s is current root object!", child->name.c_str(), name.c_str(), child->name.c_str());
		return false;
	}
	
	if (NewChildIsOwnParent(child))
	{
		/*for (uint i = 0; i < child->childs.size(); ++i)				// Iterating all the childs of the child.
		{
			child->parent->AddChild(child->childs[i]);				// Re-setting the parent of the childs to the parent of the passed child (root->GO->childs => root->childs->GO)
		}

		child->childs.clear();*/
		
		LOG("[ERROR] Cannot re-parent parents into their own children!");
		return false;
	}

	if (child->parent != nullptr)
	{	
		child->parent->DeleteChild(child);

		C_Transform* child_transform			= child->GetTransformComponent();
		child_transform->update_local_transform	= true;
	}

	child->parent = this;
	childs.push_back(child);

	return ret;
}

bool GameObject::NewChildIsOwnParent(GameObject* child)
{
	bool ret = false;

	GameObject* parent_item = this->parent;									// Will set the parent of this object as the starting point of the search.
	
	if (parent_item != nullptr)												// Will check if the child is the parent or parent of the parents of the one who called AddChild()
	{
		while (!parent_item->is_root_object)								// Iterate back up to the root object, as it is the parent of everything in the scene.
		{
			if (parent_item == child)										// Child is the parent of one of the parent objects of this object (the one which called AddChild())
			{
				ret = true;													// A parent of this object that had the passed child as the parent has been found.
				break;
			}

			parent_item = parent_item->parent;								// Setting the next parent GameObject to iterate.
		}
	}

	return ret;
}

bool GameObject::DeleteChild(GameObject* child)
{
	bool ret = false;
	
	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i] == child)
		{
			childs.erase(childs.begin() + i);
			ret = true;
			break;
		}
	}
	
	return ret;
}

Component* GameObject::CreateComponent(COMPONENT_TYPE type)
{
	Component* component = nullptr;

	bool check_for_duplicates = false;

	switch(type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		component = new C_Transform(this);
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::MESH:
		component = new C_Mesh(this);
		break;

	case COMPONENT_TYPE::MATERIAL:
		component = new C_Material(this);
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::LIGHT:
		component = new C_Light(this);
		break;

	case COMPONENT_TYPE::CAMERA:
		component = new C_Camera(this);
		break;
	}

	if (component != nullptr)
	{
		if (check_for_duplicates)
		{
			for (uint i = 0; i < components.size(); ++i)
			{
				if (type == components[i]->type)
				{
					LOG("[ERROR] %s Component could not be added to %s: No duplicates allowed!", component->GetName(), name.c_str());
					
					delete component;
					component = nullptr;

					return nullptr;
				}
			}
		}

		components.push_back(component);
	}

	return component;
}

bool GameObject::DeleteComponent(Component* component_to_delete)
{
	if (component_to_delete != nullptr)
	{
		for (uint i = 0; i < components.size(); ++i)
		{
			if (components[i] == component_to_delete)
			{
				components[i]->CleanUp();

				delete components[i];
				components[i] = nullptr;

				components.erase(components.begin() + i);

				return true;
			}
		}
	}

	LOG("[STATUS] Deleted Component %s of Game Object %s", component_to_delete->GetName(), name.c_str());

	return false;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
		{
			return components[i];
		}
	}

	//LOG("[WARNING] Could not find %s Component in %s", GetComponentNameFromType(type), name.c_str());

	return nullptr;
}

const char* GameObject::GetComponentNameFromType(COMPONENT_TYPE type)
{
	switch (type)
	{
	case COMPONENT_TYPE::NONE:		{ return "None"; }		break;
	case COMPONENT_TYPE::TRANSFORM: { return "Transform"; } break;
	case COMPONENT_TYPE::MESH:		{ return "Mesh"; }		break;
	case COMPONENT_TYPE::MATERIAL:	{ return "Material"; }	break;
	case COMPONENT_TYPE::LIGHT:		{ return "Light"; }		break;
	case COMPONENT_TYPE::UNKNOWN:	{ return "Unknown"; }	break;
	}

	LOG("[ERROR] Could Not Get Component Name From Type");

	return "Invalid Component Type";
}

std::vector<Component*> GameObject::GetAllComponentsWithType(COMPONENT_TYPE type)
{
	std::vector<Component*> type_components;
	
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
		{
			type_components.push_back(components[i]);
		}
	}

	return type_components;
}

C_Transform* GameObject::GetTransformComponent()
{
	return (C_Transform*)GetComponent(COMPONENT_TYPE::TRANSFORM);					// The component is returned directly without any checks as the checks will be done in GetComponent().
}

C_Mesh* GameObject::GetMeshComponent()
{
	return (C_Mesh*)GetComponent(COMPONENT_TYPE::MESH);
}

C_Material* GameObject::GetMaterialComponent()
{
	return (C_Material*)GetComponent(COMPONENT_TYPE::MATERIAL);
}

C_Light* GameObject::GetLightComponent()
{
	return (C_Light*)GetComponent(COMPONENT_TYPE::LIGHT);
}

C_Camera* GameObject::GetCameraComponent()
{
	return (C_Camera*)GetComponent(COMPONENT_TYPE::CAMERA);
}

std::vector<C_Mesh*> GameObject::GetAllMeshComponents()
{
	std::vector<C_Mesh*> mesh_components;
	
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == COMPONENT_TYPE::MESH)
		{
			mesh_components.push_back((C_Mesh*)components[i]);
		}
	}

	return mesh_components;
}

// --- GAME OBJECT GETTERS AND SETTERS ---
const char* GameObject::GetName() const
{
	return name.c_str();
}

bool GameObject::IsActive() const
{
	return is_active;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetID(uint id)
{

}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

void GameObject::SetIsActive(const bool& set_to)
{
	is_active = set_to;

	SetChildsIsActive(set_to, this);
}

void GameObject::SetIsStatic(const bool& set_to)
{
	is_static = set_to;

	SetChildsIsStatic(set_to, this);
}

void GameObject::SetChildsIsActive(const bool& set_to, GameObject* parent)
{
	if (parent != nullptr)
	{
		for (uint i = 0; i < parent->childs.size(); ++i)
		{
			parent->childs[i]->is_active = set_to;

			SetChildsIsActive(set_to, parent->childs[i]);
		}
	}
}

void GameObject::SetChildsIsStatic(const bool& set_to, GameObject* parent)
{
	if (parent != nullptr)
	{
		for (uint i = 0; i < parent->childs.size(); ++i)
		{
			parent->childs[i]->is_static = set_to;

			SetChildsIsStatic(set_to, parent->childs[i]);
		}
	}
}