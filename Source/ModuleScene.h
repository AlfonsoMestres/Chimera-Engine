#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class ModuleScene : public Module
{
	public:
		ModuleScene();
		~ModuleScene();

		bool Init() override;
		update_status Update() override;
		void Draw();
		void DrawHierarchy();

		GameObject* CreateGameObject(const char* goName = nullptr, GameObject* goParent = nullptr, 
										const math::float4x4& transform = math::float4x4().identity, const char* fileLocation = nullptr);
		GameObject* CreateCamera(GameObject* goParent = nullptr, const math::float4x4& transform = math::float4x4().identity);
		GameObject* GenerateSphere(GameObject* goParent, int slices, int stacks, const math::float3& pos, const math::Quat& rot, const float size, const math::float4& color);
		GameObject* GenerateCylinder(GameObject* goParent, const math::float3& pos, const math::Quat& rot, float height, float radius, unsigned slices, unsigned stacks, const math::float4& color);

	public:
		GameObject* root = nullptr;
		GameObject* goSelected = nullptr;
};

#endif
