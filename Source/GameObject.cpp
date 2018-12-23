#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ModuleResourceManager.h"
#include "SDL/include/SDL_mouse.h"

/// CARE Creating this without father could lead to memory leak
GameObject::GameObject() { 
	uuid = App->resource->NewGuuid();
}

GameObject::GameObject(const char* goName, const math::float4x4& transform, const char* fileLocation) {

	uuid = App->resource->NewGuuid();
	name = goName;

	if (fileLocation != nullptr) {
		filePath = fileLocation;
	}

	parent = App->scene->root;
	parentUuid = App->scene->root->uuid;
	this->transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	this->transform->AddTransform(transform);
	App->scene->root->goChilds.push_back(this);
}

GameObject::GameObject(const char* goName, const math::float4x4& transform, GameObject* goParent, const char* fileLocation) {

	uuid = App->resource->NewGuuid();
	name = goName;

	if (goParent != nullptr) {
		parent = goParent;
		parentUuid = goParent->uuid;
		goParent->goChilds.push_back(this);
	} else {
		parent = App->scene->root;
		parentUuid = App->scene->root->uuid;
		App->scene->root->goChilds.push_back(this);
	}

	this->transform = (ComponentTransform*)AddComponent(ComponentType::TRANSFORM);
	this->transform->AddTransform(transform);

	if (fileLocation != nullptr) {
		filePath = fileLocation;
	}
}

GameObject::GameObject(const GameObject& duplicateGameObject) {

	uuid = App->resource->NewGuuid();
	parentUuid = duplicateGameObject.parentUuid;
	char* copyName = new char[strlen(duplicateGameObject.name)];
	strcpy(copyName, duplicateGameObject.name);
	name = copyName;
	filePath = duplicateGameObject.filePath;
	bbox = duplicateGameObject.bbox;

	for (const auto &component : duplicateGameObject.components) {
		Component* duplicatedComponent = component->Duplicate();
		components.push_back(duplicatedComponent);
		duplicatedComponent->goContainer = this;
		duplicatedComponent->parentUuid = uuid;
		if (duplicatedComponent->componentType == ComponentType::TRANSFORM) {
			transform = (ComponentTransform*)duplicatedComponent;
		}
	}

	for (const auto &child : duplicateGameObject.goChilds) {
		GameObject* duplicatedChild = new GameObject(*child);
		duplicatedChild->parent = this;
		duplicatedChild->parentUuid = uuid;
		goChilds.push_back(duplicatedChild);
	}
}

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
	goChilds.clear();

}

void GameObject::Update() {

	if (!enabled) return;

	for (std::list<GameObject*>::iterator itChild = goChilds.begin(); itChild != goChilds.end();) {

		(*itChild)->Update();

		// TODO: Move up and down elements
		if ((*itChild)->moveGOUp) {
			(*itChild)->moveGOUp = false;
			if (std::abs(std::distance(goChilds.begin(), itChild)) != 0) {
				LOG("Begin move up");
			}
		}

		if ((*itChild)->moveGODown) {
			(*itChild)->moveGODown = false;
			if (std::abs(std::distance(goChilds.begin(), itChild)) != goChilds.size() - 1) {
				LOG("Begin move down");
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

void GameObject::Draw() const{

	if (!enabled) return;

	for (const auto &child : goChilds) {
		child->Draw();
	}
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

	if (texture == nullptr) {
		texture = App->textures->defaultTexture;
	}

	glUseProgram(shader);
	ModelTransform(shader);

	Component* mesh = GetComponent(ComponentType::MESH);
	if(mesh != nullptr && mesh->enabled) {
		((ComponentMesh*)mesh)->Draw(shader, texture);
	}

	if (drawGOBBox) {
		DrawBBox();
	}

	glUseProgram(0);
}

void GameObject::CleanUp() {

	for (auto &child : goChilds) {
		child->CleanUp();
	}

}

void GameObject::DrawProperties() {
	assert(name != nullptr);

	// We could probably spent computing time calculating the goName length, but instead we use a fixed max name length
	ImGui::InputText("Name", (char*)name, 30.0f); ImGui::SameLine();

	if (ImGui::Checkbox("Enabled", &enabled)) {
		for (auto &component : components) {
			component->enabled = enabled;
		}
	}

	if (ImGui::CollapsingHeader("Info")) {
		ImGui::Text("UUID: "); ImGui::SameLine();
		ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, uuid.c_str());
		ImGui::Text("Parent UUID: "); ImGui::SameLine();
		ImGui::TextColored({ 0.4f,0.4f,0.4f,1.0f }, parentUuid.c_str());
	}

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
						droppedGo->transform->SetLocalToWorld(droppedGo->GetGlobalTransform());
					}
					droppedGo->parent->goChilds.remove(droppedGo);
					droppedGo->parent = this;
					droppedGo->parentUuid = uuid;
					if (droppedGo->transform != nullptr) {
						droppedGo->transform->SetWorldToLocal(droppedGo->parent->GetGlobalTransform());
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
			component = new ComponentCamera(this);
			if (App->camera->selectedCamera == nullptr) {
				App->camera->selectedCamera = (ComponentCamera*)component;
			}
			App->camera->gameCameras.push_back((ComponentCamera*)component);
			break;
		case ComponentType::TRANSFORM:
			component = new ComponentTransform(this, math::float4x4().identity);
			transform = (ComponentTransform*)component;
			break;
		case  ComponentType::MESH:
			if (GetComponent(ComponentType::MESH) != nullptr) {
				LOG("This GO already have a MESH");
			} else {
				component = new ComponentMesh(this, nullptr);
			}
			break;
		case ComponentType::MATERIAL:
			// TODO: Unity allow multiple but its weird to have this. RESTRICT
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

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it) {
		if ((*it) == component) {
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

//TODO: this is not computing correctly, chimney is not added correctly to the global AABB
AABB GameObject::ComputeBBox() const {
	bbox.SetNegativeInfinity();

	// Apply transformation of our GO
	//bbox.TransformAsAABB(GetGlobalTransform());

	// Child meshes
	for (const auto &child : goChilds){
		if (child->GetComponent(ComponentType::MESH) != nullptr) {
			bbox.Enclose(child->ComputeBBox());
		}
	}

	return bbox;
}

// TODO: Migrate this to the debug draw library saw in class
void GameObject::DrawBBox() const {
	glUseProgram(App->program->basicProgram);
	AABB bbox = ComputeBBox();
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
		0.5, -0.5, -0.5, 1.0,
		0.5,  0.5, -0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,
		0.5, -0.5,  0.5, 1.0,
		0.5,  0.5,  0.5, 1.0,
		-0.5,  0.5,  0.5, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 
		2, 6, 3, 7
	};

	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	math::float4x4 boxtransform = math::float4x4::FromTRS(bbox.CenterPoint(), Quat::identity, bbox.Size());
	glUniformMatrix4fv(glGetUniformLocation(App->program->basicProgram, "model"), 1, GL_TRUE, &(boxtransform)[0][0]);

	float color[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(glGetUniformLocation(App->program->basicProgram, "Vcolor"), 1, color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glLineWidth(4.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glLineWidth(1.0f);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
	glUseProgram(0);
}