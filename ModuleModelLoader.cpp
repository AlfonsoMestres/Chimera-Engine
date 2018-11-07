#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleModelLoader.h"

ModuleModelLoader::ModuleModelLoader() { }

ModuleModelLoader::~ModuleModelLoader() { }

bool ModuleModelLoader::Init() {
	return LoadModel("BakerHouse.fbx");
}

bool ModuleModelLoader::LoadModel(const char* pathFile) {

	const aiScene* scene = aiImportFile(pathFile, NULL);

	if (scene) {
		GenerateMeshData(scene);
		GenerateMaterialData(scene);
	} else {
		LOG(aiGetErrorString());
	}

	return scene;
}

bool ModuleModelLoader::CleanUp() {

	for (unsigned i = 0; i < meshes.size(); ++i) {
		if (meshes[i].vbo != 0) {
			glDeleteBuffers(1, &meshes[i].vbo);
		}

		if (meshes[i].ibo != 0) {
			glDeleteBuffers(1, &meshes[i].ibo);
		}
	}

	for (unsigned i = 0; i < materials.size(); ++i) {
		if (materials[i].texture0 != 0) {
			App->textures->Unload(materials[i].texture0);
		}
	}

	return true;
}

void ModuleModelLoader::GenerateMeshData(const aiScene* scene) {

	for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
		const aiMesh* src_mesh = scene->mMeshes[i];

		Mesh dst_mesh;

		glGenBuffers(1, &dst_mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, dst_mesh.vbo);

		// Positions
		glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

		// Texture coords
		math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices, sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
		for (unsigned i = 0; i < src_mesh->mNumVertices; ++i)
		{
			texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Indices
		glGenBuffers(1, &dst_mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh.ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned i = 0; i < src_mesh->mNumFaces; ++i)
		{
			assert(src_mesh->mFaces[i].mNumIndices == 3);

			*(indices++) = src_mesh->mFaces[i].mIndices[0];
			*(indices++) = src_mesh->mFaces[i].mIndices[1];
			*(indices++) = src_mesh->mFaces[i].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		dst_mesh.material = src_mesh->mMaterialIndex;
		dst_mesh.num_vertices = src_mesh->mNumVertices;
		dst_mesh.num_indices = src_mesh->mNumFaces * 3;

		meshes.push_back(dst_mesh);
	}

}

void ModuleModelLoader::GenerateMaterialData(const aiScene* scene) {

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];

		Material dst_material;

		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			dst_material.texture0 = App->textures->Load(file.data);
		}

		materials.push_back(dst_material);
	}

}