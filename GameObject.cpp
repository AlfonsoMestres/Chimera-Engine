#include "Application.h"
#include "ModuleScene.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"

GameObject::GameObject() {
	this->name = "GameObject";
	this->parent = App->scene->root;
}

GameObject::GameObject(const char* goName) {
	assert(goName != nullptr);

	if (goName != nullptr) {
		this->name = goName;
	} else {
		this->name = "GameObject";
	}

	this->parent = App->scene->root;
}

GameObject::GameObject(const char* goName, GameObject* goParent) {
	assert(goName != nullptr);
	assert(goParent != nullptr);

	if (goName != nullptr) {
		this->name = goName;
	} else {
		this->name = "GameObject";
	}

	if (goParent != nullptr) {
		this->parent = goParent;
	} else {
		this->parent = App->scene->root;
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

}

void GameObject::Draw() {
	for (const auto &child : goChilds) {
		child->Draw();
	}
	
	LOG("Drawing GO %s", name);
}

Component* GameObject::AddComponent(ComponentType type){
	Component* component = nullptr;

	switch (type) {
		case ComponentType::CAMERA:
			break;
		case ComponentType::TRANSFORM:
			component = new ComponentTransform(this);
			transform = (ComponentTransform*)component;
			break;
		case  ComponentType::MESH:
			component = new ComponentMesh(this);
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

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it){
		if ((*it) == component){
			components.erase(it);
			delete component;
			component = nullptr;
			return;
		}
	}
}
