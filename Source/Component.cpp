#include "Application.h"
#include "Component.h"
#include "ModuleResourceManager.h"

Component::Component(GameObject* gameObject, ComponentType type) {
	uuid = App->resource->NewGuuid();
	parentUuid = gameObject->uuid;
	goContainer = gameObject;
	componentType = type;
}

Component::Component(const Component& duplicateComponent) {
	uuid = App->resource->NewGuuid();
	parentUuid = duplicateComponent.parentUuid;
	goContainer = duplicateComponent.goContainer;
	componentType = duplicateComponent.componentType;
	enabled = duplicateComponent.enabled;
}

Component::~Component() { }

void Component::Update() { }

bool Component::DrawComponentState() {
	ImGui::Checkbox("Active", &enabled); ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.8f, 0.8f));

	bool removed = ImGui::SmallButton("Remove Component");

	ImGui::Text("UUID: "); ImGui::SameLine();
	ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, uuid.c_str());
	/*ImGui::Text("Go UUID: "); ImGui::SameLine();
	ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, parentUuid.c_str());*/

	if (removed) Remove();

	ImGui::PopStyleColor(3);
	return removed;
}

void Component::Remove() {
	goContainer->RemoveComponent(this);
}