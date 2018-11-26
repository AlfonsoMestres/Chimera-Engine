#ifndef __COMPONENTTMATERIAL_H__
#define __COMPONENTTMATERIAL_H__

#include "assert.h"
#include "GL/glew.h"
#include "Component.h"
#include "Component.h"
#include "Imgui/imgui.h"
#include "assimp/material.h"

class ComponentMaterial : public Component
{
	public:
		ComponentMaterial(GameObject* goContainer, const aiMaterial* material);
		~ComponentMaterial();

		void SetMaterial(const aiMaterial* material);
		void DeleteTexture();

	public:
		unsigned int shader = 0;
		Texture* texture = nullptr;

};

#endif
