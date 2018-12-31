#include "Application.h"
#include "ModuleSceneLoader.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

ModuleSceneLoader::ModuleSceneLoader() { }

ModuleSceneLoader::~ModuleSceneLoader() { }

bool ModuleSceneLoader::Init() {
	// TODO: Here we will call the basic scene read to be loaded
	App->textures->LoadDefaulTextures();
	LoadFile("Models/BakerHouse/BakerHouse.fbx");
	return true;
}

void ModuleSceneLoader::LoadFile(const char* path) {
	assert(path != nullptr);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	 
	if (scene != NULL) {
		filepath = path;

		GameObject* rootGO = App->scene->CreateGameObject(scene->mRootNode->mName.C_Str(), App->scene->root, (math::float4x4&)aiMatrix4x4(), filepath);

		ProcessTree(scene->mRootNode, scene, aiMatrix4x4(), rootGO);
		aiReleaseImport(scene);

		rootGO->ComputeBBox();
	} else {
		LOG("ERROR importing file: %s \n", aiGetErrorString());
	}
}

void ModuleSceneLoader::ProcessTree(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform, GameObject* goParent) {
	assert(scene != nullptr);
	assert(node != nullptr);
	assert(goParent != nullptr);
	 
	aiMatrix4x4 transform = parentTransform * node->mTransformation;

	for (unsigned i = 0u; i < node->mNumMeshes; ++i) {

		GameObject* meshGO = App->scene->CreateGameObject(node->mName.C_Str(), goParent, (math::float4x4&)transform, filepath);
		ComponentMesh* mesh = (ComponentMesh*)meshGO->AddComponent(ComponentType::MESH);
		mesh->name = scene->mMeshes[node->mMeshes[i]]->mName.C_Str();
		mesh->ComputeMesh(scene->mMeshes[node->mMeshes[i]]);

		ComponentMaterial* material = (ComponentMaterial*)meshGO->AddComponent(ComponentType::MATERIAL);
		material->ComputeMaterial(scene->mMaterials[mesh->MaterialIndex()]);
	}

	for (unsigned i = 0u; i < node->mNumChildren; i++) {
		// Generate child
		ProcessTree(node->mChildren[i], scene, transform, goParent);
	}

}
