#include "Component.h"

Component::Component(GameObject* gameObject, ComponentType type) {
	goContainer = gameObject;
	componentType = type;
}

Component::~Component() { }

void Component::Update() { }

bool Component::DrawComponentState() {
	ImGui::Checkbox("Active", &enabled); ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.8f, 0.8f));

	bool removed = ImGui::SmallButton("Remove Component");
	if (removed) Remove();

	ImGui::PopStyleColor(3);
	return removed;
}

void Component::Remove() {
	goContainer->RemoveComponent(this);
}