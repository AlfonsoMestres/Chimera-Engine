#include "GameObject.h"

GameObject::GameObject(const char* goName) {
	assert(goName != nullptr);

	this->name = goName;
}

GameObject::~GameObject() { }

void GameObject::Update() {

}

Component* GameObject::CreateComponent(componentType type){

	return nullptr;
}
