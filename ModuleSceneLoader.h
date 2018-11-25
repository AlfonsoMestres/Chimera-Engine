#ifndef __MODULESCENELOADER_H__
#define __MODULESCENELOADER_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/matrix4x4.h"
#include "Math/float3.h"
#include "Math/float4.h"
#include "Math/Quat.h"
#include "GL/glew.h"

struct aiNode;
struct aiScene;
class GameObject;

class ModuleSceneLoader : public Module
{
	public:
		ModuleSceneLoader();
		~ModuleSceneLoader();

		bool Start();
		void LoadFile(const char* path);
		void LoadScene(const aiScene* scene);
		GameObject* ProcessNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4 &parentTransform, GameObject* parent);

	public:
		//bool checkers = false;
		//Texture checkersTexture = Texture(0,0,0);
		const char *filepath = nullptr;
};

#endif