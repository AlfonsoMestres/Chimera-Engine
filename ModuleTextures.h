#pragma once
#include<list>
#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"
#include "SDL.h"
#include "SDL_image/include/SDL_image.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool			Init();
	bool			CleanUp();
	GLuint const	Load(const char* path);
	void			ReloadTexture(const char* newPath, GLuint& texture);
	void			SetNewParameter(const char* newPath, GLuint& texture, GLuint newTextFilter, GLuint newResizeMethod, GLuint newWrapMethod, GLuint newClampMethod);
	void			SwitchMipMaps(const char* newPath, GLuint& texture, bool state);

public:
	const char* currentPathLoaded;
	int imgWidth;
	int imgHeight;
	int imgPixelDepth;
	char* imgFormat;
	GLuint textFilter = 0;
	GLuint resizeMethod = 0;
	GLuint wrapMethod = 0;
	GLuint clampMethod = 0;
	bool generateMipMaps = false;

};		   