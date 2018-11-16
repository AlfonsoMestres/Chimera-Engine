#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModuleTextures::ModuleTextures() { }

ModuleTextures::~ModuleTextures() { }

bool ModuleTextures::Init() {
	LOG("Init Image library");

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();

	return ilutRenderer(ILUT_OPENGL);
}

// Load new texture from file path
Texture const ModuleTextures::Load(const char* path) {
	assert(path != nullptr);

	unsigned textureId = 0u;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(path)) {

		// Generate a new texture
		glGenTextures(1, &textureId);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureId);

		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);

		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		format = ilGetInteger(IL_IMAGE_FORMAT);
		pixelDepth = ilGetInteger(IL_IMAGE_DEPTH);

		// Using RGBA if we got an alpha channel
		bool success = false;
		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3) {
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		} else if (channels == 4) {
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		// Quit if we failed the conversion
		if (!success) {
			LOG("Error: Could not convert the image correctly. %s", iluErrorString(ilGetError()));
			return Texture(0, 0, 0);
		}

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());

		// Filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);

		switch (wrapMode) {

			case 0:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				break;
			case 1:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				break;
			case 2:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case 3:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			default:
				LOG("Warning: wrong texture wrap mode %d", wrapMode);
				break;
		}

		// TODO: Set linear or nearest mipmap
		if (mipmaping) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureId);
		} 

		ilDeleteImages(1, &imageId); 
		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture(textureId, width, height); 
	}

	LOG("Error: Image loading %s",iluErrorString(ilGetError()));

	return Texture(0, 0, 0);
}

void ModuleTextures::DrawGUI() {
	ImGui::Text("This will be applied only to the next loaded models");
	ImGui::Text("Filter type:");
	ImGui::RadioButton("Linear", &filterType, GL_LINEAR); ImGui::SameLine();
	ImGui::RadioButton("Nearest", &filterType, GL_NEAREST);
	if (ImGui::Checkbox("Mipmap", &mipmaping)) { };
}