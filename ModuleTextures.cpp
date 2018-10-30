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
GLuint const ModuleTextures::Load(const char* path)
{
	currentPathLoaded = path;
	ILboolean success;
	ILuint imageID;				// Create an image ID as a ULuint
	GLuint textureID;			// Create a texture ID as a GLuint
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	if (ilLoadImage(path)) {


		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		
		imgWidth = ImageInfo.Width;
		imgHeight = ImageInfo.Height;
		imgPixelDepth = ImageInfo.Depth;
		
		switch (ImageInfo.Format)
		{
			case IL_COLOUR_INDEX: imgFormat = "Colour_index"; break;
			case IL_RGB: imgFormat = "RGB"; break;
			case IL_RGBA: imgFormat = "RGBA"; break;
			case IL_BGR: imgFormat = "BGR"; break;
			case IL_BGRA: imgFormat = "BGRA"; break;
			case IL_LUMINANCE: imgFormat = "Luminance"; break;
			default: imgFormat = "Not handled"; break;
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOG(iluErrorString(error));
			return -1;
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		glTexParameteri(GL_TEXTURE_2D, wrapMethod, clampMethod);

		// MipMap is overriding the texture and the resize methods
		if (generateMipMaps) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureID);
		} else {
			glTexParameteri(GL_TEXTURE_2D, textFilter, resizeMethod);
		}

	}
	else {
		LOG("Error: Image loading error");
		LOG(iluErrorString(ilGetError()));
		return -1;
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOG("Texture loaded correctly");

	return textureID; // Return the GLuint to the texture so you can use it!
}

// Path to new texture and Gluint from the texture that we would like to override
void ModuleTextures::ReloadTexture(const char* newPath, GLuint& texture) {
	if (texture != 0) {
		glDeleteTextures(1, &texture);
	}

	texture = Load(newPath);

	if (texture == -1) {
		LOG("Error: Texture cannot be loaded");
	}

}

void ModuleTextures::SwitchMipMaps(const char* newPath, GLuint& texture, bool state) {
	generateMipMaps = state;

	ReloadTexture(newPath, texture);
}

void ModuleTextures::SetNewParameter(const char* newPath, GLuint& texture, GLuint newTextFilter, GLuint newResizeMethod, GLuint newWrapMethod, GLuint newClampMethod) {

	textFilter = newTextFilter;
	resizeMethod = newResizeMethod;
	wrapMethod = newWrapMethod;
	clampMethod = newClampMethod;

	ReloadTexture(newPath, texture);
}