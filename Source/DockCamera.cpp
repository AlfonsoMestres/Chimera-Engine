#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "DockCamera.h"

DockCamera::DockCamera() { }

DockCamera::~DockCamera() { }

void DockCamera::Draw() {
	ImGui::Begin("Game", &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImVec2 size = ImGui::GetWindowSize();

	if (App->camera->selectedCamera != nullptr) {
		App->camera->selectedCamera->SetScreenNewScreenSize(size.x, size.y);
	}

	if (App->camera->selectedCamera != nullptr && App->camera->selectedCamera->enabled == true) {
		ImGui::Image((ImTextureID)App->camera->selectedCamera->renderTexture, size, { 0,1 }, { 1,0 });
	} else {
		ImGui::Image((ImTextureID)App->textures->noCameraSelectedTexture->id, size, { 0,1 }, { 1,0 });
	}
	ImGui::End();
}

bool DockCamera::IsFocused() const { return focus; }
