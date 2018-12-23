#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"

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
		void			DrawDebugData(ComponentCamera* camera) const;
		void			GenerateBlockUniforms();

	public:
		bool			vsyncEnabled = false;
		void*			context = nullptr;
		unsigned		ubo = 0u;

	protected:
		bool   showAxis = true;
		bool   showGrid = true;
};

#endif
