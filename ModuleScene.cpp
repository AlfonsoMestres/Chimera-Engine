#include "Application.h"
#include "ModuleShader.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"

ModuleScene::ModuleScene()
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Handle texture loader
	texture0 = App->textures->Load("./textures/Lenna.png");

	if (texture0 == -1) {
		LOG("Error: Texture cannot be loaded");
		return false;
	}

	// Generate program with vertex and fragment shaders and load it to GL
	progDefault = App->shader->LoadShaders("../default.vs", "../default.fs");

	if (!progDefault) {
		LOG("Error: Program cannot be compiled");
		return false;
	}

	// Generate program with vertex and fragment shaders and load it to GL
	progTexture = App->shader->LoadShaders("../texture.vs", "../texture.fs");

	if (!progTexture) {
		LOG("Error: Program cannot be compiled");
		return false;
	}

	// Triangle exercise
    float vboData[] = {
		// Vertex
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		// UVs
		0.0f, 0.0f, 
		1.0f, 0.0f,
		0.0f, 1.0f, 
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

update_status ModuleScene::Update()
{

	// This is what kind of info GL is going to read and where do it needs to read from
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
            0,                  // attribute 0
            3,                  // number of componentes (3 floats)
            GL_FLOAT,           // data type
            GL_FALSE,           // should be normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 6));

	//Printing Colors with default shader

	glUseProgram(progDefault);

	// Editor References
	DrawReferenceGround();
	DrawReferenceAxis();

	//Uniforms - This could be inside a ImgUI to edit manually

	// Fragment shader coloring
	int fragUnifLocation = glGetUniformLocation(progDefault, "newColor");
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(fragUnifLocation, 1, color);

	// Vertex shader to GPU
	math::float4x4 Model(math::float4x4::identity); // Not moving anything
	
	glUniformMatrix4fv(glGetUniformLocation(progDefault, "proj"), 1, GL_TRUE, &App->camera->ProjectionMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progDefault, "view"), 1, GL_TRUE, &App->camera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progDefault, "model"), 1, GL_TRUE, &Model[0][0]);

	// Printing texture with texture shader
	glUseProgram(progTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(progTexture, "texture0"), 0);

	glUniformMatrix4fv(glGetUniformLocation(progTexture, "proj"), 1, GL_TRUE, &App->camera->ProjectionMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progTexture, "view"), 1, GL_TRUE, &App->camera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(progTexture, "model"), 1, GL_TRUE, &Model[0][0]);

	// Draw every GL_TRIANGLE that starts at vec[0] and you can find 6 of them
    glDrawArrays(GL_TRIANGLES, 0, 6); 

	// Cleaning for next frame
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
    if(vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }

	if (texture0 != 0) {
		glDeleteTextures(1, &texture0);
	}

	return true;
}

void ModuleScene::DrawReferenceGround() {
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

void ModuleScene::DrawReferenceAxis() {
	glLineWidth(2.0f);

	// red X
	int xAxis = glGetUniformLocation(progDefault, "newColor");
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, red);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();
	
	// green Y
	int yAxis = glGetUniformLocation(progDefault, "newColor");
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	int zAxis = glGetUniformLocation(progDefault, "newColor");
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	glLineWidth(1.0f);
}

