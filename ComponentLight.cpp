#include "ComponentLight.h"

ComponentLight::ComponentLight(GameObject* goContainer) : Component(goContainer, ComponentType::LIGHT) { }

ComponentLight::~ComponentLight() { }
