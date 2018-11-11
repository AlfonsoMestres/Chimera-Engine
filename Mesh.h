#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"
#include "MathGeoLib.h"
#include <assimp/mesh.h>
#include <assert.h>
#include <vector>
#include "GL/glew.h"

struct Texture;

class Mesh
{
	public:
		Mesh(aiMesh* mesh);
		~Mesh();

		void						Draw(unsigned shaderProgram, const std::vector<Texture>& textures) const;

	public:
		int							numIndices = 0;
		int							materialIndex = 0;
		std::vector<math::float3>	vertices;

	private:
		unsigned vao;
		unsigned vbo;
		unsigned ibo;
};

#endif //__MESH_H__
