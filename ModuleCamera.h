#ifndef __MODULECAMERA_H_
#define __MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "Point.h"
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
	bool			CleanUp();

	// Render
	math::float4x4	ProjectionMatrix();
;	math::float4x4	LookAt(math::float3& cameraPos, math::float3& cameraFront, math::float3& cameraUp);
	void			InitFrustum();

	// Movement
	void			CameraMovementKeyboard();
	void			CameraMovementMouse();
	void			MouseUpdate(const iPoint& mousePosition);

	//Helper
	void			SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight);
	void			MoveCamera(CameraMovement cameraSide);
	void			RotateCamera(CameraMovement cameraSide);
	void			SetHorizontalFOV(float& fovXDegrees);
	void			SetVerticalFOV(float& fovYDegrees);
	void			Zooming(bool positive);
	void			FocusObject(math::float3& objectCenterPos);

public:
	Frustum frustum;
	float cameraSpeed;
	float rotationSpeed;
	float mouseSensitivity;
	float fov;

	unsigned screenWidth = SCREEN_WIDTH;
	unsigned screenHeight = SCREEN_HEIGHT;
	float screenRatio = screenWidth / screenHeight;

	float fovY = 45.0f;
	float fovX = 45.0f;
	float zoomValue = 0.0f;

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

	// Center
	math::float3 sceneCenter = math::float3(0.0f, 0.0f, 0.0f);

};

#endif