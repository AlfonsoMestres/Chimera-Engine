#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow() { }

ModuleWindow::~ModuleWindow() { }

bool ModuleWindow::Init() {

	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("Error: SDL_VIDEO could not be initialized. %s\n", SDL_GetError());
		ret = false;
	} else {
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (RESIZEABLE == true) {
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (BORDERLESS == true) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (FULLSCREEN_DESKTOP == true) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window != nullptr) {
			screen_surface = SDL_GetWindowSurface(window);
		} else {
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp() {
	LOG("Destroying SDL window and quitting all SDL systems");

	if(window != nullptr) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
	return true;
}

void ModuleWindow::WindowResized(unsigned newWidth, unsigned newHeight) {
	width = newWidth;
	height = newHeight;
	App->camera->sceneCamera->SetScreenNewScreenSize(newWidth, newHeight);
}

void ModuleWindow::DrawGUI() {

	if (ImGui::Checkbox("FullScreen", &fullscreen)) {

		if (fullscreen) {
			SDL_DisplayMode displayMode;
			SDL_GetDesktopDisplayMode(0, &displayMode);
			SDL_SetWindowSize(App->window->window, displayMode.w, displayMode.h);
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		} else {
			SDL_SetWindowFullscreen(App->window->window, 0);
		}
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable)) {
		if (resizable) {
			SDL_SetWindowResizable(App->window->window, SDL_TRUE);
		} else {
			SDL_SetWindowResizable(App->window->window, SDL_FALSE);
		}
	}

	ImGui::NewLine();
	if (ImGui::Checkbox("Borderless", &borderless)) {
		if (borderless) {
			SDL_SetWindowBordered(App->window->window, SDL_FALSE);
		} else {
			SDL_SetWindowBordered(App->window->window, SDL_TRUE);
		}
	}

}
