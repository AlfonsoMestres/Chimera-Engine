#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

struct Mesh;

class MeshImporter
{
	public:
		static void ImportFBX(const char* filePath);
		static bool Import(const aiMesh* aiMesh, const char* meshName);
		static bool Load(Mesh* mesh, const char* meshName);
		static bool Save(const Mesh& mesh, const char* meshName);
		static void CleanUpStructMesh(Mesh* mesh);
};

#endif
