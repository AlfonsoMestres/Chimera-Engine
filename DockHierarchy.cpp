#include "DockHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

DockHierarchy::DockHierarchy() { }

DockHierarchy::~DockHierarchy() { }

void DockHierarchy::Draw() {

	if (!ImGui::Begin("Hierarchy", &enabled)) {
		ImGui::End();
		return;
	}
	App->scene->DrawHierarchy();
	focused = ImGui::IsWindowFocused();
	ImGui::End();

}