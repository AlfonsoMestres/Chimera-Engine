#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include <vector>
#include <assimp/mesh.h>
#include "Component.h"
#include "Math/float3.h"
#include "ModuleTextures.h"

struct par_shapes_mesh_s;
class ComponentMaterial;
class GameObject;

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
		void		DrawProperties(bool enabled) override;
		void		LoadMesh(const char* name);
		Component*	Duplicate() override;

	private:
		void Save(Config* config) override;
		void Load(Config* config, rapidjson::Value& value) override;

	public:
		std::vector<std::string> fileMeshesList;
		Mesh			mesh;
		const char*		name = nullptr;

	private:
		std::string currentMesh;
};

#endif
