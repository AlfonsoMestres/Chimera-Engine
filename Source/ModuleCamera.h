#ifndef __MODULECAMERA_H_
#define __MODULECAMERA_H_

#include "Module.h"
#include "Globals.h"
#include "Point.h"
#include "MathGeoLib.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "GL/glew.h"
#include "SDL.h"

class Camera;

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

		bool			Init() override;
		update_status	PreUpdate() override;
		update_status	Update() override;
		bool			CleanUp() override;
		void			DrawGUI();

		// Scene camera movement
		void			Move();
		void			MovementSpeed();

		// Helpers
		void			FocusSelectedObject();
		void			Zoom();

		void SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight);

	public:
		ComponentCamera* sceneCamera = nullptr;
		ComponentCamera* selectedCamera = nullptr;
		std::list<ComponentCamera*> gameCameras;
		
		GameObject* goSelected = nullptr;

		// User attributes
		float mouseSensitivity = 65.0f;
		float rotationSpeed = 65.0f;
		float cameraSpeed = 17.0f;
		
		// Mouse 
		bool firstMouse = true;
		float lastX = 0.0f;
		float lastY = 0.0f;
};

#endif