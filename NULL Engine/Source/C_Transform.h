#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "MathGeoLib/src/Math/float3.h"

class GameObject;

class C_Transform : public Component
{
public:
	C_Transform(GameObject* owner);
	~C_Transform();

	bool Update() override;
	bool CleanUp() override;

public:
	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetScale() const;
	
	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);

public:
	bool	recalculate_local_transform;					// Set to true if the parent of the owner object has been changed. That would mean recalculating the local position.

private:
	float3	position;
	float3	rotation;
	float3	scale;

	float3	local_position;
	float3	local_rotation;
	float3	local_scale;

	bool	needs_translation;
	bool	needs_rotation;
	bool	needs_scaling;
};

#endif // !_C_TRANSFORM_H__