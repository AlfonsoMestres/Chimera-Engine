#include "Application.h"
#include "ModuleScene.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

/// CARE Creating this without father could lead to memory leak
GameObject::GameObject() { }

GameObject::GameObject(const char* goName, const aiMatrix4x4& transform) {

	if (goName != nullptr) {
		this->name = goName;
	} else {
		this->name = "GameObject";
	}

	this->parent = App->scene->root;
	this->transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	this->transform->AddTransform(transform);
	App->scene->root->goChilds.push_back(this);
}

GameObject::GameObject(const char* goName, const aiMatrix4x4& transform, GameObject* goParent) {

	if (goName != nullptr) {
		this->name = goName;
	} else {
		this->name = "GameObject";
	}

	if (goParent != nullptr) {
		this->parent = goParent;
		goParent->goChilds.push_back(this);
	} else {
		this->parent = App->scene->root;
		App->scene->root->goChilds.push_back(this);
	}
}


GameObject::~GameObject() {

	for (std::list<GameObject*>::reverse_iterator it = goChilds.rbegin(); it != goChilds.rend(); ++it) {
		delete *it;
	}
	goChilds.clear();

	for (std::list<Component*>::reverse_iterator it = components.rbegin(); it != components.rend(); ++it) {
		delete *it;
	}
	components.clear();

}

void GameObject::Update() {
	for (const auto &child : goChilds) {
		child->Update();
	}

}

void GameObject::Draw() {
	for (const auto &child : goChilds) {
		child->Draw();
	}
	
	LOG("Drawing GO %s", name);
}

void GameObject::DrawHierarchy(GameObject* goSelected) {
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | goSelected == this ? ImGuiTreeNodeFlags_Selected : NULL;

	ImGui::PushID(this);
	if (goChilds.empty()) {
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool obj_open = ImGui::TreeNodeEx(this, node_flags, name);

	if (ImGui::IsItemClicked()) {
		App->scene->goSelected = this;
	}

	if (obj_open) {
		for (auto &child : goChilds){
			child->DrawHierarchy(goSelected);
		}

		if (!(node_flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}

std::string GameObject::GetFileFolder() const {
	std::string s(filePath);
	std::size_t found = s.find_last_of("/\\");
	s = s.substr(0, found + 1);
	return s;
}

Component* GameObject::AddComponent(ComponentType type) {
	Component* component = nullptr;

	switch (type) {
		case ComponentType::CAMERA:
			break;
		case ComponentType::TRANSFORM:
			component = new ComponentTransform(this, aiMatrix4x4());
			transform = (ComponentTransform*)component;
			break;
		case  ComponentType::MESH:
			component = new ComponentMesh(this, nullptr);
			break;
		case ComponentType::MATERIAL:
			component = new ComponentMaterial(this, nullptr);
			break;
		case ComponentType::EMPTY:
		default:
			break;
	}

	components.push_back(component);
	return component;
}

void GameObject::RemoveComponent(Component* component) {
	assert(component != nullptr);

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it){
		if ((*it) == component){
			components.erase(it);
			delete component;
			component = nullptr;
			return;
		}
	}
}

Component* GameObject::GetComponent(ComponentType type) const {

	for (auto &component : components) {
		if (component->componentType == type) {
			return component;
		}
	}

	return nullptr;
}
