#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include <vector>

#include "MathGeoLib.h"

#include "Module.h"
#include "GL/glew.h"
#include "SDL.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool	Init();
	bool    CleanUp();
	bool	LoadModel(const char* pathFile);

	struct Mesh
	{
		unsigned vbo = 0;
		unsigned ibo = 0;
		unsigned material = 0;
		unsigned num_vertices = 0;
		unsigned num_indices = 0;
	};

	struct Material
	{
		unsigned texture0 = 0;
	};

public:
	std::vector<Mesh>     meshes;
	std::vector<Material> materials;

private:
	void	GenerateMeshData(const aiScene* scene);
	void	GenerateMaterialData(const aiScene* scene);

};

#endif