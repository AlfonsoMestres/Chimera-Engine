#include "Application.h"
#include "ModuleScene.h"
#include "DockLight.h"

DockLight::DockLight() { }

DockLight::~DockLight() { }

void DockLight::Draw() {

	ImGui::Begin("Lights", &enabled, ImGuiWindowFlags_NoScrollbar);
	//TODO: this will display the available lights in the current scene

	ImGui::Text("Ambient");
	ImGui::Separator();
	ImGui::SliderFloat("Intensity", &App->scene->ambientLight, 0.0f, 1.0f);
	ImGui::Text("Position");
	ImGui::DragFloat3("", (float*)&App->scene->lightPosition, 0.25f, -500.f, 500.f);

	focus = ImGui::IsWindowFocused();
	ImGui::End();
}

bool DockLight::IsFocused() const { return focus; }
