#include "ModuleFileSystem.h"
#include "SDL.h"
#include "physfs.h"

ModuleFileSystem::ModuleFileSystem() { }

ModuleFileSystem::~ModuleFileSystem() { }

bool ModuleFileSystem::Init() {
	basePath = SDL_GetPrefPath("Chimera", "");
	PHYSFS_init(basePath);
	SDL_free(basePath);

	AddPath(".");

	AddPath("../Game/");

	PHYSFS_setWriteDir(".");

	if (PHYSFS_exists("/Library/") == 0) {
		PHYSFS_mkdir("/Library/");
	}
	if (PHYSFS_exists("/Library/Textures/") == 0) {
		PHYSFS_mkdir("/Library/Textures/");
	}
	if (PHYSFS_exists("/Library/Meshes/") == 0) {
		PHYSFS_mkdir("/Library/Meshes/");
	}
	if (PHYSFS_exists("/Library/Scene/") == 0) {
		PHYSFS_mkdir("/Library/Scene/");
	}
	return true;
}

bool ModuleFileSystem::CleanUp() {
	PHYSFS_deinit();
	return true;
}

unsigned ModuleFileSystem::Load(const char* pathAndFileName, char** buffer) const {
	unsigned result = 0u;

	PHYSFS_file* fsFile = PHYSFS_openRead(pathAndFileName);

	if (fsFile != nullptr) {
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);

		if (size > 0) {
			*buffer = new char[size];
			unsigned readed = (unsigned)PHYSFS_read(fsFile, *buffer, 1, size);
			if (readed != size) {
				LOG("Error reading from file %s: %s\n", pathAndFileName, PHYSFS_getLastError());
				delete[] buffer;
				buffer = nullptr;
			} else {
				result = readed;
			}
		}

		if (PHYSFS_close(fsFile) == 0) {
			LOG("Error closing file %s: %s\n", pathAndFileName, PHYSFS_getLastError());
		}
	}
	else
	{
		const char* error = PHYSFS_getLastError();
		LOG("Error opening file %s: %s\n", pathAndFileName, error);
	}

	return result;
}

unsigned ModuleFileSystem::Save(const char* pathAndFileName, const void* buffer, unsigned size, bool append) const {
	unsigned result = 0u;

	bool overwrite = PHYSFS_exists(pathAndFileName) != 0;
	PHYSFS_file* fsFile = (append) ? PHYSFS_openAppend(pathAndFileName) : PHYSFS_openWrite(pathAndFileName);

	if (fsFile != nullptr) {
		unsigned written = (unsigned)PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written == size) {
			result = written;
			LOG("File %s saved successfully", pathAndFileName);
		} else {
			LOG("Error while writing to file %s: %s", pathAndFileName, PHYSFS_getLastError());
		}

		if (PHYSFS_close(fsFile) == 0) {
			LOG("Error closing file %s: %s", pathAndFileName, PHYSFS_getLastError());
		}

	} else {
		LOG("Error opening file %s: %s", pathAndFileName, PHYSFS_getLastError());
	}

	return result;
}

bool ModuleFileSystem::Remove(const char* pathAndFileName)
{
	bool result = false;

	if (pathAndFileName != nullptr) {
		PHYSFS_delete(pathAndFileName);
		LOG("File deleted: [%s]", pathAndFileName);
		result = true;
	}

	return result;
}

bool ModuleFileSystem::Exists(const char* pathAndFileName) const {
	return PHYSFS_exists(pathAndFileName) != 0;
}

bool ModuleFileSystem::MakeDirectory(const char* path) {
	return PHYSFS_mkdir(path) != 0;
}

bool ModuleFileSystem::AddPath(const char* path) {
	bool result = false;

	if (PHYSFS_mount(path, nullptr, 1) == 0) {
		LOG("Error adding a path: %s\n", PHYSFS_getLastError());
	} else {
		result = true;
	}

	return result;
}

bool ModuleFileSystem::IsDirectory(const char* pathAndFileName) const {
	return PHYSFS_isDirectory(pathAndFileName) != 0;
}

bool ModuleFileSystem::Copy(const char* sourcePath, const char* destinationPath) {
	bool result = false;

	char buffer[8192];

	PHYSFS_file* source = PHYSFS_openRead(sourcePath);
	PHYSFS_file* destination = PHYSFS_openWrite(destinationPath);

	PHYSFS_sint32 size;
	if (source && destination) {
		while (size = (PHYSFS_sint32)PHYSFS_read(source, buffer, 1, 8192)) {
			PHYSFS_write(destination, buffer, 1, size);
		}

		PHYSFS_close(source);
		PHYSFS_close(destination);
		result = true;

		LOG("Copied file [%s] to [%s]", sourcePath, destinationPath);
	} else {
		LOG("Error while copy from [%s] to [%s]", sourcePath, destinationPath);
	}

	return result;
}

std::map<std::string, std::string> ModuleFileSystem::GetFilesFromDirectoryRecursive(const char* directory) {
	std::map<std::string, std::string> result;
	char **enumeratedFIles = PHYSFS_enumerateFiles(directory);
	char **iterator;

	std::string dir(directory);
	std::vector<std::string> directoryList;

	for (iterator = enumeratedFIles; *iterator != nullptr; iterator++) {
		if (PHYSFS_isDirectory((dir + *iterator).c_str())) {
			directoryList.push_back(*iterator);
		} else {
			result[(*iterator)] = dir;
		}
	}

	PHYSFS_freeList(enumeratedFIles);

	for (std::vector<std::string>::iterator iterator = directoryList.begin(); iterator != directoryList.end(); ++iterator) {
		(*iterator).insert(0, directory);
		(*iterator).append("/");
		std::map<std::string, std::string> partialResult = GetFilesFromDirectoryRecursive((*iterator).c_str());
		result.insert(partialResult.begin(), partialResult.end());
	}

	return result;
}

void ModuleFileSystem::GetFilesFromDirectory(const char* directory, std::vector<std::string>& fileList) const {
	char **enumeratedFIles = PHYSFS_enumerateFiles(directory);
	char **iterator;

	std::string dir(directory);

	for (iterator = enumeratedFIles; *iterator != nullptr; iterator++) {
		if (!PHYSFS_isDirectory((dir + *iterator).c_str())) {
			fileList.push_back(*iterator);
		}
	}

	PHYSFS_freeList(enumeratedFIles);
}

void ModuleFileSystem::ChangePathSlashes(std::string& fullPath) const {
	for (std::string::iterator iterator = fullPath.begin(); iterator != fullPath.end(); ++iterator) {
		if (*iterator == '\\') {
			*iterator = '/';
		} else {
			*iterator = tolower(*iterator);
		}
	}
}

void ModuleFileSystem::SplitFilePath(const char* fullPath, std::string* path, std::string* file, std::string* extension) const {
	if (fullPath != nullptr) {
		std::string fullPathString(fullPath);
		ChangePathSlashes(fullPathString);
		size_t positionSeparator = fullPathString.find_last_of("\\/");
		size_t positionDot = fullPathString.find_last_of(".");

		if (path != nullptr) {
			if (positionSeparator < fullPathString.length()) {
				*path = fullPathString.substr(0, positionSeparator + 1);
			} else {
				path->clear();
			}
		}

		if (file != nullptr) {
			if (positionSeparator < fullPathString.length()) {
				*file = fullPathString.substr(positionSeparator + 1, positionDot - positionSeparator - 1);
			} else {
				*file = fullPathString;
			}
		}

		if (extension != nullptr) {
			if (positionDot < fullPathString.length()) {
				*extension = fullPathString.substr(positionDot + 1);
			} else {
				extension->clear();
			}
		}
	}
}

std::string ModuleFileSystem::NewGuuid() {
	return xg::newGuid().str();
}