#include "Component.h"

Component::Component(GameObject* goContainer, ComponentType type) : goContainer(goContainer), componentType(type) { }

Component::~Component() { }

void Component::Update() { }
