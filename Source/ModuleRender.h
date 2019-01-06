#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"

class QuadTreeNode;
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
		void			DrawDebugData(ComponentCamera* camera) const;
		void			PrintQuadNode(QuadTreeNode* quadNode) const;

	public:
		bool			cullingFromGameCamera = true;
		bool			vsyncEnabled = false;
		void*			context = nullptr;
		unsigned		ubo = 0u;
		bool			showQuad = false;

};

#endif
