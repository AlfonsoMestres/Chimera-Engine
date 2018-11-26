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

		void						Draw(unsigned shaderProgram, const Texture* texture) const;

	public:
		const char*					name = nullptr;
		int							numIndices = 0;
		int							materialIndex = 0;
		std::vector<math::float3>	vertices;
		AABB						bbox;

	private:
		unsigned vao = 0u;
		unsigned vbo = 0u;
		unsigned ibo = 0u;
};

#endif //__MESH_H__
