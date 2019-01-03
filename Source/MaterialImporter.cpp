#include "MaterialImporter.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"	

bool MaterialImporter::Import(const char* path)
{
	bool result = false;

	char* fileBuffer = nullptr;
	unsigned lenghBuffer = App->fileSystem->Load(path, &fileBuffer);

	if (fileBuffer) {
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)fileBuffer, lenghBuffer)) {

			ilEnable(IL_FILE_OVERWRITE);
			ILuint   size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			size = ilSaveL(IL_DDS, NULL, 0);

			if (size > 0) {
				data = new ILubyte[size];
				if (ilSaveL(IL_DDS, data, size) > 0) {
					std::string fileName;
					std::string fileExtension;
					App->fileSystem->SplitFilePath(path, nullptr, &fileName, &fileExtension);

					fileName.insert(0, "/Library/Textures/");
					fileName.append(".dds");

					result = App->fileSystem->Save(fileName.c_str(), data, size, false);
				}

				delete[] data;
				data = nullptr;
			}

			ilDeleteImages(1, &ImageName);
		}
	}

	if (result == false) {
		LOG("Error: Cannot load texture from buffer %s", path);
	}

	return result;
}
