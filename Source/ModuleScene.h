#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "Math/Quat.h"
#include "Math/float3.h"
#include "Math/float4.h"
#include "Math/float4x4.h"
#include "prettywriter.h"
#include "document.h"

enum class GeometryType {
	SPHERE,
	TORUS,
	PLANE,
	CUBE
};

class Config;
class GameObject;
class QuadTreeChimera;

class ModuleScene : public Module
{
	public:
		ModuleScene();
		~ModuleScene();

		bool			Init() override;
		bool			CleanUp() override;
		update_status	Update() override;

		void			DrawHierarchy();
		GameObject*		CreateGameObject(const char* goName = nullptr, GameObject* goParent = nullptr, const math::float4x4& transform = math::float4x4().identity);
		GameObject*		CreateCamera(GameObject* goParent = nullptr, const math::float4x4& transform = math::float4x4().identity);
		void			LoadGeometry(GameObject* goParent, GeometryType geometryType);


		void			CreateGameObject(Config* config, rapidjson::Value& value);
		void			SaveScene();
		GameObject*		GetGameObjectByUUID(GameObject* gameObject, char uuidObjectName[37]);
		void			SaveGameObject(Config* config, GameObject* gameObject);
		void			LoadScene();
		void			ClearScene();

	public:
		GameObject*			root = nullptr;
		GameObject*			goSelected = nullptr;
		QuadTreeChimera*	quadTree = nullptr;

		int					scaleFactor = 1000;
		math::float3		lightPosition = math::float3(0.0f, 1.0 * scaleFactor, 1.0f * scaleFactor);
		float				ambientLight = 0.3f;
};

#endif
