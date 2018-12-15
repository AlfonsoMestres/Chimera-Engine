#include "DockHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_mouse.h"
#include "GameObject.h"

DockHierarchy::DockHierarchy() { }

DockHierarchy::~DockHierarchy() { }

void DockHierarchy::Draw() {

	if (!ImGui::Begin("Hierarchy", &enabled)) {
		ImGui::End();
		return;
	}

	// If empty heriarchy, allow to create one with a imgui popup
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && ImGui::IsWindowHovered()) {
		ImGui::OpenPopup("Modify_GameObject");
	}

	if (ImGui::BeginPopup("Modify_GameObject")) {
		if (ImGui::Selectable("Add Empty GameObject")) {
			App->scene->CreateGameObject();
		}
		ImGui::EndPopup();
	}

	App->scene->DrawHierarchy();
	focused = ImGui::IsWindowFocused();
	ImGui::End();

}