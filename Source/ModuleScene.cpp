#include <list>
#include "Config.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "QuadTreeChimera.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

ModuleScene::ModuleScene() { }

ModuleScene::~ModuleScene() { 
	CleanUp();
}

bool ModuleScene::CleanUp() {
	delete root;
	root = nullptr;
	delete quadTree;
	quadTree = nullptr;

	return true;
}

bool ModuleScene::Init() {
	root = new GameObject("root", nullptr);
	quadTree = new QuadTreeChimera();

	return true;
}

update_status ModuleScene::Update() {
	BROFILER_CATEGORY("SceneUpdate()", Profiler::Color::DarkRed);
	update_status ret = UPDATE_CONTINUE;

	root->Update();

	return ret;
}

void ModuleScene::DrawHierarchy() {
	for (auto &child : root->goChilds) {
		child->DrawHierarchy(goSelected);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* goName, GameObject* goParent, const math::float4x4& transform) {

	GameObject* gameObject = nullptr;

	if (goName != nullptr) {		
		gameObject = new GameObject(goName, transform, goParent);
	} else {

		if (goParent != nullptr) {
			std::string childNameStr = "ChildOf";
			childNameStr += goParent->name;

			gameObject = new GameObject(childNameStr.c_str(), transform, goParent);
		} else {
			gameObject = new GameObject(DEFAULT_GO_NAME, transform, goParent);
		}

	}

	return gameObject;
}

GameObject* ModuleScene::CreateCamera(GameObject* goParent, const math::float4x4& transform) {
	GameObject* gameObject = nullptr;

	gameObject = new GameObject(DEFAULT_CAMERA_NAME, transform, goParent);
	ComponentTransform* goTrans = (ComponentTransform*)gameObject->GetComponent(ComponentType::TRANSFORM);
	goTrans->SetPosition(math::float3(0.0f, 250.0f, 1000.0f));
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
		par_shapes_scale(parMesh, 500.0f, 500.0f, 500.0f);

		ComponentMesh* mesh = (ComponentMesh*)goParent->AddComponent(ComponentType::MESH);
		mesh->ComputeMesh(parMesh);
		par_shapes_free_mesh(parMesh);

		ComponentMaterial* mat = (ComponentMaterial*)goParent->AddComponent(ComponentType::MATERIAL);
		goParent->ComputeBBox();
	} else {
		LOG("Error: error loading par_shapes mesh");
	}
}

GameObject* ModuleScene::GetGameObjectByUUID(GameObject* gameObject, char uuidObjectName[37]) {
	GameObject* result = nullptr;

	if (result == nullptr && (strcmp(gameObject->uuid, uuidObjectName) == 0)) {
		result = gameObject;
	} else {
		for (auto &child : gameObject->goChilds) {
			if (child->goChilds.size() > 0) {
				result = GetGameObjectByUUID(child, uuidObjectName);
			}

			if (result == nullptr && (strcmp(child->uuid, uuidObjectName) == 0)) {
				result = child;
				break;
			} else if (result != nullptr) {
				break;
			}
		}
	}

	return result;
}

/* RapidJson storage  */
void ModuleScene::CreateGameObject(Config* config, rapidjson::Value& value) {
	if (value.HasMember("parentUuid")) {
		const char* parentUuid = config->GetString("parentUuid", value);
		char uuidGameObjectParent[37];
		sprintf_s(uuidGameObjectParent, parentUuid);

		GameObject* parent = GetGameObjectByUUID(root, uuidGameObjectParent);

		GameObject* gameObject = new GameObject(config->GetString("name", value), parent);
		gameObject->Load(config, value);
	} else {
		root->Load(config, value);
	}

}

void ModuleScene::SaveScene() {
	Config* config = new Config();
	config->StartObject("scene");

	config->AddFloat("ambientLight", ambientLight);
	config->AddFloat3("ambientLightPosition", lightPosition);
	config->AddBool("quadTreeEnabled", App->renderer->showQuad);

	config->EndObject();

	config->AddName("sceneCamera");
	App->camera->sceneCamera->Save(config);

	if (App->camera->selectedCamera != nullptr) {
		config->AddName("selectedCamera");
		App->camera->selectedCamera->Save(config);
	}

	config->StartArray("gameObjects");
	SaveGameObject(config, root);
	config->EndArray();

	config->WriteToDisk();

	delete config;
	config = nullptr;
}

void ModuleScene::SaveGameObject(Config* config, GameObject* gameObject) {
	gameObject->Save(config);

	if (gameObject->goChilds.size() > 0) {
		for (std::list<GameObject*>::iterator iterator = gameObject->goChilds.begin(); iterator != gameObject->goChilds.end(); ++iterator) {
			SaveGameObject(config, (*iterator));
		}
	}
}

void ModuleScene::LoadScene() {
	if (root->goChilds.size() >  0) {
		//TODO: If current scene is not saved, prompt 'are you sure? Do you want to save actual scene before loading?'
		ClearScene();
	}

	Config* config = new Config();

	rapidjson::Document document = config->LoadFromDisk();

	if (!document.HasParseError()) {
		rapidjson::Value& scene = document["scene"];

		ambientLight = config->GetFloat("ambientLight", scene);
		lightPosition = config->GetFloat3("ambientLightPosition", scene);

		App->camera->sceneCamera->Load(config, document["sceneCamera"]);

		rapidjson::Value gameObjects = document["gameObjects"].GetArray();
		for (rapidjson::Value::ValueIterator it = gameObjects.Begin(); it != gameObjects.End(); ++it) {
			CreateGameObject(config, *it);
		}

		App->renderer->showQuad = config->GetBool("quadTreeEnabled", scene);

		if (document.HasMember("selectedCamera")) {
			rapidjson::Value& selectedCamera = document["selectedCamera"];
			const char* parentUuid = config->GetString("goContainer", selectedCamera);
			char uuidGameObjectParent[37];
			sprintf_s(uuidGameObjectParent, parentUuid);
			GameObject* gameObjecteCameraSelected = GetGameObjectByUUID(root, uuidGameObjectParent);
			App->camera->selectedCamera = (ComponentCamera*)gameObjecteCameraSelected->GetComponent(ComponentType::CAMERA);
		}
	}
}

void ModuleScene::ClearScene() {
	CleanUp();
	Init();
}
