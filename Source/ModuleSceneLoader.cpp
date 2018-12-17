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
	App->textures->LoadDefaulTexture("checkersTexture.jpg");
	LoadFile("Models/BakerHouse/BakerHouse.fbx");
	return true;
}

void ModuleSceneLoader::LoadFile(const char* path) {
	assert(path != nullptr);

	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate);

	if (scene != NULL) {
		filepath = path;
		ProcessTree(scene->mRootNode, scene, aiMatrix4x4(), App->scene->root);
		aiReleaseImport(scene);
	} else {
		LOG("ERROR importing file: %s \n", aiGetErrorString());
	}
}

void ModuleSceneLoader::ProcessTree(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform, GameObject* goParent) {
	assert(scene != nullptr);
	assert(node != nullptr);
	assert(goParent != nullptr);

	aiMatrix4x4 transform = parentTransform * node->mTransformation;
	GameObject* gameObject = App->scene->CreateGameObject(node->mName.C_Str(), goParent, (math::float4x4&)transform, filepath);

	for (unsigned i = 0u; i < node->mNumMeshes; i++) {
		ComponentMesh* mesh = (ComponentMesh*)gameObject->AddComponent(ComponentType::MESH);
		mesh->name = scene->mMeshes[node->mMeshes[i]]->mName.C_Str();
		mesh->ComputeMesh(scene->mMeshes[node->mMeshes[i]]);

		ComponentMaterial* material = (ComponentMaterial*)gameObject->AddComponent(ComponentType::MATERIAL);
		material->ComputeMaterial(scene->mMaterials[mesh->MaterialIndex()]);
	}

	for (unsigned i = 0u; i < node->mNumChildren; i++) {
		// Generate child
		ProcessTree(node->mChildren[i], scene, transform, gameObject);
	}

}
