#include "ModuleProgram.h"

ModuleProgram::ModuleProgram() { }

ModuleProgram::~ModuleProgram() { }

bool ModuleProgram::LoadPrograms() {
	// TODO: this should be pushed back to a vector
	basicProgram = LoadProgram("./Shaders/default.vs", "./Shaders/default.fs");
	textureProgram = LoadProgram("./Shaders/texture.vs", "./Shaders/texture.fs");

	return (basicProgram != 0 && textureProgram != 0);
}

unsigned ModuleProgram::LoadProgram(const char* vertShaderPath, const char* fragShaderPath) {
	assert(vertShaderPath != nullptr);
	assert(fragShaderPath != nullptr);

	unsigned program = 0u;

	// How to: https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

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
			infoLogLength = 0;
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

		LOG("Error: Program compilation failed at %s", strInfoLog);

		delete[] strInfoLog;
		strInfoLog = nullptr;

		glDeleteProgram(programAddress); // Don't leak the program.
	}

}

bool ModuleProgram::CleanUp() {
	glDeleteProgram(basicProgram);
	glDeleteProgram(textureProgram);
	basicProgram = 0;
	textureProgram = 0;
	return true;
}
