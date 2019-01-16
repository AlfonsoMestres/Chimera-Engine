#ifndef __COMPONENTCAMERA_H__
#define __COMPONENTCAMERA_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/MathFunc.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "SDL.h"
#include "GL/glew.h"

class GameObject;

class ComponentCamera : public Component
{
	public:
		ComponentCamera(GameObject* goParent);
		~ComponentCamera();

		void			DrawProperties(bool enabled) override;
		Component*		Duplicate() override;
		void			Update() override;

		void			InitFrustum(math::float3 camPos = math::float3(0.0f, 300.0f, 1000.0f), math::float3 camFront = math::float3(0.0f, 0.0f, -1.0f), math::float3 camUp = float3::unitY);
		void			InitOrthographicFrustum(math::float3 camPos = math::float3(0.0f, 300.0f, 1000.0f), math::float3 camFront = math::float3(0.0f, 0.0f, -1.0f), math::float3 camUp = float3::unitY);

		void			LookAt(math::float3 target);
		math::float4x4	GetViewMatrix() const;
		math::float4x4	GetProjectionMatrix() const;

		void			Rotate(float dx, float dy);
		void			Orbit(float dx, float dy);
		void			SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight);

		void			Load(Config* config, rapidjson::Value& value) override;
		void			Save(Config* config) override;

	private:
		void			CreateFrameBuffer(float winWidth, float winHeight);
		void			SetHorizontalFOV(float fovXDegrees);
		void			SetVerticalFOV(float fovYDegrees);

	public:
		math::Frustum	frustum;

		// Camera specs
		float			maxFov = 80.0f;
		float			minFov = 20.0f;
		float			fovY = 45.0f;
		float			fovX = 45.0f;
		float			cameraSpeed = 0.1f;
		float			rotationSpeed = 0.5f;
		float			mouseSensitivity = 0.15f;
		float			zoomValue = 0.0f;

		unsigned		screenWidth = SCREEN_WIDTH;
		unsigned		screenHeight = SCREEN_HEIGHT;

		unsigned		fbo = 0u;
		unsigned		rbo = 0u;
		unsigned		renderTexture = 0u;

		//Camera debug settings
		bool			debugDraw = true;
		int				wireFrame = GL_FILL;

};

#endif
