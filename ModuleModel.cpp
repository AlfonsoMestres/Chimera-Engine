#include "GL/glew.h"
#include "assert.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleScene.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ModuleModel.h"

ModuleModel::ModuleModel() { }

ModuleModel::~ModuleModel() { }

unsigned ModuleModel::Load(const char* path) {
	assert(path != nullptr);

	std::string fullFilePath(path);
	std::size_t found = fullFilePath.find_last_of("/");
	std::string name = fullFilePath.substr(found + 1, fullFilePath.length()).c_str();

	GameObject(name.c_str(), nullptr);

	models.emplace_back(path);

	LOG("Loaded Model: %s", path);

	return 1;
}

void ModuleModel::DrawModels() {

	for (auto const& model : models) {
		model.Draw();
	}

}

void ModuleModel::ApplyTexture(Texture texture) {

	for (auto& model : models) {
		model.UpdateTexture(texture);
	}

}

void ModuleModel::DeleteModels() {
	models.clear();
}

// TODO: fix this and migrate the load model function into node hierarchy
//GameObject* ModuleModel::ProcessNode(const aiNode * node, const aiScene * scene, const aiMatrix4x4 & parentTransform, GameObject* parent) {
//	assert(node != nullptr); 
//	assert(scene != nullptr);
//
//	aiMatrix4x4 transform = parentTransform * node->mTransformation;
//	GameObject * gameobject = App->scene->CreateGameObject(node->mName.C_Str(), parent);
//
//	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
//		ComponentMesh* mesh = (ComponentMesh*)gameobject->AddComponent(ComponentType::MESH);
//		mesh->ComputeMesh(scene->mMeshes[node->mMeshes[i]]);
//
//		ComponentMaterial* material = (ComponentMaterial*)gameobject->AddComponent(ComponentType::MATERIAL);
//		material->ComputeMaterial(scene->mMaterials[mesh->GetMaterialIndex()]);
//	}
//
//	for (unsigned int i = 0; i < node->mNumChildren; i++) {
//
//		GameObject * child = ProcessNode(node->mChildren[i], scene, transform, gameobject);
//	}
//
//	return gameobject;
//}

void ModuleModel::DrawGUI() {

	if (models.size() != 0) {

		if (ImGui::CollapsingHeader("Model")) {

			for (auto& model : models) {
				model.DrawInfo();
			}

		}

		if (ImGui::Checkbox("Checkers Texture", &checkersTexture)) {

			if (checkersTexture && checkTexture.id == 0) {
				checkTexture = App->textures->Load("checkersTexture.jpg");
			}

		}

	}

}
