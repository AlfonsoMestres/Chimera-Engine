#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "assert.h"
#include "Component.h"
#include "ModuleTextures.h"
#include "Geometry/AABB.h"
#include "Math/float3.h"
#include "Math/Quat.h"
#include <assimp/mesh.h>
#include <vector>
#include "GL/glew.h"

class ComponentMesh : public Component
{
	public:
		ComponentMesh(GameObject* goContainer, aiMesh* mesh);
		ComponentMesh(const ComponentMesh& duplicatedComponent);
		~ComponentMesh();

		void ComputeMesh(aiMesh* mesh);
		const unsigned MaterialIndex();

		void CleanUp();

		void		Draw(unsigned shaderProgram, const Texture* textures) const;
		void		DrawProperties() override;
		Component*	Duplicate() override;

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
