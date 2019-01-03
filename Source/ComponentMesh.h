#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include <vector>
#include <assimp/mesh.h>
#include "Component.h"
#include "Math/float3.h"
#include "ModuleTextures.h"

struct par_shapes_mesh_s;
class ComponentMaterial;

class ComponentMesh : public Component
{
	public:
		ComponentMesh(GameObject* goContainer, Mesh* mesh);
		ComponentMesh(const ComponentMesh& duplicatedComponent);
		~ComponentMesh();

		void CleanUp();

		void		ComputeMesh();
		void		ComputeMesh(par_shapes_mesh_s* parMesh);
		void		Draw(unsigned shaderProgram, const ComponentMaterial* material) const;
		void		DrawProperties() override;
		void		LoadMesh(const char* name);
		Component*	Duplicate() override;

	public:
		Mesh			mesh;
		const char*		name = nullptr;

	private:
		std::string currentMesh;
};

#endif
