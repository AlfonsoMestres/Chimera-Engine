#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include <list>
#include <vector>

class GameObject;
class QuadTreeNode;
class ComponentMesh;
class ComponentCamera;

class ModuleRender : public Module
{
	public:
		ModuleRender();
		~ModuleRender();

		bool			Init() override;
		update_status	PreUpdate() override;
		update_status	Update() override;
		update_status	PostUpdate() override;
		bool			CleanUp();

	private:
		void			InitSDL();
		void			InitOpenGL() const;

		void			SetViewMatrix(ComponentCamera* camera) const;
		void			SetProjectionMatrix(ComponentCamera* camera) const;
		void			GenerateBlockUniforms();
		void			GenerateFallBackMaterial();

		void			DrawMeshes(ComponentCamera* camera);
		void			DrawDebugData(ComponentCamera* camera) const;
		void			PrintQuadNode(QuadTreeNode* quadNode) const;
		void			PrintRayCast() const;
		void			DrawWithoutCulling(ComponentMesh* mesh) const;
		void			CullingFromQuadTree(ComponentCamera* camera, ComponentMesh* mesh);
		void			CullingFromFrustum(ComponentCamera* camera, ComponentMesh* mesh) const;

	public:
		float			sceneViewportX = 0.0f;
		float			sceneViewportY = 0.0f;
		bool			frustCulling = true;
		int				frustumCullingType = 0;
		bool			vsyncEnabled = false;
		void*			context = nullptr;
		unsigned		ubo = 0u;
		bool			showQuad = false;
		bool			showRayCast = false;
		unsigned		fallback = 0u;
		std::list<ComponentMesh*> meshes;
		std::vector<GameObject*> quadGOCollided;
};

#endif
