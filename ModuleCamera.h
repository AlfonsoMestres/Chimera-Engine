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
	Pitch,
	Yaw
};

public:

	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status	PreUpdate();
	update_status	Update();
	bool			CleanUp();
	math::float4x4	ProjectionMatrix();
;	math::float4x4	LookAt(math::float3& cameraPos, math::float3& cameraFront, math::float3& cameraUp);
	void			InitFrustum();
	void			MouseUpdate(int mouseXpos, int mouseYpos);
	void			SetHorizontalFOV(float& fovXDegrees);
	void			SetVerticalFOV(float& fovYDegrees);
	void			SetScreenNewScreenSize(float& newWidth, float& newHeight);
	void			MoveCamera(CameraMovement cameraSide);
	void			RotateCamera();

public:
	Frustum frustum;
	float cameraSpeed;
	float rotationSpeed;
	float mouseSensitivity;
	float fov;

	float screenWidth = SCREEN_WIDTH;
	float screenHeight = SCREEN_HEIGHT;
	float screenRatio = screenWidth / screenHeight;

	float fovY = 45.0f;
	float fovX = 45.0f;

	// Camera rotations
	float pitch;
	float yaw;

	// Camera states
	math::float3 cameraPos;
	math::float3 cameraFront;
	math::float3 cameraUp;

	// Mouse 
	bool firstMouse = true;
	int lastX = 0;
	int lastY = 0;

};

#endif