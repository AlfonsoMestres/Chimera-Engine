#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Mesh.h"
#include "assert.h"
#include "Component.h"

class ComponentMesh : public Component
{
	public:
		ComponentMesh(GameObject* goContainer);
		~ComponentMesh();

		void ComputeMesh(aiMesh* mesh);
		void CleanUp();
		void Draw(unsigned shaderProgram, const std::vector<Texture>& textures) const;

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

#endif
