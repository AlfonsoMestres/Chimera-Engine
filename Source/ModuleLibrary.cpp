#include "ModuleLibrary.h"
#include "Application.h"
#include "MeshImporter.h"
#include "ModuleFileSystem.h"
#include "MaterialImporter.h"
#include "imgui.h"
#include "thread";

bool stopWatcher = false;

ModuleLibrary::ModuleLibrary() { }

ModuleLibrary::~ModuleLibrary() { 
	CleanUp();
}

void LibraryWatcher() {
	std::map<std::string, std::string> oldFilesAssets;
	std::map<std::string, std::string> currentFilesAssets;
	std::map<std::string, std::string> currentFilesLibrary;
	while (!stopWatcher) {
		App->fileSystem->GetFilesFromDirectoryRecursive("/Assets/", true, currentFilesAssets);
		if ((oldFilesAssets.size() == 0 && oldFilesAssets.size() != currentFilesAssets.size()) || oldFilesAssets.size() < currentFilesAssets.size()) {
			App->fileSystem->GetFilesFromDirectoryRecursive("/Library/", false, currentFilesLibrary);
			for (std::map<std::string, std::string>::iterator iterator = currentFilesAssets.begin(); iterator != currentFilesAssets.end(); ++iterator) {
				std::string fileName = (*iterator).first;
				App->fileSystem->ChangePathSlashes(fileName);
				if (currentFilesLibrary.find(fileName.substr(0, fileName.length() - 4)) == currentFilesLibrary.end()) {
					std::string ext(fileName.substr(fileName.length() - 3));

					std::string fullPath = (*iterator).second;
					fullPath.append(fileName);
					if (ext == "png" || ext == "tif") {
						MaterialImporter::Import(fullPath.c_str());
					}
					if (ext == "fbx" || ext == "FBX") {
						MeshImporter::ImportFBX(fullPath.c_str());
					}
				}
			}
			oldFilesAssets = currentFilesAssets;
			App->library->UpdateMeshesList();
			App->library->UpdateTexturesList();
			App->library->UpdateScenesList();
		} else if (oldFilesAssets.size() > currentFilesAssets.size()) {
			oldFilesAssets = currentFilesAssets;
		}
		Sleep(1000);
	}
	oldFilesAssets.clear();
	currentFilesAssets.clear();
	currentFilesLibrary.clear();
}

bool ModuleLibrary::Init() {

	std::thread watcherThread(LibraryWatcher);

	fileScenesList = new std::vector<std::string>();
	fileMeshesList = new std::vector<std::string>();
	fileTexturesList = new std::vector<std::string>();

	watcherThread.detach();

	UpdateMeshesList();
	UpdateTexturesList();
	UpdateScenesList();

	return true;
}

update_status ModuleLibrary::Update() {
	BROFILER_CATEGORY("LibraryUpdate()", Profiler::Color::Coral);
	if (toBeDeleted) {
		toBeDeleted = false;
		if (removeHead) {
			UpdateMeshesList();
		} else {
			UpdateTexturesList();
		}
	}

	return UPDATE_CONTINUE;
}

bool ModuleLibrary::CleanUp() {
	stopWatcher = true;

	delete fileScenesList;
	fileScenesList = nullptr;
	delete fileMeshesList;
	fileMeshesList = nullptr;
	delete fileTexturesList;
	fileTexturesList = nullptr;

	Sleep(1000);
	return true;
}

void ModuleLibrary::UpdateMeshesList() {
	fileMeshesList->clear();
	App->fileSystem->GetFilesFromDirectory("/Library/Meshes/", *fileMeshesList);
}

void ModuleLibrary::UpdateTexturesList() {
	fileTexturesList->clear();
	App->fileSystem->GetFilesFromDirectory("/Library/Textures/", *fileTexturesList);
}

void ModuleLibrary::UpdateScenesList() {
	fileScenesList->clear();
	App->fileSystem->GetFilesFromDirectory("/Library/Scenes/", *fileScenesList);
}
