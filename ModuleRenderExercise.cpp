#include "Application.h"
#include "ModuleShader.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	InitFrustum();

	// Generate program with vertex and fragment shaders and load it to GL
	program = App->shader->LoadShaders("../default.vs", "../default.fs");

	if (!program) {
		LOG("Error: Program cannot be compiled");
		return false;
	}

	glUseProgram(program);

	// Triangle exercise
    float vboData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f
	};

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

update_status ModuleRenderExercise::Update()
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

	//Use shaders loadeds in program
	glUseProgram(program);


	//Uniforms - This could be inside a ImgUI to edit manually

	// Fragment shader coloring
	int fragUnifLocation = glGetUniformLocation(program, "newColor");
	float color[4] = { 0.5f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(fragUnifLocation, 1, color);

	// Vertex shader to GPU
	math::float3 target(0.0f, 0.0f, 0.0f); // Our basic camera position and angle
	math::float3 eye(0.0f, 0.0f, 7.0f);
	math::float3 up(0.0f, 1.0f, 0.0f);
	math::float4x4 Model(math::float4x4::identity); // Not moving anything
	
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &LookAt(target, eye, up)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &ProjectionMatrix()[0][0]);


	// Draw every GL_TRIANGLE that starts at vec[0] and you can find 3 of them
    glDrawArrays(GL_TRIANGLES, 0, 3); 

	// Cleaning for next frame
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    if(vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }

	return true;
}


math::float4x4 ModuleRenderExercise::ProjectionMatrix()
{
	math::float4x4 projectMatrix;
	projectMatrix = frustum.ProjectionMatrix();

	return projectMatrix;
}

math::float4x4 ModuleRenderExercise::LookAt(math::float3& target, math::float3& eye, math::float3& up)
{
	math::float4x4 matrix;

	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));

	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye); matrix[1][3] = -u.Dot(eye); matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

void ModuleRenderExercise::InitFrustum() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;								// TODO: Change this to -1 * instead /
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (SCREEN_WIDTH / SCREEN_HEIGHT));
}
