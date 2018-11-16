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

	public:

		ModuleCamera();
		~ModuleCamera();

		bool Init();
		update_status	PreUpdate() override;
		bool			CleanUp() override;
		void			DrawGUI();

		// Movement
		void			CameraMovementKeyboard();
		void			RotateCamera(const fPoint& mousePosition, bool orbit = false);
		void			MoveCamera(CameraMovement cameraSide);

		// Helpers
		void			FocusSelectedObject();
		void			UpdatePitchYaw();
		void			Zoom();

	public:
		Model* selectedObject = nullptr;

		// User attributes
		float mouseSensitivity = 65.0f;
		float rotationSpeed = 65.0f;;
		float cameraSpeed = 17.0f;;

		// Camera specs
		float maxFov = 100.0f;
		float minFov = 10.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		math::float3 cameraPos = math::float3(10.0f, 10.0f, 10.0f);

		//ViewMatrix info
		math::float3 front = math::float3(-0.577350259f, -0.577350259f, -0.577350259f);
		math::float3 side;
		math::float3 up = math::float3(0.0f, 1.0f, 0.0f);

		// Mouse 
		bool firstMouse = true;
		float lastX = 0.0f;
		float lastY = 0.0f;
};

#endif