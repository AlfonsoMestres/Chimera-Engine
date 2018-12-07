#include "ModuleScene.h"

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
	for (auto &child : this->root->goChilds) {
		child->DrawHierarchy(goSelected);
	}
}

GameObject* ModuleScene::CreateGameObject(const char* goName, GameObject* goParent, const aiMatrix4x4& transform, const char* fileLocation) {

	GameObject* gameObject = nullptr;

	if (goName != nullptr) {

		// TODO: this should be deleted
		char* go_name = new char[strlen(goName)];
		strcpy(go_name, goName);
		
		gameObject = new GameObject(go_name, transform, goParent, fileLocation);

	} else {

		if (goParent != nullptr) {
			std::string childName = "ChildOf";
			childName += goParent->name;

			gameObject = new GameObject(childName.c_str(), transform, goParent, fileLocation);
		} else {
			gameObject = new GameObject(std::string("GameObject").c_str(), transform, goParent, fileLocation);
		}

	}

	return gameObject;
}

GameObject* ModuleScene::DuplicateGO(GameObject* goToDuplicate) {

	if (goToDuplicate == nullptr) return nullptr;

	GameObject* duplicatedGo = new GameObject(goToDuplicate);
	duplicatedGo->parent = goToDuplicate->parent;
	duplicatedGo->parent->goChilds.push_back(duplicatedGo);
	LOG("Duplicated %s", duplicatedGo->name);
	return duplicatedGo;
}