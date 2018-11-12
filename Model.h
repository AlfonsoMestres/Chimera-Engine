#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include "MathGeoLib.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <list>
#include <vector>
#include "GL/glew.h"
#include <string>

struct Texture {
	int id;
	int width;
	int height;
	Texture(int id, int width, int height) : id(id), width(width), height(height) {}
};

class Model
{
	public:
		Model(const char* file);
		~Model();

		void					Draw() const;
		void					UpdateTexture(Texture texture);
		void					DrawInfo() const;


	private:
		bool					LoadModel(const char* pathFile);
		void					GenerateMeshData(const aiNode* node, const aiScene* scene);
		void					GenerateMaterialData(const aiScene* scene);
		void					GetAABB();

		const char*				path;
		std::list<Mesh>			meshes;
		std::vector<Texture>	textures;
		AABB					boundingBox = AABB({ 0,0,0 }, { 0,0,0 });
};

#endif //__MODEL_H__