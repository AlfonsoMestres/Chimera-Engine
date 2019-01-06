#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "DockCamera.h"

DockCamera::DockCamera() { }

DockCamera::~DockCamera() { }

void DockCamera::Draw() {

	ImGui::Begin("Game", &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(-(App->window->width - size.x) * 0.5f, -(App->window->height - size.y) * 0.5f));
	if (App->camera->selectedCamera != nullptr && App->camera->selectedCamera->enabled == true) {
		ImGui::Image((ImTextureID)App->camera->selectedCamera->renderTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	} else {
		//TODO: this should be a unique Material, not an old Texture
		ImGui::Image((ImTextureID)App->textures->noCameraSelectedTexture->id, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	}
	ImGui::End();
}

bool DockCamera::IsFocused() const { return focus; }
