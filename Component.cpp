#include "Component.h"

Component::Component(GameObject* goContainer, ComponentType type) {
	this->goContainer = goContainer;
	this->componentType = type;
}

Component::~Component() { }

void Component::Update() { }

void Component::Remove() {
	this->goContainer->RemoveComponent(this);
}