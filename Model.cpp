#include "Model.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "ModuleScene.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "imgui.h"

Model::Model(const char* file) {
	std::string s(file);
	std::size_t found = s.find_last_of("/\\");
	s = s.substr(0, found + 1);
	this->path = s.c_str();

	LoadModel(file);
}

Model::~Model() { }

bool Model::LoadModel(const char* pathFile) {
	assert(pathFile != nullptr);

	std::string fullFilePath(pathFile);

	std::size_t found = fullFilePath.find("\\");
	while (found != std::string::npos) {
		fullFilePath.replace(found, std::string("\\").length(), "/");
		found = fullFilePath.find("\\");
	}

	found = fullFilePath.find_last_of("/");
	std::string name = fullFilePath.substr(found + 1, fullFilePath.length());
	name = name.substr(0, name.length() - 4);

	const aiScene* scene = aiImportFile(pathFile, { aiProcess_Triangulate | aiProcess_GenUVCoords });

	if (scene) {
		ProcessTree(scene->mRootNode, scene, aiMatrix4x4(), App->scene->root);
		GetModelBoundingBox();

		// We dont want to send a transform, so indentity 4x4
		GameObject* go = App->scene->CreateGameObject(name.c_str(), App->scene->root);
		App->camera->goSelected = go;

	} else {
		LOG("Error: Model failed to be imported %s", aiGetErrorString());
	}

	return scene;
}

GameObject* Model::ProcessTree(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform, GameObject* goParent) {
	assert(scene != nullptr);
	assert(node != nullptr);
	assert(goParent != nullptr);

	aiMatrix4x4 transform = parentTransform * node->mTransformation;
	GameObject* gameObject = App->scene->CreateGameObject(node->mName.C_Str(), goParent, transform);

	for (unsigned i = 0; i < node->mNumMeshes; i++) {
		ComponentMesh* mesh = (ComponentMesh*)gameObject->AddComponent(ComponentType::MESH);
		mesh->ComputeMesh(scene->mMeshes[node->mMeshes[i]]);

		ComponentMaterial* material = (ComponentMaterial*)gameObject->AddComponent(ComponentType::MATERIAL);
		material->ComputeMaterial(scene->mMaterials[mesh->MaterialIndex()]);
	}

	for (unsigned i = 0; i < node->mNumChildren; i++) {
		GameObject* child = ProcessTree(node->mChildren[i], scene, transform, gameObject);
	}

	return gameObject;

}

void Model::DrawInfo() const {

	// TODO: this is weird, change collapsing to other imgui element
	if (ImGui::CollapsingHeader("Meshes loaded")) {

		for (auto& meshSelected : meshes) {

			if (ImGui::CollapsingHeader(meshSelected.name, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {

				if (&meshSelected != nullptr) {

					ImGui::Text("Triangles Count: %d", meshSelected.numIndices / 3);
					ImGui::Text("Vertices Count: %d", meshSelected.vertices.size());
					ImGui::Text("Mesh size:\n X: %f | Y: %f | Z: %f", meshSelected.bbox.Size().x, meshSelected.bbox.Size().y, meshSelected.bbox.Size().z);

				} else {
					ImGui::Text("No mesh attached");
				}

			}
		}
	}

}


void Model::GetModelBoundingBox() {

	for (auto& mesh : meshes) {
		boundingBox.Enclose(mesh.bbox);
	}

}
