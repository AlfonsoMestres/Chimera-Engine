#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include <list>
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "GL/glew.h"
#include "IL/ilut.h"
#include "Math/float4.h"
#include "Geometry/AABB.h"

enum class MaterialType;
class ComponentMaterial;

struct Texture {
	int id = 0;
	int width = 0;
	int height = 0;
	Texture(int id, int width, int height) : id(id), width(width), height(height) { }
};

struct Mesh {
	unsigned vao = 0u;
	unsigned vbo = 0u;
	unsigned ibo = 0u;

	float*		normals = nullptr;
	float*		colors = nullptr;
	float*		uvs = nullptr;
	unsigned	verticesNumber = 0u;
	float*		vertices = nullptr;
	unsigned	indicesNumber = 0u;
	unsigned*	indices = nullptr;

	unsigned normalsOffset = 0u;
	unsigned texturesOffset = 0u;
	unsigned vertexSize = 0u;

	math::AABB	bbox;
};

struct Material {
	unsigned		occlusionMap = 0u;
	float			ambientK = 0.5f;
	int				ambientWidth = 0;
	int				ambientHeight = 0;

	unsigned		diffuseMap = 0u;
	math::float4	diffuseColor = math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	float			diffuseK = 1.0f;
	int				diffuseWidth = 0;
	int				diffuseHeight = 0;

	unsigned		specularMap = 0u;
	math::float4	specularColor = math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	float			specularK = 0.6f;
	float			shininess = 64.0f;
	int				specularWidth = 0;
	int				specularHeight = 0;

	unsigned		emissiveMap = 0u;
	math::float4	emissiveColor = math::float4(0.0f, 0.0f, 0.0f, 0.0f);
	int				emissiveWidth = 0;
	int				emissiveHeight = 0;

	math::float4	color = math::float4::one;
};

class ModuleTextures : public Module
{
	public:
		ModuleTextures();
		~ModuleTextures();

		bool			Init() override;
		bool			CleanUp() override;
		void			DrawGUI();

		void			LoadMaterial(std::string path, unsigned& textureID, int& width, int& height);
		void			LoadMaterial(const char* path, ComponentMaterial* componentMaterial, MaterialType materialTypeSelected);
		void			Unload(unsigned id);

	private:
		void			LoadDefaulTextures();
		Texture* const  Load(const char* path);

	public:
		bool			mipmaping = false;
		int				filterType = GL_LINEAR;
		int				mipMapMode = GL_NEAREST_MIPMAP_NEAREST;
		int				wrapMode = GL_CLAMP;
		Texture*		noCameraSelectedTexture = nullptr;

};		   

#endif