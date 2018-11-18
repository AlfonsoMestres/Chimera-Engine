#include "CustomFile.h"

CustomFile::CustomFile(const char* filePath) {
	std::string fullFilePath(filePath);

	// OS compatibility
	std::size_t found = fullFilePath.find("\\");
	while (found != std::string::npos) {
		fullFilePath.replace(found, std::string("\\").length(), "/");
		found = fullFilePath.find("\\");
	}

	// Path
	found = fullFilePath.find_last_of("/");
	path = fullFilePath.substr(0, found + 1).c_str();

	// Name
	found = fullFilePath.find_last_of("/");
	name = fullFilePath.substr(found + 1, fullFilePath.length()).c_str();

	// Extension
	found = fullFilePath.find_last_of(".");
	extension = fullFilePath.substr(found + 1, fullFilePath.length()).c_str();

	LOG("File imported: %s", fullFilePath);
}

CustomFile::~CustomFile() { }