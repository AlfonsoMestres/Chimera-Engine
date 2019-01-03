#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

enum class GeometryType {
	SPHERE,
	TORUS,
	PLANE,
	CUBE
};

class ModuleScene : public Module
{
	public:
		ModuleScene();
		~ModuleScene();

		bool Init() override;
		update_status Update() override;
		void Draw(const math::Frustum& frustum) const;
		void DrawHierarchy();

		GameObject* CreateGameObject(const char* goName = nullptr, GameObject* goParent = nullptr, 
										const math::float4x4& transform = math::float4x4().identity, const char* fileLocation = nullptr);
		GameObject* CreateCamera(GameObject* goParent = nullptr, const math::float4x4& transform = math::float4x4().identity);
		void		LoadGeometry(GameObject* goParent, GeometryType geometryType);

	public:
		GameObject* root = nullptr;
		GameObject* goSelected = nullptr;

		math::float3 lightPosition = math::float3(0.0f, 10.0f, 10.0f);
		float ambientLight = 0.3f;
};

#endif
