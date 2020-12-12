#include "Application.h"															// ATTENTION: Globals.h already included in Application.h.
#include "M_Window.h"
#include "M_Input.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"

#include "M_Camera3D.h"

#define MOVEMENT_SPEED 12.0f
#define ROTATION_SPEED 12.0f
#define ZOOM_SPEED 24.0f

M_Camera3D::M_Camera3D(bool is_active) : Module("Camera3D", is_active),
master_camera(nullptr),
current_camera(nullptr)
{
	CreateMasterCamera();
	
	CalculateViewMatrix();

	X					= vec3(1.0f, 0.0f, 0.0f);									//
	Y					= vec3(0.0f, 1.0f, 0.0f);									//
	Z					= vec3(0.0f, 0.0f, 1.0f);									//

	position_origin		= vec3(60.0f, 40.0f, 60.0f);								//
	reference_origin	= vec3(0.0f, 0.0f, 0.0f);									//
	position			= position_origin;											// 
	reference			= reference_origin;											// 

	LookAt(reference);

	movement_speed		= MOVEMENT_SPEED;
	rotation_speed		= ROTATION_SPEED;
	zoom_speed			= ZOOM_SPEED;
}

M_Camera3D::~M_Camera3D()
{
	current_camera = nullptr;
	
	master_camera->CleanUp();
	RELEASE(master_camera);
}

// -----------------------------------------------------------------
bool M_Camera3D::Init(ParsonNode& root)
{
	//Position.x = root.GetNumber("X");
	//Position.y = root.GetNumber("Y");
	//Position.z = root.GetNumber("Z");
	
	master_camera->GetTransformComponent()->SetLocalPosition(float3(60.0f, 40.0f, 60.0f));
	//current_camera->UpdateFrustumTransform();

	return true;
}

// -----------------------------------------------------------------
bool M_Camera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool M_Camera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool M_Camera3D::LoadConfiguration(ParsonNode& root)
{
	bool ret = true;

	//Position.x = root.GetNumber("X");
	//Position.y = root.GetNumber("Y");
	//Position.z = root.GetNumber("Z");

	return ret;
}

bool M_Camera3D::SaveConfiguration(ParsonNode& root) const
{
	bool ret = true;

	//root.SetNumber("X", Position.x);
	//root.SetNumber("Y", Position.y);
	//root.SetNumber("Z", Position.z);

	LOG("SAVED CAMERA INFO");

	return ret;
}

// -----------------------------------------------------------------
UPDATE_STATUS M_Camera3D::Update(float dt)
{
	if (App->editor->EditorSceneIsBeingClicked())
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
		{
			WASDMovement();

			FreeLookAround();
		}

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
			{
				RotateAroundReference();
			}
		}

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
		{
			PanCamera();
		}

		if (App->input->GetMouseZ() != 0)
		{
			Zoom();
		}

		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
		{
			ReturnToWorldOrigin();
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN)
		{
			Focus(reference);
		}
	}

	return UPDATE_STATUS::CONTINUE;
}

// -----------------------------------------------------------------
void M_Camera3D::CreateMasterCamera()
{
	master_camera = new GameObject();
	master_camera->SetName("MasterCamera");
	master_camera->CreateComponent(COMPONENT_TYPE::CAMERA);
	SetCurrentCamera(master_camera->GetCameraComponent());
	
	if (App != nullptr)
	{
		master_camera->GetCameraComponent()->SetAspectRatio(App->window->GetWidth() / App->window->GetHeight());
	}
}

C_Camera* M_Camera3D::GetCurrentCamera() const
{
	return current_camera;
}

void M_Camera3D::SetCurrentCamera(C_Camera* c_camera)
{
	if (c_camera == nullptr)
	{
		LOG("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component was nullptr.");
		return;
	}

	if (c_camera->GetOwner() == nullptr)																						// Highly unlikely case. Gets checked just to make sure.
	{
		LOG("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component's owner was nullptr.");
		return;
	}

	if (current_camera != nullptr)
	{
		current_camera->SetFrustumIsHidden(false);
	}
	
	c_camera->SetFrustumIsHidden(true);
	current_camera = c_camera;
}

void M_Camera3D::SetMasterCameraAsCurrentCamera()
{
	if (master_camera != nullptr)
	{
		LOG("[ERROR] Camera: Could not set the master camera as the current camera! Error: Master Camera was nullptr.");
		LOG("[WARNING] Camera: Created a new Master Camera. Reason: Master Camera was nullptr!");
		
		CreateMasterCamera();
	}

	if (master_camera->GetCameraComponent() == nullptr)
	{
		LOG("[ERROR] Camera: Could not set the master camera as the current camera! Error: Master Camera did not have a Camera Component.");
		LOG("[WARNING] Camera: Created a new Camera Component for the Master Camera. Reason: Master Camera did not have a Camera Component!");

		master_camera->CreateComponent(COMPONENT_TYPE::CAMERA);
	}

	current_camera = master_camera->GetCameraComponent();
}

// -----------------------------------------------------------------
void M_Camera3D::PointAt(const vec3 &position, const vec3 &reference, bool RotateAroundReference)
{
	this->position = position;												// Updates the position to the given one.
	this->reference = reference;											// Updates the reference to the given one.

	Z = normalize(position - reference);									// Forward vector. Where the camera is looking.
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));						// The Right vector is reconstructed as Normal/Orthographic to Z. Cross product returns a vector perp to two others.
	Y = cross(Z, X);														// The Up vector is reconstructed as the cross product of Z and X. This means that Y is perpendicular to both.

	if(!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void M_Camera3D::LookAt(const vec3 &spot)									// Almost identical to PointAt except only the reference and XYZ are updated. DOES NOT TRANSLATE.
{
	reference = spot;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}

// -----------------------------------------------------------------
void M_Camera3D::Focus(const vec3& target_position)
{

}

// -----------------------------------------------------------------
void M_Camera3D::Move(const vec3 &Movement)
{
	position += Movement;
	reference += Movement;
}

// -----------------------------------------------------------------
void M_Camera3D::ReturnToWorldOrigin()
{
	PointAt(position_origin, reference_origin, true);
}

// -----------------------------------------------------------------
void M_Camera3D::WASDMovement()
{
	float3 new_position		= float3::zero;
	Frustum frustum			= current_camera->GetFrustum();
	float mov_speed			= movement_speed * App->GetUnpausableDt();
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)								// --- CAMERA MOVEMEMENT BOOST
	{																									// 
		mov_speed = movement_speed * 2 * App->GetUnpausableDt();										// 
	}																									// ---------------------------


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)									// --- FORWARD/BACKARD MOVEMENT (+Z/-Z)
	{																									// 
		new_position += frustum.Front() * mov_speed;													// 
	}																									// 
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)									// 
	{																									// 
		new_position -= frustum.Front() * mov_speed;													// 
	}																									// ----------------------------------------


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)									// --- LEFT/RIGHT MOVEMENT (STRAFE -X/+X)
	{																									// 										
		new_position -= frustum.WorldRight() * mov_speed;												// 										
	}																									// 										
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)									// 										
	{																									// 										
		new_position += frustum.WorldRight() * mov_speed;												// 										
	}																									// ----------------------------------------


	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_REPEAT)									// --- UPWARD/DOWNWARD MOVEMENT (+Y/-Y)
	{																									// 
		new_position += frustum.Up() * mov_speed;														// 
	}																									// 
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)									// 
	{																									// 
		new_position -= frustum.Up() * mov_speed;														// 
	}																									// ------------------------------------

	current_camera->Move(new_position);
}

void M_Camera3D::FreeLookAround()
{
	// Free Look
	int dx = -App->input->GetMouseXMotion();							// Motion value registered by the mouse in the X axis. Negated so the camera behaves like it should.
	int dy = -App->input->GetMouseYMotion();							// Motion value registered by the mouse in the Y axis. Negated so the camera behaves like it should.

	float sensitivity = rotation_speed * App->GetDt();					// Factor that will be applied to dx before constructing the angle with which to rotate the vectors.

	if (dx != 0)														// --- 
	{
		float delta_X = (float)dx * sensitivity;						// The value of the angle that we will rotate the camera by is very, very small, as it will be applied each frame.

		
		
		X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));					// All vectors of the camera (X = Right, Y = Up, Z = Forward), will be rotated by the value of the angle (delta_X)
		Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));					// The axis of rotation will be Y (yaw), not to confuse with the Y vector, which belongs to the camera.
		Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));					// Keep in mind that X(Right) will always remain axis aligned.
	}

	if (dy != 0)														// Same as above but only affecting the Y and Z vectors, as X will act as the pivot of the rotation.
	{
		float delta_Y = (float)dy * sensitivity;

		Y = rotate(Y, delta_Y, X);										// As stated above, X(Right) will be used as the X axis (pitch) as, even if it is rotated, it will always be perfectly
		Z = rotate(Z, delta_Y, X);										// axis aligned in space, at least for this case.

		if (Y.y < 0.0f)													// If the y component of the Y(Up) vector is negative.
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);			// The y component of Z(Forward) will be recalculated.
			Y = cross(Z, X);											// A new Y(Up) vector orthogonal to both Z(Forward) and X(Right) (cross product) will be calculated.
		}
	}
}

void M_Camera3D::RotateAroundReference()								// Almost identical to FreeLookAround(), but instead of only modifying XYZ, the position of the camera is also modified.
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float sensitivity = rotation_speed * App->GetDt();

	position -= reference;												// One of the two differences with FreeLookAround().

	if (dx != 0)
	{
		float delta_X = (float)dx * sensitivity;

		X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float delta_Y = (float)dy * sensitivity;

		Y = rotate(Y, delta_Y, X);
		Z = rotate(Z, delta_Y, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	position = reference + Z * length(position);						// While FreeLookAround() rotates the camera on its axis, this method also translates the camera.
}																		// This makes the camera rotate around the reference as the distance will be maintained but pos will be affected by Z.

void M_Camera3D::PanCamera()
{
	float3 new_X		= float3::zero;
	float3 new_Y		= float3::zero;
	float3 new_position = float3::zero;
	Frustum frustum		= current_camera->GetFrustum();

	float dx			= (float)App->input->GetMouseXMotion();
	float dy			= (float)App->input->GetMouseYMotion();

	if (dx != 0)
	{
		new_X = -dx * frustum.WorldRight() * App->GetDt();
	}

	if (dy != 0)
	{
		new_Y = dy * frustum.Up() * App->GetDt();
	}

	new_position = new_X + new_Y;
	
	current_camera->Move(new_position);
}

void M_Camera3D::Zoom()
{	
	Frustum frustum		= current_camera->GetFrustum();
	float3 new_Z		= frustum.Front() * (float)App->input->GetMouseZ() * zoom_speed * App->GetDt();

	current_camera->Move(new_Z);
}

// -----------------------------------------------------------------
float* M_Camera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();
	return &ViewMatrix;
}

mat4x4 M_Camera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return ViewMatrix;
}

// -----------------------------------------------------------------
void M_Camera3D::CalculateViewMatrix()
{
	vec3 T = { -dot(X, position), -dot(Y, position), -dot(Z, position) };			// The camera's current transform is calculated.
	
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, 
						X.y, Y.y, Z.y, 0.0f, 
						X.z, Y.z, Z.z, 0.0f, 
						T.x, T.y, T.z, 1.0f);

	ViewMatrixInverse = inverse(ViewMatrix);
}

vec3 M_Camera3D::GetPosition() const
{
	return position;
}

vec3 M_Camera3D::GetReference() const
{
	return reference;
}

vec3 M_Camera3D::GetSpot() const
{
	return vec3(0.0f, 0.0f, 0.0f);
}

void M_Camera3D::SetPosition(const vec3& position)
{
	this->position = position;
}

void M_Camera3D::SetReference(const vec3& reference)
{
	this->reference = reference;
}

void M_Camera3D::SetSpot(const vec3& spot)
{

}

float M_Camera3D::GetMovementSpeed() const
{
	return movement_speed;
}

float M_Camera3D::GetRotationSpeed() const
{
	return rotation_speed;
}

float M_Camera3D::GetZoomSpeed() const
{
	return zoom_speed;
}

void M_Camera3D::SetMovementSpeed(const float& movement_speed)
{
	this->movement_speed = movement_speed;
}

void M_Camera3D::SetRotationSpeed(const float& rotation_speed)
{
	this->rotation_speed = rotation_speed;
}

void M_Camera3D::SetZoomSpeed(const float& zoom_speed)
{
	this->zoom_speed = zoom_speed;
}