#include <list>
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentMesh.h"
#include "ModuleProgram.h"
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

void ModuleScene::Draw() {
	root->Draw();
}

void ModuleScene::DrawHierarchy() {
	for (auto &child : root->goChilds) {
		child->DrawHierarchy(goSelected);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* goName, GameObject* goParent, const math::float4x4& transform, const char* fileLocation) {

	GameObject* gameObject = nullptr;

	if (goName != nullptr) {

		char* go_name = new char[strlen(goName)];
		strcpy(go_name, goName);
		
		gameObject = new GameObject(go_name, transform, goParent, fileLocation);

	} else {

		if (goParent != nullptr) {
			std::string childNameStr = "ChildOf";
			childNameStr += goParent->name;
			char* childName = new char[strlen(childNameStr.c_str())];
			strcpy(childName, childNameStr.c_str());

			gameObject = new GameObject(childName, transform, goParent, fileLocation);
		} else {
			char* goName = new char[strlen(DEFAULT_GO_NAME)];
			strcpy(goName, DEFAULT_GO_NAME);

			gameObject = new GameObject(goName, transform, goParent, fileLocation);
		}

	}

	return gameObject;
}

GameObject* ModuleScene::CreateCamera(GameObject* goParent, const math::float4x4& transform) {
	GameObject* gameObject = nullptr;

	char* cameraName = new char[strlen(DEFAULT_CAMERA_NAME)];
	strcpy(cameraName, DEFAULT_CAMERA_NAME);

	gameObject = new GameObject(cameraName, transform, goParent, nullptr);
	gameObject->AddComponent(ComponentType::CAMERA);

	return gameObject;
}

GameObject* ModuleScene::GenerateSphere(GameObject* goParent, int slices, int stacks, const math::float3& pos,
										const math::Quat& rot, const float size, const math::float4& color) {

	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(slices, stacks);

	if (mesh) {
		GameObject* sphere = new GameObject("Sphere", math::float4x4::identity, goParent, nullptr);
		sphere->transform->SetRotation(rot);
		sphere->transform->SetPosition(pos);

		par_shapes_scale(mesh, size, size, size);

		ComponentMesh* sphereMesh = (ComponentMesh*)sphere->AddComponent(ComponentType::MESH);
		sphereMesh->ComputeMesh(mesh);

		par_shapes_free_mesh(mesh);

		ComponentMaterial* sphereMaterial = (ComponentMaterial*)sphere->AddComponent(ComponentType::MATERIAL);
		sphereMaterial->shader = App->program->basicProgram;
		sphereMaterial->color = color;

		goSelected = sphere;

		return sphere;
	}

	LOG("Error: par_shape_mesh sphere error");
	return nullptr;
}

//TODO: Fix this ... thing. Not rendering correctly the meshes
GameObject* ModuleScene::GenerateCylinder(GameObject* goParent, const math::float3& pos, const math::Quat& rot,
										float height, float radius, unsigned slices, unsigned stacks, const math::float4& color) {

	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);

	if (mesh == nullptr || top == nullptr || bottom == nullptr) {
		LOG("Error: par_shape_mesh cylinder error");
		return nullptr;
	}

	GameObject* cylinder = new GameObject("Cylinder", math::float4x4::identity, goParent, nullptr);
	cylinder->transform->SetRotation(rot);
	cylinder->transform->SetPosition(pos);

	par_shapes_scale(mesh, radius, height, radius);
	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);

	ComponentMesh* cylinderMesh = (ComponentMesh*)cylinder->AddComponent(ComponentType::MESH);
	cylinderMesh->ComputeMesh(mesh);

	par_shapes_free_mesh(mesh);

	ComponentMaterial* cylinderMaterial = (ComponentMaterial*)cylinder->AddComponent(ComponentType::MATERIAL);
	cylinderMaterial->shader = App->program->basicProgram;
	cylinderMaterial->color = color;

	return cylinder;
}
