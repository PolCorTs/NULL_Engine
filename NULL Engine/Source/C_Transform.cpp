#include "MathGeoLib/src/Math/TransformOps.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
matrix(matrix.identity),
recalculate_global_transform(false)
{
	float4x4 rotation_mat;
	
	matrix.Decompose(position, rotation_mat, scale);

	rotation = rotation_mat.ToEulerXYZ();

	int a = 0;

	if (owner->parent != nullptr)
	{
		//owner->parent->transform->position
	}
}

C_Transform::~C_Transform()
{

}

bool C_Transform::Update()
{
	bool ret = true;

	if (recalculate_global_transform)
	{
		matrix.Translate(position);

		matrix.RotateX(rotation.x);
		matrix.RotateY(rotation.y);
		matrix.RotateZ(rotation.z);

		matrix.Scale(scale);

		recalculate_global_transform = false;
	}

	return ret;
}

bool C_Transform::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- C_TRANSFORM METHODS ---
float3 C_Transform::GetPosition() const
{
	return position;
}

float3 C_Transform::GetRotation() const
{
	return rotation;
}

float3 C_Transform::GetScale() const
{
	return scale;
}

void C_Transform::SetPosition(const float3& position)
{
	this->position = position;

	recalculate_global_transform = true;
}

void C_Transform::SetRotation(const float3& rotation)
{
	this->rotation = rotation;

	recalculate_global_transform = true;
}

void C_Transform::SetScale(const float3& scale)
{
	this->scale = scale; 

	recalculate_global_transform = true;
}