#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject* goContainer) : Component(goContainer, ComponentType::MESH) { }

ComponentMesh::~ComponentMesh() { }
