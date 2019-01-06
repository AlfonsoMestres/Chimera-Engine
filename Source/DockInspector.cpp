#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "DockInspector.h"

DockInspector::DockInspector() { }

DockInspector::~DockInspector() { }

void DockInspector::Draw() {

	if (!ImGui::Begin("Inspector", &enabled)) {
		ImGui::End();
		return;
	}

	if (focus) {
		focus = false;
		ImGui::SetWindowFocus();
	}

	if (App->scene->goSelected != nullptr) {
		App->scene->goSelected->DrawProperties();

		if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowWidth(), 25))) {
			ImGui::OpenPopup("AddComponentPopup");
		}
		ImGui::SameLine();
		if (ImGui::BeginPopup("AddComponentPopup")) {
			ImGui::Text("Components");
			ImGui::Separator();
			if (ImGui::Selectable("Camera")) {
				App->scene->goSelected->AddComponent(ComponentType::CAMERA);
			}
			ImGui::Separator();
			if (ImGui::Selectable("Mesh")) {
				App->scene->goSelected->AddComponent(ComponentType::MESH);
			}
			if (ImGui::Selectable("Material")) {
				App->scene->goSelected->AddComponent(ComponentType::MATERIAL);
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void DockInspector::Focus(GameObject* gameobject) {
	focus = true;
}
