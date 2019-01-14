#include "DockQuad.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"
#include "ModuleRender.h"

DockQuad::DockQuad() { }

DockQuad::~DockQuad() { }

void DockQuad::Draw() {

	if (!ImGui::Begin("QuadTree", &enabled)) {
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Draw", &App->renderer->showQuad);

	if (App->renderer->showQuad) {
		if (App->camera->quadCamera != nullptr) {
			ImGui::Image((ImTextureID)App->camera->quadCamera->renderTexture, { ImGui::GetWindowWidth(), ImGui::GetWindowWidth() }, { 0,1 }, { 1,0 });
		} 
	}

	if (App->camera->quadCamera != nullptr) {
		ImGui::SliderFloat3("Frust FRONT", (float*)&App->camera->quadCamera->frustum.front, -10, 10);
		ImGui::SliderFloat3("Position", (float*)&App->camera->quadCamera->frustum.pos, -100.0f * App->scene->scaleFactor, 100.f * App->scene->scaleFactor);
		ImGui::SliderFloat("Near", (float*)&App->camera->quadCamera->frustum.nearPlaneDistance, 1.0f, App->camera->quadCamera->frustum.farPlaneDistance);
		ImGui::SliderFloat("Near", (float*)&App->camera->quadCamera->frustum.farPlaneDistance, App->camera->quadCamera->frustum.nearPlaneDistance, 10000.0f);
		ImGui::SliderFloat("Frustum W", (float*)&App->camera->quadCamera->frustum.orthographicWidth, 10.0f, 100000.0f);
		ImGui::SliderFloat("Frustum H", (float*)&App->camera->quadCamera->frustum.orthographicHeight, 10.0f, 100000.0f);
	}

	focus = ImGui::IsWindowFocused();
	ImGui::End();
}

bool DockQuad::IsFocused() const { return focus; }
