#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__

#include "Module.h"
#include "GL/glew.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	unsigned	LoadProgram(const char* vertShaderPath, const char* fragShaderPath);

private:
	char*		ReadShaderFile(const char* shaderPath);
	bool		CompileShader(unsigned shaderAddress, const char* shaderContent);
	void		CompileProgram(unsigned programAddress);

};

#endif