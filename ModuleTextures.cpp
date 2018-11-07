#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// Default filters
	textFilter = GL_TEXTURE_MIN_FILTER;
	resizeMethod = GL_LINEAR;
	wrapMethod = GL_TEXTURE_WRAP_S;
	clampMethod = GL_CLAMP;

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	
	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	return true;
}

// Load new texture from file path
unsigned ModuleTextures::Load(const char* path) {

	ILuint imageId;				// Create an image ID as a ULuint

	ilGenImages(1, &imageId); 		// Generate the image ID

	ilBindImage(imageId); 			// Bind the image

	if (ilLoadImage(path)) {

		unsigned textureId = 0;			// Create a texture ID as a GLuint

		// Generate a new texture
		glGenTextures(1, &textureId);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureId);

		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);
		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		
		switch (imageInfo.Format)
		{
			case IL_COLOUR_INDEX: imgFormat = "Colour_index"; break;
			case IL_RGB: imgFormat = "RGB"; break;
			case IL_RGBA: imgFormat = "RGBA"; break;
			case IL_BGR: imgFormat = "BGR"; break;
			case IL_BGRA: imgFormat = "BGRA"; break;
			case IL_LUMINANCE: imgFormat = "Luminance"; break;
			default: imgFormat = "Not handled"; break;
		}

		// Using RGBA if we got an alpha channel
		bool success;
		int channels = ilGetInteger(IL_IMAGE_CHANNELS);

		if (channels == 3) {
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		} else if (channels == 4) {
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		// Quit out if we failed the conversion
		if (!success) {

			ILenum error = ilGetError();
			LOG("Error: %s", iluErrorString(error));

			return -1;
		}

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glTexParameteri(GL_TEXTURE_2D, wrapMethod, clampMethod);

		// MipMap is overriding the texture and the resize methods
		if (generateMipMaps) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureId);
		} else {
			glTexParameteri(GL_TEXTURE_2D, textFilter, resizeMethod);
		}

		ilDeleteImages(1, &imageId); // Because we have already copied image data into texture data we can release memory used by image.
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId; // Return the GLuint to the texture so you can use it!
	}

	LOG("Error: Image loading %s",iluErrorString(ilGetError()));

	return -1;
}

// Path to new texture and Gluint from the texture that we would like to override
void ModuleTextures::ReloadTexture(const char* newPath, unsigned texture) {
	
	Unload(texture);

	texture = Load(newPath);

	if (texture == -1) {
		LOG("Error: Texture cannot be loaded");
	}

}

void ModuleTextures::Unload(unsigned texture) {
	if (texture != 0) {
		glDeleteTextures(1, &texture);
	}
}

void ModuleTextures::SwitchMipMaps(const char* newPath, unsigned texture, bool state) {
	generateMipMaps = state;

	ReloadTexture(newPath, texture);
}

void ModuleTextures::SetNewParameter(const char* newPath, unsigned texture, unsigned newTextFilter, unsigned newResizeMethod, unsigned newWrapMethod, unsigned newClampMethod) {

	textFilter = newTextFilter;
	resizeMethod = newResizeMethod;
	wrapMethod = newWrapMethod;
	clampMethod = newClampMethod;

	ReloadTexture(newPath, texture);
}