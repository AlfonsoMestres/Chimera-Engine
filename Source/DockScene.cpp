#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ImGuizmo/ImGuizmo.h"
#include "DockScene.h"

DockScene::DockScene() { }

DockScene::~DockScene() { }

void DockScene::Draw() {
	ImGui::Begin("Scene", &enabled, ImGuiWindowFlags_NoScrollbar);

	winSize = ImGui::GetWindowSize();
	
	App->camera->sceneCamera->SetScreenNewScreenSize(winSize.x, winSize.y);

	ImVec2 pos = ImGui::GetWindowPos();

	viewport.x = ImGui::GetCursorPosX() + pos.x;
	viewport.y = ImGui::GetCursorPosY() + pos.y;

	ImGui::Image((ImTextureID)App->camera->sceneCamera->renderTexture, winSize, { 0,1 }, { 1,0 });
	App->renderer->DrawImGuizmo(winSize.x, winSize.y, pos.x, pos.y);

	App->camera->sceneFocused = ImGui::IsMouseHoveringWindow();

	focus = ImGui::IsWindowFocused();
	ImGui::End();

}

bool DockScene::IsFocused() const { return focus; }
