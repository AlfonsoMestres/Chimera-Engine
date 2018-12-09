#include "DockInspector.h"
#include "Application.h"
#include "ModuleScene.h"

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
		const char* components[] = { "Transform", "Mesh", "Material" };

		if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowWidth(), 25))) {
			ImGui::OpenPopup("AddComponentPopup");
		}
		ImGui::SameLine();
		if (ImGui::BeginPopup("AddComponentPopup")) {
			ImGui::Text("Components");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(components); i++) {
				if (ImGui::Selectable(components[i])) {
					App->scene->goSelected->AddComponent((ComponentType)i);
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void DockInspector::Focus(GameObject* gameobject) {
	focus = true;
}
