#include "Application.h"
#include "ModuleLibrary.h"
#include "ModuleFileSystem.h"
#include "DockAssets.h"

DockAssets::DockAssets() { }

DockAssets::~DockAssets() { }
 
void DockAssets::Draw() {

	if (!ImGui::Begin("Assets", &enabled)) {
		ImGui::End();
		return;
	}

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	bool libraryOpen = ImGui::TreeNodeEx("Library", node_flags, "Library");

	if (libraryOpen) {
		DrawTreeNode("Meshes", false);
		DrawTreeNode("Textures", false);

		ImGui::TreePop();
	}

	ImGui::End();
}

void DockAssets::DrawTreeNode(const char* name, bool isLeaf) {
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (isLeaf) {
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (itemSelected == name) {
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}

	bool resourceOpen = ImGui::TreeNodeEx(name, nodeFlags, name);

	ClickBehaviour(name);

	if (resourceOpen) {
		if (name == "Meshes") {
			for (std::vector<std::string>::iterator iterator = App->library->fileMeshesList.begin(); iterator != App->library->fileMeshesList.end(); ++iterator) {
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		else if (name == "Textures") {
			for (std::vector<std::string>::iterator iterator = App->library->fileTexturesList.begin(); iterator != App->library->fileTexturesList.end(); ++iterator) {
				DrawTreeNode((*iterator).c_str(), true);
			}
		}

		ImGui::TreePop();
	}
}

void DockAssets::ClickBehaviour(const char* name) {
	if (ImGui::IsItemClicked(0)) {
		itemSelected = name;
	}

	if (ImGui::IsItemClicked(1)) {
		itemSelected = name;
		ImGui::OpenPopup("ContextMenu");
	}

	if (itemSelected == name) {
		if (ImGui::BeginPopup("ContextMenu")) {

			if (ImGui::MenuItem("Delete")) {
				std::string ext;
				App->fileSystem->SplitFilePath(name, nullptr, nullptr, &ext);
				std::string nameToRemove = name;
				if (ext == "head") {
					nameToRemove.insert(0, "/Library/Meshes/");
					App->library->removeHead = true;
					App->library->toBeDeleted = true;
				} else if (ext == "dds") {
					nameToRemove.insert(0, "/Library/Textures/");
					App->library->removeHead = false;
					App->library->toBeDeleted = true;
				}

				App->fileSystem->Remove(nameToRemove.c_str());
			}
			ImGui::EndPopup();
		}
	}
}

bool DockAssets::IsFocused() const { return focus; }