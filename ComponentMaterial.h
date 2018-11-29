#ifndef __COMPONENTTMATERIAL_H__
#define __COMPONENTTMATERIAL_H__

#include "assert.h"
#include "GL/glew.h"
#include "Component.h"
#include "Component.h"
#include "Imgui/imgui.h"
#include "ModuleTextures.h"
#include "assimp/material.h"

class ComponentMaterial : public Component
{
	public:
		ComponentMaterial(GameObject* goContainer);
		ComponentMaterial(GameObject* goContainer, const aiMaterial* material);
		~ComponentMaterial();

		void ComputeMaterial(const aiMaterial* material);
		void DeleteTexture();

	public:
		unsigned shader = 0u;
		Texture* texture = nullptr;

};

#endif
