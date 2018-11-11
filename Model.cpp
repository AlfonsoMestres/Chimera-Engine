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
	App->camera->selectedObjectBB = BoundingBox;
	App->camera->FocusSelectedObject();
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
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
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

void Model::DrawTexture() {

	if (ImGui::CollapsingHeader("Textures")) {

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

	if (meshes.size() == 0 || meshes.front().vertices.size() == 0) {
		return;
	}

	math::float3 minV; 
	math::float3 maxV;
	minV = maxV = meshes.front().vertices[0];
	for (auto &mesh : meshes) {

		for (auto &vertice : mesh.vertices) {

			minV.x = min(minV.x, vertice.x);
			minV.y = min(minV.y, vertice.y);
			minV.z = min(minV.z, vertice.z);
			maxV.x = max(maxV.x, vertice.x);
			maxV.y = max(maxV.y, vertice.y);
			maxV.z = max(maxV.z, vertice.z);

		}
	}

	//centerModel = math::float3((minV.x + maxV.x) * 0.5, (minV.y + maxV.y) * 0.5, (minV.z + maxV.z) * 0.5);
	//boundingBoxSize = BoundingBox.Size();

	BoundingBox.minPoint = minV;
	BoundingBox.maxPoint = maxV;
}