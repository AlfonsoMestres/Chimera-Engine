#include "Model.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleProgram.h"
#include "imgui.h"

Model::Model(const char* file) {
	std::string s(file);
	std::size_t found = s.find_last_of("/\\");
	s = s.substr(0, found + 1);
	this->path = s.c_str();
	LoadModel(file);
	// Updating the focused object
	App->camera->selectedObjectBB = boundingBox;
	//App->camera->FocusSelectedObject();
}

Model::~Model() { }

bool Model::LoadModel(const char* pathFile) {
	assert(pathFile != nullptr);

	const aiScene* scene = aiImportFile(pathFile, { aiProcess_Triangulate | aiProcess_GenUVCoords });

	if (scene) {
		GenerateMeshData(scene->mRootNode, scene);
		GenerateMaterialData(scene);
		GetAABB();
	} else {
		LOG("Error: %s", aiGetErrorString());
	}

	return scene;
}

void Model::GenerateMeshData(const aiNode* node, const aiScene* scene) {
	assert(scene != nullptr);

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		GenerateMeshData(node->mChildren[i], scene);
	}

}

void Model::Draw() const {

	for (auto &mesh : meshes) {
		mesh.Draw(App->program->textureProgram, textures);
	}

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

	if (ImGui::CollapsingHeader("Texture")) {

		for (auto &texture : textures) {
			ImGui::Text("Size:  Width: %d | Height: %d", texture.width, texture.height);
			float size = ImGui::GetWindowWidth();
			ImGui::Image((ImTextureID)texture.id, { size,size });
		}
	}

}


void Model::UpdateTexture(Texture texture) {

	for (auto &Oldtexture : textures) {
		Oldtexture = texture;
	}

}

void Model::GenerateMaterialData(const aiScene* scene) {
	assert(scene != nullptr);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i) {

		const aiMaterial* materialSrc = scene->mMaterials[i];

		aiString file;
		aiTextureMapping mapping = aiTextureMapping_UV;

		if (materialSrc->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, 0) == AI_SUCCESS) {
			std::string pathFile(this->path);
			pathFile += file.C_Str();
			textures.push_back(App->textures->Load(pathFile.c_str()));
		} else {
			LOG("Error: Could not load the %fth material", i);
		}

	}

}

// Axis Aligned Bounding Box
void Model::GetAABB() {

	for (auto& mesh : meshes) {
		boundingBox.Enclose(mesh.bbox);
	}

	boundingBox.FaceCenterPoint(5);

}
