#ifndef __MODULESHADER_H__
#define __MODULESHADER_H__

#include "Module.h"
#include "GL/glew.h"

class ModuleShader : public Module
{
public:
	ModuleShader();
	~ModuleShader();

	char*	ReadShaderFile(const char* shaderPath);
	GLuint	LoadShaders(const char* vertShaderPath, const char* fragShaderPath);
	void	CheckCompilationErrors(GLuint shader);

};

#endif
