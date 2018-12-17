#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ModuleScene.h"

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
			std::string childName = "ChildOf";
			childName += goParent->name;

			gameObject = new GameObject(childName.c_str(), transform, goParent, fileLocation);
		} else {
			gameObject = new GameObject(DEFAULT_GO_NAME, transform, goParent, fileLocation);
		}

	}

	return gameObject;
}

//GameObject* ModuleScene::GenerateSphere(GameObject* goParent, int slices, int stacks, const math::float3& position,
//										const math::Quat& rotation, const math::float3& size, const math::float4& color) {
//	assert(slices >= 1);
//	assert(stacks >= 1);
//
//	//TODO: first multiple cameras
//	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(slices, stacks);
//
//	if (mesh == nullptr) {
//		LOG("Error: Sphere mesh invalid.");
//		return nullptr;
//	}
//
//	GameObject* sphere = new GameObject("Sphere", float4x4::FromTRS(position, rotation, size), goParent, nullptr);
//
//	par_shapes_scale(mesh, size.x, size.y, size.z);
//
//	ComponentMesh* sphereMesh = (ComponentMesh*)sphere->AddComponent(ComponentType::MESH);
//	sphereMesh->ComputeMesh(mesh);
//
//	par_shapes_free_mesh(mesh);
//
//	ComponentMaterial* sphereMaterial = (ComponentMaterial*)sphere->AddComponent(ComponentType::MATERIAL);
//	sphereMaterial->SetTexture(App->textures->defaultTexture);
//
//	return nullptr;
//}

//GameObject* ModuleScene::CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation, 
//										float height, float radius, unsigned slices, unsigned stacks, const math::float4& color) {
//	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
//	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);
//
//	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
//	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);
//
//	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
//	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);
//
//	if (mesh == nullptr) {
//		LOG("Error: Cylinder couldn't be created. Par_shapes returned nullptr.");
//		return nullptr;
//	}
//
//	par_shapes_scale(mesh, radius, height, radius);
//	par_shapes_merge_and_free(mesh, top);
//	par_shapes_merge_and_free(mesh, bottom);
//
//	GameObject* cylinder = App->scene->CreateGameObject(name, math::float4x4::FromTRS(position, rotation, math::float3::one));
//
//	ComponentMesh* cylinder_mesh = (ComponentMesh*)cylinder->CreateComponent(component_type::Mesh);
//	cylinder_mesh->GenerateMesh(mesh);
//
//	par_shapes_free_mesh(mesh);
//
//	ComponentMaterial* cylinder_material = (ComponentMaterial*)cylinder->CreateComponent(component_type::Material);
//	cylinder_material->texture = App->textures->loadTexture("checkersTexture.jpg.jpg");
//
//	return cylinder;
//}
