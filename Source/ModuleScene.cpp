#include <list>
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

ModuleScene::ModuleScene() { }

ModuleScene::~ModuleScene() { }

bool ModuleScene::Init() {
	root = new GameObject();
	root->name = "root";

	return true;
}

update_status ModuleScene::Update() {
	update_status ret = UPDATE_CONTINUE;

	root->Update();

	return ret;
}

void ModuleScene::Draw(const math::Frustum& frustum) const {
	root->Draw(frustum);
}

void ModuleScene::DrawHierarchy() {
	for (auto &child : root->goChilds) {
		child->DrawHierarchy(goSelected);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* goName, GameObject* goParent, const math::float4x4& transform, const char* fileLocation) {

	GameObject* gameObject = nullptr;

	if (goName != nullptr) {		
		gameObject = new GameObject(goName, transform, goParent, fileLocation);
	} else {

		if (goParent != nullptr) {
			std::string childNameStr = "ChildOf";
			childNameStr += goParent->name;

			gameObject = new GameObject(childNameStr.c_str(), transform, goParent, fileLocation);
		} else {
			gameObject = new GameObject(DEFAULT_GO_NAME, transform, goParent, fileLocation);
		}

	}

	return gameObject;
}

GameObject* ModuleScene::CreateCamera(GameObject* goParent, const math::float4x4& transform) {
	GameObject* gameObject = nullptr;

	gameObject = new GameObject(DEFAULT_CAMERA_NAME, transform, goParent, nullptr);
	ComponentTransform* goTrans = (ComponentTransform*)gameObject->GetComponent(ComponentType::TRANSFORM);
	goTrans->SetPosition(math::float3(0.0f, 2.5f, 10.0f));
	gameObject->AddComponent(ComponentType::CAMERA);

	return gameObject;
}

void ModuleScene::LoadGeometry(GameObject* goParent, GeometryType geometryType) {
	par_shapes_mesh_s* parMesh = nullptr;

	switch (geometryType) {
		case GeometryType::SPHERE:
			parMesh = par_shapes_create_parametric_sphere(30, 30);
			break;
		case GeometryType::TORUS:
			parMesh = par_shapes_create_torus(30, 30, 0.5f);
			break;
		case GeometryType::PLANE:
			parMesh = par_shapes_create_plane(30, 30);
			break;
		case GeometryType::CUBE:
			parMesh = par_shapes_create_cube();
			break;
	}

	if (parMesh != nullptr) {
		par_shapes_scale(parMesh, 5.0f, 5.0f, 5.0f);

		ComponentMesh* mesh = (ComponentMesh*)goParent->AddComponent(ComponentType::MESH);
		mesh->ComputeMesh(parMesh);
		par_shapes_free_mesh(parMesh);

		ComponentMaterial* mat = (ComponentMaterial*)goParent->AddComponent(ComponentType::MATERIAL);
		goParent->ComputeBBox();
	} else {
		LOG("Error: error loading par_shapes mesh");
	}
}
