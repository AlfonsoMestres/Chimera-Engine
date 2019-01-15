#include "DockQuad.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "QuadTreeChimera.h"

DockQuad::DockQuad() { }

DockQuad::~DockQuad() { }

void DockQuad::Draw() {

	if (!ImGui::Begin("QuadTree", &enabled)) {
		ImGui::End();
		return;
	}

	ImVec2 winSize = ImGui::GetWindowSize();

	ImGui::Checkbox("Draw", &App->renderer->showQuad);

	if (App->renderer->showQuad) {
		if (App->camera->quadCamera != nullptr) {
			ImGui::Image((ImTextureID)App->camera->quadCamera->renderTexture, { winSize.x, winSize.x }, { 0,1 }, { 1,0 });
		} 
	}

	ImGui::SliderInt("Min nodes", &maxNodes, 1, 5);
	if (ImGui::Button("Recalculate")) {
		App->scene->quadTree->InitQuadTree(App->scene->quadTree->quadLimits);
	}

	//if (App->camera->quadCamera != nullptr) {
	//	ImGui::SliderFloat3("Position", (float*)&App->camera->quadCamera->frustum.pos, -10000, 10000);
	//	ImGui::SliderFloat("Near", (float*)&App->camera->quadCamera->frustum.nearPlaneDistance, 1.0f, App->camera->quadCamera->frustum.farPlaneDistance);
	//	ImGui::SliderFloat("Near far", (float*)&App->camera->quadCamera->frustum.farPlaneDistance, App->camera->quadCamera->frustum.nearPlaneDistance, 10000.0f);
	//	ImGui::SliderFloat("Frustum W", (float*)&App->camera->quadCamera->frustum.orthographicWidth, 1.0f, 5000.0f);
	//	ImGui::SliderFloat("Frustum H", (float*)&App->camera->quadCamera->frustum.orthographicHeight, 1.0f, 5000.0f);
	//}

	focus = ImGui::IsWindowFocused();
	ImGui::End();
}

bool DockQuad::IsFocused() const { return focus; }
