#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ComponentMaterial.h"
#include "MaterialImporter.h"
#include "ModuleFileSystem.h"

ModuleTextures::ModuleTextures() { }

ModuleTextures::~ModuleTextures() {
	CleanUp();
}

bool ModuleTextures::Init() {
	LOG("Init Image library");

	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();

	LoadDefaulTextures();

	return ilutRenderer(ILUT_OPENGL);
}

bool ModuleTextures::CleanUp() {
	for (std::list<ComponentMaterial*>::iterator iterator = materials.begin(); iterator != materials.end();) {
		delete *iterator;
		iterator = materials.erase(iterator);
		*iterator = nullptr;
	}

	delete defaultTexture;
	delete noCameraSelectedTexture;
	defaultTexture = nullptr;
	noCameraSelectedTexture = nullptr;

	return true;
}

Texture* const ModuleTextures::Load(const char* path) {
	assert(path != nullptr);

	ILuint imageId = 0u;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(path)) {
		unsigned textureId = 0u;
		ILinfo imageInfo;
		int pixelDepth = 0;
		int format = 0;
		int height = 0;
		int width = 0;

		// Generate a new texture
		glGenTextures(1, &textureId);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureId);

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
			LOG("Error: Could not convert the image to texture correctly. %s", iluErrorString(ilGetError()));
			return nullptr;
		}

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());

		// Filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);

		if (wrapMode != NULL) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		}

		// TODO: Set linear or nearest mipmap
		if (mipmaping) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipMapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMapMode);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureId);
		} 

		ilDeleteImages(1, &imageId); 
		glBindTexture(GL_TEXTURE_2D, 0);

		return new Texture(textureId, width, height); 
	}

	LOG("Error: Texture loading %s",iluErrorString(ilGetError()));

	return nullptr;
}

void ModuleTextures::LoadMaterial(std::string path, unsigned& textureID, int& width, int& height) {
	unsigned imageID;

	ilGenImages(1, &imageID);

	ilBindImage(imageID);

	path.insert(0, "/Library/Textures/");

	LOG("Loading material %s", path.c_str());

	char* fileBuffer;
	unsigned lenghBuffer = App->fileSystem->Load(path.c_str(), &fileBuffer);

	if (ilLoadL(IL_DDS, fileBuffer, lenghBuffer)) {
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

		if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
			LOG("Error: Image conversion failed %s", iluErrorString(ilGetError()));
			return;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}

	ilDeleteImages(1, &imageID);
	LOG("Material creation successful.");
}

void ModuleTextures::LoadMaterial(const char* path, ComponentMaterial* componentMaterial, MaterialType materialTypeSelected) {
	switch (materialTypeSelected) {
		case MaterialType::OCCLUSION_MAP:
			if (componentMaterial->material.occlusionMap != 0u) {
				Unload(componentMaterial->material.occlusionMap);
			}
			LoadMaterial(path, componentMaterial->material.occlusionMap, componentMaterial->material.ambientWidth, componentMaterial->material.ambientHeight);
			break;
		case MaterialType::DIFFUSE_MAP:
			if (componentMaterial->material.diffuseMap != 0u) {
				Unload(componentMaterial->material.diffuseMap);
			}
			LoadMaterial(path, componentMaterial->material.diffuseMap, componentMaterial->material.diffuseWidth, componentMaterial->material.diffuseHeight);
			break;
		case MaterialType::SPECULAR_MAP:
			if (componentMaterial->material.specularMap != 0u) {
				Unload(componentMaterial->material.specularMap);
			}
			LoadMaterial(path, componentMaterial->material.specularMap, componentMaterial->material.specularWidth, componentMaterial->material.specularHeight);
			break;
		case MaterialType::EMISSIVE_MAP:
			if (componentMaterial->material.emissiveMap != 0u) {
				Unload(componentMaterial->material.emissiveMap);
			}
			LoadMaterial(path, componentMaterial->material.emissiveMap, componentMaterial->material.emissiveWidth, componentMaterial->material.emissiveHeight);
			break;
	}
}

void ModuleTextures::Unload(unsigned id) {
	if (id != 0u) {
		glDeleteTextures(1, &id);
	}
}

void ModuleTextures::LoadDefaulTextures() {
	//TODO: check if in assets folder, if not load this again
	noCameraSelectedTexture = Load("nocamselected.jpg");
	defaultTexture = Load("checkers.jpg");

	//TODO: check if checkers is located in Library/Textures, if not, generate new checkers
	if (!App->fileSystem->Exists("/Library/Textures/checkers.dds")) {
		if (App->fileSystem->Exists("/Assets/Default/checkers.png")) {
			MaterialImporter::Import("/Assets/Default/checkers.png");
		} else {
			LOG("Error: No default texture found");
		}
	} 
	//TODO: This will be used when we code the materials list in the scene, not for now
	/*ComponentMaterial* baseMaterial = (ComponentMaterial*)App->scene->root->AddComponent(ComponentType::MATERIAL);
	App->textures->LoadMaterial("checkers.dds", baseMaterial, MaterialType::DIFFUSE_MAP);*/

}

void ModuleTextures::DrawGUI() {
	ImGui::Text("This will be applied only to the next loaded models");
	ImGui::Text("Filter type:");
	ImGui::RadioButton("Linear", &filterType, GL_LINEAR); ImGui::SameLine();
	ImGui::RadioButton("Nearest", &filterType, GL_NEAREST);
	ImGui::Text("Fill type:");
	ImGui::RadioButton("Clamp", &wrapMode, GL_CLAMP); ImGui::SameLine();
	ImGui::RadioButton("Clamp to border", &wrapMode, GL_CLAMP_TO_BORDER); ImGui::SameLine();
	ImGui::RadioButton("Repeat", &wrapMode, GL_REPEAT); ImGui::SameLine();
	ImGui::RadioButton("Mirrored repeat", &wrapMode, GL_MIRRORED_REPEAT);
	if (ImGui::Checkbox("Mipmap", &mipmaping)) { 
		ImGui::RadioButton("Nearest", &mipMapMode, GL_NEAREST_MIPMAP_NEAREST); ImGui::SameLine();
		ImGui::RadioButton("Linear", &mipMapMode, GL_NEAREST_MIPMAP_LINEAR);
	}
}
