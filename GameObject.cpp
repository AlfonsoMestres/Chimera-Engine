#include "Application.h"
#include "ModuleScene.h"
#include "ModuleProgram.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

/// CARE Creating this without father could lead to memory leak
GameObject::GameObject() { }

GameObject::GameObject(const char* goName, const aiMatrix4x4& transform, const char* fileLocation) {

	name = goName;

	if (fileLocation != nullptr) {
		filePath = fileLocation;
	}

	this->parent = App->scene->root;
	this->transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	this->transform->AddTransform(transform);
	App->scene->root->goChilds.push_back(this);
}

GameObject::GameObject(const char* goName, const aiMatrix4x4& transform, GameObject* goParent, const char* fileLocation) {

	name = goName;

	if (goParent != nullptr) {
		this->parent = goParent;
		goParent->goChilds.push_back(this);
	} else {
		this->parent = App->scene->root;
		App->scene->root->goChilds.push_back(this);
	}

	this->transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	this->transform->AddTransform(transform);

	if (fileLocation != nullptr) {
		filePath = fileLocation;
	}
}

// TODO: this is not being called
GameObject::~GameObject() {

	for (auto &component : components) {
		delete component;
		component = nullptr;
	}
	components.clear();

	for (auto &child : goChilds) {
		delete child;
		child = nullptr;
	}

	delete transform;
	transform = nullptr;
	delete parent;
	parent = nullptr;
	delete name;
	name = nullptr;
}

void GameObject::Update() {
	for (const auto &child : goChilds) {
		child->Update();
	}

}

void GameObject::Draw() const{


	for (const auto &child : goChilds) {
		child->Draw();
	}

	if (!enabled) return;
	if (transform == nullptr) return;

	ComponentMaterial* material = (ComponentMaterial*)GetComponent(ComponentType::MATERIAL);
	unsigned shader = 0u;
	Texture* texture = nullptr;

	if (material != nullptr && material->enabled) {
		shader = material->GetShader();
		texture = material->GetTexture();
	} else {
		shader = App->program->textureProgram;
	}

	glUseProgram(shader);
	ModelTransform(shader);

	std::vector<Component*> meshes = GetComponents(ComponentType::MESH);
	for (const auto &mesh : meshes) {
		if (mesh->enabled) {
			((ComponentMesh*)mesh)->Draw(shader, texture);
		}
	}

	glUseProgram(0);
}


void GameObject::DrawProperties() const {
	assert(name != nullptr);

	ImGui::InputText("Name", (char*)name, sizeof(name));

	for (auto &component : components) {
		component->DrawProperties();
	}
}

void GameObject::DrawHierarchy(GameObject* goSelected) {
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (goSelected == this ? ImGuiTreeNodeFlags_Selected : 0);

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
			component = new ComponentMaterial(this);
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

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it){
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

std::vector<Component*> GameObject::GetComponents(ComponentType type) const {
	std::vector<Component*> list;
	for (auto &component : components) {
		if (component->componentType == type) {
			list.push_back(component);
		}
	}

	return list;
}

math::float4x4 GameObject::GetLocalTransform() const {
	if (transform == nullptr) {
		return float4x4::identity;
	}

	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}

math::float4x4 GameObject::GetGlobalTransform() const {
	if (parent != nullptr) {
		return parent->GetGlobalTransform() * GetLocalTransform();
	}

	return GetLocalTransform();
}

void GameObject::ModelTransform(unsigned shader) const {
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_TRUE, &GetGlobalTransform()[0][0]);
}

AABB& GameObject::ComputeBBox() const {
	bbox.SetNegativeInfinity();

	// Current GO meshes
	for (const auto &mesh : GetComponents(ComponentType::MESH)) {
		bbox.Enclose(((ComponentMesh *)mesh)->bbox);
	}

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	// Child meshes
	for (const auto &child : goChilds){
		if (child->GetComponents(ComponentType::MESH).size() > 0) {
			bbox.Enclose(child->ComputeBBox());
		}
	}

	return bbox;
}