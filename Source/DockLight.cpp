#include "Component.h"
#include "DockLight.h"
#include "Application.h"
#include "ModuleScene.h"

DockLight::DockLight() { }

DockLight::~DockLight() { }

void DockLight::Draw() {

	ImGui::Begin("Lights", &enabled, ImGuiWindowFlags_NoScrollbar);
	//TODO: this will display the available lights in the current scene

	ImGui::Text("Ambient");
	ImGui::Separator();
	ImGui::SliderFloat("Intensity", &App->scene->ambientLight, 0.0f, 1.0f);
	ImGui::Text("Position");
	ImGui::DragFloat3("", (float*)&App->scene->lightPosition, 10.0f, -50000.f, 50000.f);

	focus = ImGui::IsWindowFocused();
	ImGui::End();
}

bool DockLight::IsFocused() const { return focus; }
