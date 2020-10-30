#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Light.h"

#include "GameObject.h"

GameObject::GameObject() : id(0), name("GameObject"), is_active(true), is_static(false), parent (nullptr)
{

}

GameObject::GameObject(uint id, std::string name, bool is_active, bool is_static) :
id(id),
name(name),
is_active(is_active),
is_static(is_static),
parent(nullptr)
{
	if (name.empty())
	{
		name = "GameObject";
	}
}

GameObject::~GameObject()
{

}

bool GameObject::Update()
{
	bool ret = true;

	//for (components)

	return ret;
}

bool GameObject::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < components.size(); ++i)
	{
		delete components[i];
		components[i] = nullptr;
	}

	components.clear();

	for (uint i = 0; i < childs.size(); ++i)
	{
		delete childs[i];
		childs[i] = nullptr;
	}

	childs.clear();

	return ret;
}

// --- GAMEOBJECT METHODS ---
bool GameObject::AddChild(GameObject* child)
{
	bool ret = true;

	if (child->parent != nullptr)
	{
		child->parent->DeleteChild(child);
	}

	child->parent = this;

	childs.push_back(child);

	if (!child->childs.empty())
	{
		for (uint i = 0; i < child->childs.size(); ++i)
		{
			AddChild(child->childs[i]);
		}

		child->childs.clear();
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
		component = new C_Transform();
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::MESH:
		component = new C_Mesh();
		break;

	case COMPONENT_TYPE::MATERIAL:
		component = new C_Material();
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::LIGHT:
		component = new C_Light();
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

void GameObject::SetID(uint id)
{

}