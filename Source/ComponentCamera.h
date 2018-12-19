#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/MathFunc.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "SDL.h"
#include "GL/glew.h"

class ComponentCamera : public Component
{
	public:
		ComponentCamera(GameObject* goParent);
		~ComponentCamera();

		void			DrawProperties() override;
		Component*		Duplicate() override;
		void			Update();

		void			InitFrustum();

		void			LookAt(math::float3 target);
		math::float4x4	GetViewMatrix();
		math::float4x4	GetProjectionMatrix();

		void			Rotate(float dx, float dy);
		void			Orbit(float dx, float dy);

		void			SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight);
		void			SetHorizontalFOV(float fovXDegrees);
		void			SetVerticalFOV(float fovYDegrees);

		void			CreateFrameBuffer();

	public:
		math::Frustum	frustum;

		math::float3	cameraPosition = math::float3(0.0f, 3.0f, 20.0f);
		math::float3	cameraFront = math::float3(0.0f, 0.0f, -1.0f);
		math::float3	cameraUp = math::float3(0.0f, 1.0f, 0.0f);

		// Camera specs
		float			maxFov = 100.0f;
		float			minFov = 10.0f;
		float			pitch = 0.0f;
		float			yaw = 0.0f;

		unsigned		screenWidth = SCREEN_WIDTH;
		unsigned		screenHeight = SCREEN_HEIGHT;

		float			screenRatio = screenWidth / screenHeight;

		float			cameraSpeed = 15.0f;
		float			rotationSpeed = 65.0f;
		float			mouseSensitivity = 0.2f;

		float			fovY = 45.0f;
		float			fovX = 45.0f;
		float			zoomValue = 0.0f;

		unsigned		fbo = 0u;
		unsigned		rbo = 0u;
		unsigned		renderTexture = 0u;
};

#endif
