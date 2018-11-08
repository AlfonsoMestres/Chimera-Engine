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
;	void			LookAt(math::float3& cameraPos, math::float3& cameraFront);
	void			InitFrustum();
	void			UpdatePitchYaw();

	// Movement
	void			CameraMovementKeyboard();
	void			RotateCameraKeyBoard(CameraMovement cameraSide, bool orbit = false);
	void			RotateCameraMouse(const iPoint& mousePosition, bool orbit = false);
	void			MoveCamera(CameraMovement cameraSide);

	//Helper
	void			SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight);
	void			FocusObject(math::float3& objectCenterPos);
	void			SetHorizontalFOV(float& fovXDegrees);
	void			SetVerticalFOV(float& fovYDegrees);
	void			Zooming();

public:
	float mouseSensitivity;
	float rotationSpeed;
	float cameraSpeed;
	Frustum frustum;
	float zoomSpeed;

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

	//ViewMatrix info
	math::float4x4 viewMatrix;
	math::float3 front;
	math::float3 side;
	math::float3 up;

	// Mouse 
	bool firstMouse = true;
	int lastX = 0;
	int lastY = 0;

	// Center
	math::float3 objectCenter = math::float3(0.0f, 0.0f, 0.0f);

};

#endif