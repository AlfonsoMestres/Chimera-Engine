#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include <list>
#include "Module.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class GameObject;
enum class ComponentType;

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
		GameObject* ModuleScene::CreateCamera(GameObject* goParent = nullptr, const math::float4x4& transform = math::float4x4().identity);
		/*GameObject* GenerateSphere(GameObject* goParent, int slices, int stacks, const math::float3& position = math::float3(0.0f, 0.0f, 0.0f), const math::Quat& rotation = math::Quat::identity,
										const math::float3& size = math::float3(1.0f,1.0f,1.0f), const math::float4& color = math::float4(0.3f, 0.7f, 0.2f, 1.0f));*/
		/*GameObject* ModuleScene::CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation,
											float height, float radius, unsigned slices, unsigned stacks, const math::float4& color);*/

	public:
		GameObject* root = nullptr;
		GameObject* goSelected = nullptr;
};

#endif
