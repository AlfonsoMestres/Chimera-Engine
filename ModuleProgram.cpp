#include "ModuleProgram.h"

ModuleProgram::ModuleProgram() { }


ModuleProgram::~ModuleProgram() { }

unsigned ModuleProgram::LoadProgram(const char* vertShaderPath, const char* fragShaderPath) {
	unsigned program = 0;

	// How to: https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html
	unsigned vertShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	char* vertShaderStr = ReadShaderFile(vertShaderPath);
	char* fragShaderStr = ReadShaderFile(fragShaderPath);

	if (CompileShader(vertShader, vertShaderStr) && CompileShader(fragShader, fragShaderStr)) {

		program = glCreateProgram();

		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		glLinkProgram(program);

		CompileProgram(program);
	}
	
	delete[] vertShaderStr; 
	delete[] fragShaderStr;
	// Remove shaders, we wont need them anymore if they are loaded correctly into Program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

char* ModuleProgram::ReadShaderFile(const char* shaderPath) {
	FILE* file;
	errno_t err = fopen_s(&file, shaderPath, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		char* shaderData = (char*)malloc(size + 1);
		fread(shaderData, 1, size, file);
		shaderData[size] = 0;
		fclose(file);
		return shaderData;
	}

	LOG("Error: Shader reading failed with %s", shaderPath);
	return nullptr;
}

bool ModuleProgram::CompileShader(unsigned shaderAddress, const char* shaderContent) {
	int compiled = GL_FALSE;

	glShaderSource(shaderAddress, 1, &shaderContent, NULL);
	glCompileShader(shaderAddress);
	glGetShaderiv(shaderAddress, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		int infoLogLength = 0.0f;
		glGetShaderiv(shaderAddress, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0.0f) {
			GLchar* strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shaderAddress, infoLogLength, NULL, strInfoLog);

			LOG("Error: Shader failed at %s", strInfoLog);

			delete[] strInfoLog;
			infoLogLength = NULL;
		}

		glDeleteShader(shaderAddress); // Don't leak the shader.
	}

	return compiled;
}

void ModuleProgram::CompileProgram(unsigned programAddress) {
	int errorLength = 0;

	glGetProgramiv(programAddress, GL_COMPILE_STATUS, &errorLength);

	if (errorLength > 0) {
		int written = 0;
		GLchar* strInfoLog = new GLchar[errorLength + 1];
		glGetProgramInfoLog(programAddress, errorLength, &written, strInfoLog);

		LOG("Error: Program failed at %s", strInfoLog);

		delete[] strInfoLog;

		glDeleteProgram(programAddress); // Don't leak the program.
	}

}
