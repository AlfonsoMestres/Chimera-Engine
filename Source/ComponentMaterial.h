#ifndef __COMPONENTTMATERIAL_H__
#define __COMPONENTTMATERIAL_H__

#include "assert.h"
#include "GL/glew.h"
#include "Component.h"
#include "Component.h"
#include "Imgui/imgui.h"
#include "Math/float4.h"
#include "ModuleTextures.h"
#include "assimp/material.h"

enum class MaterialType {
	NO_TYPE_SELECTED = 0,
	OCCLUSION_MAP,
	DIFFUSE_MAP,
	SPECULAR_MAP,
	EMISSIVE_MAP
};

class ComponentMaterial : public Component
{
	public:
		ComponentMaterial(GameObject* goContainer);
		ComponentMaterial(GameObject* goContainer, const aiMaterial* material);
		ComponentMaterial(const ComponentMaterial& duplicatedComponent);
		~ComponentMaterial();

		void		UnloadMaterial();

		void		DrawProperties() override;
		Component*	Duplicate() override;

	public:
		Material	material;

	private:
		void		DeleteTexture(unsigned id);
		void		DrawComboBoxMaterials(const char* id, MaterialType matType, static std::string& labelCurrentFileTextureSelected);

	private:
		std::string diffuseSelected;
		std::string occlusionSelected;
		std::string specularSelected;
		std::string emissiveSelected;
};

#endif
