#include "MeshImporter.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFileSystem.h"

void MeshImporter::ImportFBX(const char* filePath) {
	bool result = false;

	char* fileBuffer;
	unsigned lenghBuffer = App->fileSystem->Load(filePath, &fileBuffer);
	const aiScene* scene = aiImportFileFromMemory(fileBuffer, lenghBuffer, aiProcessPreset_TargetRealtime_MaxQuality, "");

	std::string fileName;
	App->fileSystem->SplitFilePath(filePath, nullptr, &fileName, nullptr);

	if (scene != nullptr && scene->mMeshes != nullptr) {
		for (int i = 0; i < scene->mNumMeshes; i++) {
			std::string meshName = fileName;
			meshName.append("_" + std::to_string(i));
			Import(scene->mMeshes[i], meshName.c_str());
		}
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
}

bool MeshImporter::Import(const aiMesh* aiMesh, const char* meshName) {
	bool result = false;

	if (aiMesh == nullptr) {
		LOG("Error: FBX failed to import");
		return result;
	}

	Mesh meshStruct;

	meshStruct.verticesNumber = aiMesh->mNumVertices;
	meshStruct.vertices = new float[meshStruct.verticesNumber * 3];
	memcpy(meshStruct.vertices, aiMesh->mVertices, sizeof(float) * meshStruct.verticesNumber * 3);

	if (aiMesh->HasFaces()) {
		meshStruct.indicesNumber = aiMesh->mNumFaces * 3;
		meshStruct.indices = new unsigned[meshStruct.indicesNumber];
		for (unsigned i = 0u; i < aiMesh->mNumFaces; ++i) {
			memcpy(&meshStruct.indices[i * 3], aiMesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
		}
	}

	if (aiMesh->HasTextureCoords(0)) {
		meshStruct.uvs = new float[meshStruct.verticesNumber * 2];
		int uvsCount = 0;
		for (unsigned i = 0u; i < meshStruct.verticesNumber; i++) {
			meshStruct.uvs[uvsCount] = aiMesh->mTextureCoords[0][i].x;
			uvsCount++;
			meshStruct.uvs[uvsCount] = aiMesh->mTextureCoords[0][i].y;
			uvsCount++;
		}
	}

	if (aiMesh->HasNormals()) {
		meshStruct.normals = new float[meshStruct.verticesNumber * 3];
		memcpy(meshStruct.normals, aiMesh->mNormals, sizeof(float) * meshStruct.verticesNumber * 3);
	}

	if (aiMesh->HasVertexColors(0)) {
		meshStruct.colors = new float[meshStruct.verticesNumber * 3];
		memcpy(meshStruct.colors, aiMesh->mColors, sizeof(float) * meshStruct.verticesNumber * 3);
	}

	meshStruct.bbox.SetNegativeInfinity();
	meshStruct.bbox.Enclose((math::float3*)aiMesh->mVertices, aiMesh->mNumVertices);

	return Save(meshStruct, meshName);
}

bool MeshImporter::Load(Mesh* meshStruct, const char* meshName) {
	bool result = false;

	if (meshStruct == nullptr) {
		LOG("Error: FBX failed to load");
		return result;
	}

	char* buffer;
	std::string meshPath("/Library/Meshes/");
	meshPath.append(meshName);
	unsigned size = 0u;
	size = App->fileSystem->Load(meshPath.c_str(), &buffer);

	if (buffer != nullptr && size > 0) {
		char* cursor = buffer;

		// indices / vertices / uvs / normals/ colors / 
		unsigned ranges[5];
		unsigned bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		meshStruct->indicesNumber = ranges[0];
		meshStruct->verticesNumber = ranges[1];

		// indices
		bytes = sizeof(unsigned) * meshStruct->indicesNumber;
		meshStruct->indices = new unsigned[meshStruct->indicesNumber];
		memcpy(meshStruct->indices, cursor, bytes);
		cursor += bytes;

		// vertices
		bytes = sizeof(float) * meshStruct->verticesNumber * 3;
		meshStruct->vertices = new float[meshStruct->verticesNumber * 3];
		memcpy(meshStruct->vertices, cursor, bytes);
		cursor += bytes;

		if (ranges[2] > 0) {
			bytes = sizeof(float)* meshStruct->verticesNumber * 2;
			meshStruct->uvs = new float[meshStruct->verticesNumber * 2];
			memcpy(meshStruct->uvs, cursor, bytes);
			cursor += bytes;
		}

		if (ranges[3] > 0) {
			bytes = sizeof(float)* meshStruct->verticesNumber * 3;
			meshStruct->normals = new float[meshStruct->verticesNumber * 3];
			memcpy(meshStruct->normals, cursor, bytes);
			cursor += bytes;
		}

		if (ranges[4] > 0) {
			bytes = sizeof(float)* meshStruct->verticesNumber * 3;
			meshStruct->colors = new float[meshStruct->verticesNumber * 3];
			memcpy(meshStruct->colors, cursor, bytes);
			cursor += bytes;
		}

		delete[] cursor;
		cursor = nullptr;

		result = true;
	}

	delete[] buffer;
	buffer = nullptr;

	return result;
}


bool MeshImporter::Save(const Mesh& mesh, const char* meshName) {
	bool result = false;

	// indices / vertices / uvs / normals/ colors / 
	unsigned ranges[5] = {
		mesh.indicesNumber,
		mesh.verticesNumber,
		(mesh.uvs) ? mesh.verticesNumber : 0u,
		(mesh.normals) ? mesh.verticesNumber : 0u,
		(mesh.colors) ? mesh.verticesNumber : 0u
	};

	unsigned size = sizeof(ranges);
	size += sizeof(unsigned) * mesh.indicesNumber;
	size += sizeof(float) * mesh.verticesNumber * 3;
	if (mesh.uvs) {
		size += sizeof(float) * mesh.verticesNumber * 2;
	}
	if (mesh.normals) {
		size += sizeof(float) * mesh.verticesNumber * 3;
	}
	if (mesh.colors) {
		size += sizeof(float) * mesh.verticesNumber * 3;
	}

	char* data = new char[size];
	char* cursor = data;

	unsigned bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(unsigned) * mesh.indicesNumber;
	memcpy(cursor, mesh.indices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * mesh.verticesNumber * 3;
	memcpy(cursor, mesh.vertices, bytes);
	cursor += bytes;

	if (mesh.uvs != nullptr) {
		bytes = sizeof(float)* mesh.verticesNumber * 2;
		memcpy(cursor, mesh.uvs, bytes);
		cursor += bytes;
	}

	if (mesh.normals != nullptr) {
		bytes = sizeof(float)* mesh.verticesNumber * 3;
		memcpy(cursor, mesh.normals, bytes);
		cursor += bytes;
	}

	if (mesh.colors != nullptr) {
		bytes = sizeof(float)* mesh.verticesNumber * 3;
		memcpy(cursor, mesh.colors, bytes);
		cursor += bytes;
	}

	std::string fileToSave("/Library/Meshes/");

	fileToSave.append(meshName);
	fileToSave.append(".head");

	result = App->fileSystem->Save(fileToSave.c_str(), data, size, false);

	delete[] data;
	data = nullptr;

	return result;
}

void MeshImporter::CleanUpStructMesh(Mesh* mesh) {
	if (mesh != nullptr) {
		if (mesh->indices != nullptr) {
			delete[] mesh->indices;
			mesh->indices = nullptr;
		}

		if (mesh->vertices != nullptr) {
			delete[] mesh->vertices;
			mesh->vertices = nullptr;
		}

		if (mesh->uvs != nullptr) {
			delete[] mesh->uvs;
			mesh->uvs = nullptr;
		}

		if (mesh->normals != nullptr) {
			delete[] mesh->normals;
			mesh->normals = nullptr;
		}

		if (mesh->colors != nullptr) {
			delete[] mesh->colors;
			mesh->colors = nullptr;
		}

	}
}