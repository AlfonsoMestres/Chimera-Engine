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

	if (drawGOBBox) {
		DrawBBox();
	}

	// TODO: make child bbox appear too when checked the ui element
	if (drawChildsBBox) {
		for (auto &child : goChilds) {
			child->DrawBBox();
		}
	}

	glUseProgram(0);
}


void GameObject::DrawProperties() const {
	assert(name != nullptr);

	// We could probably spent computing time calculating the goName length, but instead we use a fixed max name length
	ImGui::InputText("Name", (char*)name, 30.0f);

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

//TODO: this is not computing correctly, chimney is not added correctly to the global AABB
AABB GameObject::ComputeBBox() const {
	bbox.SetNegativeInfinity();

	// Current GO meshes
	for (const auto &mesh : GetComponents(ComponentType::MESH)) {
		bbox.Enclose(((ComponentMesh *)mesh)->bbox);
	}

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	// Child meshes
	for (const auto &child : goChilds){
		if (child->GetComponents(ComponentType::MESH).size() != 0) {
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
		0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	float4x4 boxtransform = float4x4::FromTRS(bbox.CenterPoint(), Quat::identity, bbox.Size());
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