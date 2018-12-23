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

class ComponentMaterial : public Component
{
	public:
		ComponentMaterial(GameObject* goContainer);
		ComponentMaterial(GameObject* goContainer, const aiMaterial* material);
		ComponentMaterial(const ComponentMaterial& duplicatedComponent);
		~ComponentMaterial();

		void ComputeMaterial(const aiMaterial* material);
		void DeleteTexture();

		void		DrawProperties() override;
		Component*	Duplicate() override;

	public:
		unsigned		shader = 0u;
		math::float4	color = math::float4::one;
		Texture*		texture = nullptr;
};

#endif
