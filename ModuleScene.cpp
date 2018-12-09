#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_mouse.h"

ModuleScene::ModuleScene() { }

ModuleScene::~ModuleScene() { }

bool ModuleScene::Init() {
	root = new GameObject();
	root->name = "root";

	return true;
}

update_status ModuleScene::Update() {
	update_status ret = UPDATE_CONTINUE;

	root->Update();

	return ret;
}

void ModuleScene::Draw() {
	root->Draw();
}

void ModuleScene::DrawHierarchy() {
	//TODO!: Create empty game object, appears in the middle of the frustum render
	// If empty heriarchy, allow to create one with a imgui popup
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		ImGui::OpenPopup("Modify_GameObject");
	}

	if (ImGui::BeginPopup("Modify_GameObject")) {
		if (ImGui::Selectable("Add Empty GameObject")) {
			App->scene->CreateGameObject();
		}
		ImGui::EndPopup();
	}

	for (auto &child : root->goChilds) {
		child->DrawHierarchy(goSelected);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* goName, GameObject* goParent, const aiMatrix4x4& transform, const char* fileLocation) {

	GameObject* gameObject = nullptr;

	if (goName != nullptr) {

		char* go_name = new char[strlen(goName)];
		strcpy(go_name, goName);
		
		gameObject = new GameObject(go_name, transform, goParent, fileLocation);

	} else {

		if (goParent != nullptr) {
			std::string childName = "ChildOf";
			childName += goParent->name;

			gameObject = new GameObject(childName.c_str(), transform, goParent, fileLocation);
		} else {
			gameObject = new GameObject(DEFAULT_GO_NAME, transform, goParent, fileLocation);
		}

	}

	return gameObject;
}
