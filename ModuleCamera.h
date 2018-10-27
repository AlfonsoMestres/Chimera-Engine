#ifndef __MODULECAMERA_H_
#define __MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "SDL.h"


class ModuleCamera : public Module
{

enum CameraMovement {
	Upwards,
	Downwards,
	Left,
	Right,
	Forward,
	Backwards
};

enum CameraRotation {
	PositivePitch,
	NegativePitch,
	PositiveYaw,
	NegativeYaw
};

public:

	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
;	math::float4x4	LookAt(math::float3& target, math::float3& eye, math::float3& up);
	math::float4x4	ProjectionMatrix();
	void			InitFrustum();
	void			MouseUpdate(math::float2& mouseNewPosition);
	void			SetFOV();
	void			MoveCamera(CameraMovement cameraSide);
	void			RotateCamera(CameraRotation cameraSide);

public:
	math::float2 oldMousePos;
	Frustum frustum;
	float cameraSpeed = 0.5f;
	// Camera rotations
	float pitch;
	float yaw;

	// Camera states
	math::float3& target = math::float3(0.0f, 0.0f, 0.0f);
	math::float3& eye = math::float3(0.0f, 0.0f, 0.0f);
	math::float3& up = math::float3(0.0f, 0.0f, 0.0f);
	math::float3 forw;
	math::float3 sidew;
	math::float3 upw;

	// Mouse 
	bool firstMouse = true;
	float lastX = SCREEN_WIDTH / 2;
	float lastY = SCREEN_HEIGHT / 2;

};

#endif