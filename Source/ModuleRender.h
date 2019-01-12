#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "ImGuizmo/ImGuizmo.h"
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

		void			DrawImGuizmo(float width, float height, float winPosX, float winPosY);

	private:
		void			InitSDL();
		void			InitOpenGL() const;

		void			SetViewMatrix(ComponentCamera* camera) const;
		void			SetProjectionMatrix(ComponentCamera* camera) const;
		void			GenerateBlockUniforms();
		void			GenerateFallBackMaterial();

		/* Debug elements drawing */
		void			DrawDebugData(ComponentCamera* camera) const;
		void			PrintQuadNode(QuadTreeNode* quadNode) const;
		void			PrintRayCast() const;

		/* Mesh drawing */
		void			DrawMeshes(ComponentCamera* camera);
		void			DrawWithoutCulling(ComponentMesh* mesh) const;
		void			CullingFromQuadTree(ComponentCamera* camera, ComponentMesh* mesh);
		void			CullingFromFrustum(ComponentCamera* camera, ComponentMesh* mesh) const;

	public:
		bool			frustCulling = true;
		int				frustumCullingType = 0;
		bool			vsyncEnabled = false;
		void*			context = nullptr;
		unsigned		ubo = 0u;
		bool			showQuad = false;
		bool			showRayCast = false;
		unsigned		fallback = 0u;

		int				imGuizmoOp = 0;
		int				imGuizmoMode = 0;

		std::list<ComponentMesh*> meshes;
		std::vector<GameObject*> quadGOCollided;
};

#endif
