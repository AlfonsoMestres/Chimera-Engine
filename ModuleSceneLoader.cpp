#include "Application.h"
#include "ModuleSceneLoader.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

ModuleSceneLoader::ModuleSceneLoader() { }

ModuleSceneLoader::~ModuleSceneLoader() { }

bool ModuleSceneLoader::Start() {
	LoadFile("Models/BakerHouse/BakerHouse.fbx");
	return true;
}


void ModuleSceneLoader::LoadFile(const char* path) {
	assert(path != nullptr);
	
	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate);

	if (scene != NULL) {
		filepath = path;
		LoadScene(scene);
	} else {
		LOG("ERROR importing file:%s \n", aiGetErrorString());
	}
}

void ModuleSceneLoader::LoadScene(const aiScene* scene) {
	assert(scene != nullptr);

	ProcessNode(scene->mRootNode, scene, aiMatrix4x4(), App->scene->root);

	aiReleaseImport(scene);
}

GameObject* ModuleSceneLoader::ProcessNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform, GameObject* parent) {
	assert(node != nullptr); 
	assert(scene != nullptr);

	//aiMatrix4x4 transform = parentTransform * node->mTransformation;
	//GameObject* gameobject = App->scene->CreateGameObject(transform, filepath, node->mName.C_Str(), parent);

	//for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	//	ComponentMesh* mesh = (ComponentMesh*)gameobject->CreateComponent(ComponentType::Mesh);
	//	mesh->SetMesh(scene->mMeshes[node->mMeshes[i]]);

	//	ComponentMaterial* material = (ComponentMaterial*)gameobject->CreateComponent(ComponentType::Material);
	//	material->SetMaterial(scene->mMaterials[mesh->GetMaterialIndex()]); 
	//}

	//for (unsigned int i = 0; i < node->mNumChildren; i++) {
	//	GameObject * child = ProcessNode(node->mChildren[i], scene, transform, gameobject);
	//}

	return nullptr;
}