#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "ModuleModel.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "SDL.h"
#include "GL/glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
	public:
		ModuleRender();
		~ModuleRender();

		bool			Init() override;
		update_status	PreUpdate() override;
		update_status	Update() override;
		update_status	PostUpdate() override;
		void			DrawGUI();

		void			InitFrustum();
		void			InitSDL();
		void			InitOpenGL();
		void			ViewMatrix(unsigned programUsed);
		void			ProjectionMatrix(unsigned programUsed);
		void			ModelTransform(unsigned programUsed);
		void			LookAt(math::float3& cameraPos, math::float3& target);
		void			SetScreenNewScreenSize();
		void			DrawDebugData();

	private:
		void			DrawReferenceDebug();
		void			CreateFrameBuffer();

	public:
		float			bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Frustum			frustum;
		math::float4x4	viewMatrix;
		bool			vsyncEnabled = false;
		void*			context = nullptr;
		unsigned		fbo = 0u;
		unsigned		rbo = 0u;
		unsigned		renderTexture = 0u;

	protected:
		bool   showAxis = true;
		bool   showGrid = true;
};

#endif
