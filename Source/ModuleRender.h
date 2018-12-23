#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "MathGeoLib.h"
#include "SDL.h"
#include "GL/glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class ComponentCamera;

class ModuleRender : public Module
{
	public:
		ModuleRender();
		~ModuleRender();

		bool			Init() override;
		bool			Start();
		update_status	PreUpdate() override;
		update_status	Update() override;
		update_status	PostUpdate() override;
		bool			CleanUp();

		void			InitSDL();
		void			InitOpenGL() const;
		void			SetViewMatrix(ComponentCamera* camera) const;
		void			SetProjectionMatrix(ComponentCamera* camera) const;
		void			GenerateBlockUniforms();

	private:
		void			DrawDebugData(ComponentCamera* camera);

	public:
		bool			vsyncEnabled = false;
		void*			context = nullptr;

		unsigned		ubo = 0u;

	protected:
		bool   showAxis = true;
		bool   showGrid = true;
};

#endif
