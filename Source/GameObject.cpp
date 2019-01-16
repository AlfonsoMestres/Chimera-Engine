#include "Config.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "ComponentCamera.h"
#include "QuadTreeChimera.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ModuleFileSystem.h"
#include "SDL/include/SDL_mouse.h"
#include "debugdraw.h"

#define MAXNAME 64

GameObject::GameObject() {
	sprintf_s(uuid, App->fileSystem->NewGuuid().str().c_str());
	transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
}

GameObject::GameObject(std::string goName, GameObject* goParent) {
	sprintf_s(uuid, App->fileSystem->NewGuuid().str().c_str());

	char* newName = new char[MAXNAME];
	strcpy(newName, goName.c_str());
	name = newName;
	delete[] newName;

	if (goParent != nullptr) {
		parent = goParent;
		sprintf_s(parentUuid, goParent->uuid);
		goParent->goChilds.push_back(this);
	} 
}

GameObject::GameObject(std::string goName, const math::float4x4& parentTransform) {
	sprintf_s(uuid, App->fileSystem->NewGuuid().str().c_str());

	char* newName = new char[MAXNAME];
	strcpy(newName, goName.c_str());
	name = newName;
	delete[] newName;

	parent = App->scene->root;
	sprintf_s(parentUuid, App->scene->root->uuid);
	transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	transform->AddTransform(parentTransform);
	App->scene->root->goChilds.push_back(this);
}

GameObject::GameObject(std::string goName, const math::float4x4& parentTransform, GameObject* goParent) {
	sprintf_s(uuid, App->fileSystem->NewGuuid().str().c_str());

	char* newName = new char[MAXNAME];
	strcpy(newName, goName.c_str());
	name = newName;
	delete[] newName;

	if (goParent != nullptr) {
		parent = goParent;
		sprintf_s(parentUuid, goParent->uuid);
		goParent->goChilds.push_back(this);
	} else {
		parent = App->scene->root;
		sprintf_s(parentUuid, App->scene->root->uuid);
		App->scene->root->goChilds.push_back(this);
	}

	transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	transform->AddTransform(parentTransform);
}

GameObject::GameObject(const GameObject& duplicateGameObject) {
	sprintf_s(uuid, App->fileSystem->NewGuuid().str().c_str());
	sprintf_s(parentUuid, duplicateGameObject.parentUuid);

	char* newName = new char[MAXNAME];
	strcpy(newName, duplicateGameObject.name.c_str());
	name = newName;
	delete[] newName;

	staticGo = duplicateGameObject.staticGo;
	if (staticGo) {
		UpdateStaticChilds(staticGo);
	}
	bbox = duplicateGameObject.bbox;

	for (const auto &component : duplicateGameObject.components) {
		Component* duplicatedComponent = component->Duplicate();
		components.push_back(duplicatedComponent);
		duplicatedComponent->goContainer = this;

		sprintf_s(duplicatedComponent->parentUuid, uuid);
		if (duplicatedComponent->componentType == ComponentType::TRANSFORM) {
			transform = (ComponentTransform*)duplicatedComponent;
		}
		if (duplicatedComponent->componentType == ComponentType::MESH) {
			mesh = (ComponentMesh*)duplicatedComponent;
		}
		if (duplicatedComponent->componentType == ComponentType::MATERIAL) {
			material = (ComponentMaterial*)duplicatedComponent;
		}
	}

	for (const auto &child : duplicateGameObject.goChilds) {
		GameObject* duplicatedChild = new GameObject(*child);
		duplicatedChild->parent = this;
		sprintf_s(duplicatedChild->parentUuid, uuid);
		goChilds.push_back(duplicatedChild);
	}
}

GameObject::~GameObject() {

	if (staticGo) {
		App->scene->quadTree->Remove(this);
	}

	for (auto &component : components) {
		delete component;
		component = nullptr;
	}
	components.clear();

	for (auto &child : goChilds) {
		delete child;
		child = nullptr;
	}
	goChilds.clear();	

	parent = nullptr;
	transform = nullptr;
	mesh = nullptr;
	material = nullptr;
}

void GameObject::Update() {

	if (!enabled) return;

	for (std::list<GameObject*>::iterator itChild = goChilds.begin(); itChild != goChilds.end();) {

		(*itChild)->Update();

		if ((*itChild)->moveGOUp) {
			(*itChild)->moveGOUp = false;
			if (std::abs(std::distance(goChilds.begin(), itChild)) != 0) {
				std::swap(*itChild, *std::prev(itChild));
			}
		}

		if ((*itChild)->moveGODown) {
			(*itChild)->moveGODown = false;
			if (std::abs(std::distance(goChilds.begin(), itChild)) != goChilds.size() - 1) {
				std::swap(*itChild, *std::next(itChild));
			}
		}

		if ((*itChild)->toBeCopied) {
			(*itChild)->toBeCopied = false;
			GameObject* goCopied = new GameObject(**itChild);
			goCopied->parent = this;
			goChilds.push_back(goCopied);
			LOG("Duplicated GO: %s", (*itChild)->name);
		}

		if ((*itChild)->toBeDeleted) {
			(*itChild)->toBeDeleted = false;
			(*itChild)->CleanUp();
			LOG("Removed GO: %s", (*itChild)->name);
			delete *itChild;
			goChilds.erase(itChild++);
		} else {
			++itChild;
		}

	}

}

void GameObject::CleanUp() {

	for (auto &child : goChilds) {
		child->CleanUp();
	}

}

void GameObject::DrawProperties() {

	if (ImGui::Checkbox("Enabled", &enabled)) {
		for (auto &component : components) {
			component->enabled = enabled;
		}
	} ImGui::SameLine();

	char* newName = new char[MAXNAME];
	strcpy(newName, name.c_str());
	ImGui::InputText("Name", (char*)newName, MAXNAME);
	name = newName;
	delete[] newName;


	if (ImGui::Checkbox("Static", &staticGo)) {
		UpdateStaticChilds(staticGo);
	}

	if (ImGui::CollapsingHeader("Info")) {
		ImGui::Text("UUID: "); ImGui::SameLine();
		ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, uuid);
		ImGui::Text("Parent UUID: "); ImGui::SameLine();
		ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, parentUuid);
	}

	if(components.size() > 0) {
		for (std::list<Component*>::iterator it = components.begin(); it != components.end();) {
			if (!(*it)->toBeDeleted) {
				(*it)->DrawProperties(staticGo);
				it++;
			} else {
				it = RemoveComponent(it);
			}
		}
	}
}

void GameObject::DrawHierarchy(GameObject* goSelected) {
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (goSelected == this ? ImGuiTreeNodeFlags_Selected : 0);

	ImGui::PushID(this);
	if (goChilds.empty()) {
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool obj_open = ImGui::TreeNodeEx(this, node_flags, name.c_str());

	if (ImGui::IsItemClicked()) {
		if (App->scene->goSelected != nullptr) {
			App->scene->goSelected->drawGOBBox = false;
		}
		App->scene->goSelected = this;
		drawGOBBox = true;
	} 

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){
		GameObject* draggedGo = this;
		ImGui::SetDragDropPayload("DragDropHierarchy", &draggedGo, sizeof(GameObject*), ImGuiCond_Once);
		ImGui::Text("%s", name);
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragDropHierarchy")) {
			IM_ASSERT(payload->DataSize == sizeof(GameObject*));
			GameObject* droppedGo = (GameObject*)*(const int*)payload->Data;

			if (droppedGo->parent != this) {

				bool droppedIntoChild = false;
				GameObject* inheritedTrasnform = this;
				while (inheritedTrasnform->parent != nullptr) {
					if (inheritedTrasnform->parent == droppedGo) {
						droppedIntoChild = true;
					}
					inheritedTrasnform = inheritedTrasnform->parent;
				}
				inheritedTrasnform = nullptr;

				if (!droppedIntoChild) {
					goChilds.push_back(droppedGo);

					if (droppedGo->transform != nullptr) {
						droppedGo->transform->SetLocalToWorld(droppedGo->transform->GetGlobalTransform());
					}
					droppedGo->parent->goChilds.remove(droppedGo);
					droppedGo->parent = this;
					sprintf_s(droppedGo->parentUuid, uuid);
					if (droppedGo->transform != nullptr) {
						droppedGo->transform->SetWorldToLocal(droppedGo->parent->transform->GetGlobalTransform());
					}
				}

			}
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemHovered() && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		ImGui::OpenPopup("Modify_GameObject");
		App->scene->goSelected = this;
	}

	if (ImGui::BeginPopup("Modify_GameObject")) {
		if (ImGui::Selectable("Add Empty GameObject")) {
			App->scene->CreateGameObject(DEFAULT_GO_NAME, this);
		}
		if (ImGui::Selectable("Duplicate") && App->scene->goSelected != nullptr) {
			toBeCopied = true;
		}
		if (ImGui::Selectable("Remove") && App->scene->goSelected != nullptr) {
			toBeDeleted = true;
			if (App->scene->goSelected == this) {
				App->scene->goSelected = nullptr;
			}
		}
		if (ImGui::Selectable("Move up") && App->scene->goSelected != nullptr) {
			moveGOUp = true;
		}
		if (ImGui::Selectable("Move down") && App->scene->goSelected != nullptr) {
			moveGODown = true;
		}

		ImGui::EndPopup();
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

Component* GameObject::AddComponent(ComponentType type) {
	Component* component = nullptr;

	switch (type) {
		case ComponentType::CAMERA:
			component = new ComponentCamera(this);
			if (App->camera->selectedCamera == nullptr) {
				App->camera->selectedCamera = (ComponentCamera*)component;
			}
			App->camera->gameCameras.push_back((ComponentCamera*)component);
			break;
		case ComponentType::TRANSFORM:
			if (GetComponent(ComponentType::TRANSFORM) == nullptr) {
				component = new ComponentTransform(this, math::float4x4().identity);
				transform = (ComponentTransform*)component;
			} else {
				LOG("This GO already have a TRANSFORM");
			}
			break;
		case  ComponentType::MESH:
			if (GetComponent(ComponentType::MESH) == nullptr) {
				component = new ComponentMesh(this);
				mesh = (ComponentMesh*)component;
				//We need to have a material to render a mesh, so we include one if not already added
				AddComponent(ComponentType::MATERIAL);
			} else {
				LOG("This GO already have a MESH");
			}
			break;
		case ComponentType::MATERIAL:
			if (GetComponent(ComponentType::MATERIAL) == nullptr) {
				component = new ComponentMaterial(this);
				material = (ComponentMaterial*)component;
			} else {
				LOG("This GO already have a MATERIAL");
			}
			break;
		case ComponentType::EMPTY:
		default:
			break;
	}

	if (component != nullptr) {
		components.push_back(component);
	}
	return component;
}

std::list<Component*>::iterator GameObject::RemoveComponent(std::list<Component*>::iterator component) {
	assert(*component != nullptr);

	delete *component;
	*component = nullptr;
	return components.erase(component);
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

void GameObject::ModelTransform(unsigned shader) const {
	//TODO: we could probably check if GO have transfom if we want to generate GO without location as Scripts Components, etc.
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_TRUE, &transform->GetGlobalTransform()[0][0]);
}

void GameObject::ComputeBBox() {

	for (auto& child : goChilds) {
		if (child->mesh != nullptr) {
			child->ComputeBBox();
		}
	}

	if (mesh != nullptr && transform != nullptr) {
		bbox.SetNegativeInfinity();
		bbox.Enclose(mesh->mesh.bbox);
		bbox.TransformAsAABB(transform->GetGlobalTransform());
	}

}

void GameObject::UpdateStaticChilds(bool staticState) {
	staticGo = staticState;
	if (staticGo && GetComponent(ComponentType::MESH) != nullptr) {
		App->scene->quadTree->Insert(this, true);
	} else if (!staticGo && GetComponent(ComponentType::MESH) != nullptr) {
		App->scene->quadTree->Remove(this);
	}
	for(auto &child : goChilds){
		child->UpdateStaticChilds(staticState);
	}
}

/* RapidJson storage */
bool GameObject::Save(Config* config) {
	config->StartObject();

	config->AddString("uuid", uuid);
	config->AddString("name", name.c_str());

	if (parent != nullptr) {
		config->AddString("parentUuid", parent->uuid);
	}

	config->AddBool("enabled", enabled);
	config->AddBool("static", staticGo);

	config->StartArray("components");

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		(*it)->Save(config);
	}

	config->EndArray();

	config->EndObject();

	return true;
}

void GameObject::Load(Config* config, rapidjson::Value& value) {
	sprintf_s(uuid, config->GetString("uuid", value));

	enabled = config->GetBool("enabled", value);
	staticGo = config->GetBool("static", value);

	if (parent != nullptr) {
		sprintf_s(parentUuid, config->GetString("parentUuid", value));
	} else {
		sprintf_s(parentUuid, "");
	}
		 
	rapidjson::Value components = value["components"].GetArray();

	for (rapidjson::Value::ValueIterator it = components.Begin(); it != components.End(); ++it) {
		Component* component = AddComponent(config->GetComponentType("componentType", (*it)));

		if (component != nullptr) {
			component->Load(config, (*it));
		}
	}

	if (staticGo) {
		App->scene->quadTree->Insert(this, true);
	}

}