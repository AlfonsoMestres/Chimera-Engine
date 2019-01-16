#include "ModuleProgram.h"

ModuleProgram::ModuleProgram() { }

ModuleProgram::~ModuleProgram() {
	CleanUp();
}

bool ModuleProgram::LoadPrograms() {
	// TODO: this should be pushed back to a vector
	colorProgram = LoadProgram("./Shaders/color.vs", "./Shaders/color.fs");
	textureProgram = LoadProgram("./Shaders/texture.vs", "./Shaders/texture.fs");
	blinnProgram = LoadProgram("./Shaders/blinn.vs", "./Shaders/blinn.fs");

	return (colorProgram != 0 && textureProgram != 0 && blinnProgram != 0);
}

unsigned ModuleProgram::LoadProgram(const char* vertShaderPath, const char* fragShaderPath) {
	unsigned program = 0u;

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
	
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	// Remove shaders, we wont need them anymore if they are loaded correctly into Program
	delete[] vertShaderStr; 
	delete[] fragShaderStr;
	vertShaderStr = nullptr;
	fragShaderStr = nullptr;

	return program;
}

char* ModuleProgram::ReadShaderFile(const char* shaderPath) {
	FILE* file = nullptr;
 	char* shaderData = nullptr;
	int err = fopen_s(&file, shaderPath, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		shaderData = (char*)malloc(size + 1);
		int res = fread(shaderData, 1, size, file);
		shaderData[size] = 0;
		if (res != size) {
			LOG("Error: Shader not loaded correctly");
			shaderData = nullptr;
		}
		fclose(file);
	} else {
		LOG("Error: Shader reading failed with %s", shaderPath);
		shaderData = nullptr;
	}

	return shaderData;
}

bool ModuleProgram::CompileShader(unsigned shaderAddress, const char* shaderContent) {
	assert(shaderContent != nullptr);

	int compiled = GL_FALSE;

	glShaderSource(shaderAddress, 1, &shaderContent, NULL);
	glCompileShader(shaderAddress);
	glGetShaderiv(shaderAddress, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		int infoLogLength = 0;
		glGetShaderiv(shaderAddress, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0) {
			GLchar* strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shaderAddress, infoLogLength, NULL, strInfoLog);

			LOG("Error: Shader compilation failed at %s", strInfoLog);

			delete[] strInfoLog;
			strInfoLog = nullptr;
		}

		glDeleteShader(shaderAddress); // Don't leak the shader.
	}

	return compiled;
}

void ModuleProgram::CompileProgram(unsigned programAddress) {
	int errorLength = 0;

	glGetProgramiv(programAddress, GL_COMPILE_STATUS, &errorLength);

	if (errorLength > 0) {
		GLchar* strInfoLog = new GLchar[errorLength + 1];
		glGetProgramInfoLog(programAddress, errorLength, NULL, strInfoLog);

		LOG("Error: Program compilation failed at %s", strInfoLog);

		delete[] strInfoLog;
		strInfoLog = nullptr;

		glDeleteProgram(programAddress); // Don't leak the program.
	}

}

bool ModuleProgram::CleanUp() {
	glDeleteProgram(colorProgram);
	glDeleteProgram(textureProgram);
	glDeleteProgram(blinnProgram);
	colorProgram = 0u;
	textureProgram = 0u;
	blinnProgram = 0u;
	return true;
}
