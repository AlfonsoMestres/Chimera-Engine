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

	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();
	ImGui::SetCursorPos(ImVec2(-(App->window->width - size.x) * 0.5f, -(App->window->height - size.y) * 0.5f));

	viewport.x = ImGui::GetCursorPosX() + ImGui::GetWindowPos().x;
	viewport.y = ImGui::GetCursorPosY() + ImGui::GetWindowPos().y;

	ImGui::Image((ImTextureID)App->camera->sceneCamera->renderTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });

	App->renderer->DrawImGuizmo(size.x, size.y, pos.x, pos.y);

	focus = ImGui::IsWindowFocused();
	ImGui::End();

}

bool DockScene::IsFocused() const { return focus; }
