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
		unsigned occlusionMap = 0u; //ambient
		float ambientK = 0.5f;
		int ambientWidth = 0;
		int ambientHeight = 0;

		unsigned diffuseMap = 0u;
		math::float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		float diffuseK = 1.0f;
		int diffuseWidth = 0;
		int diffuseHeight = 0;

		unsigned specularMap = 0u;
		math::float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		float specularK = 0.6f;
		float shininess = 64.0f;
		int specularWidth = 0;
		int specularHeight = 0;

		unsigned emissiveMap = 0u;
		math::float4 emissiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		int emissiveWidth = 0;
		int emissiveHeight = 0;

		int width = 0;
		int height = 0;
		unsigned		shader = 0u;
		math::float4	color = math::float4::one;
		Texture*		texture = nullptr;
};

#endif
