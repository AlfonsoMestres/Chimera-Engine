#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* goContainer) : Component(goContainer, ComponentType::TRANSFORM) { }

ComponentTransform::~ComponentTransform() { }
