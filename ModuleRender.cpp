#include "Application.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"

ModuleRender::ModuleRender() { }

// Destructor
ModuleRender::~ModuleRender() { }

// Called before render is available
bool ModuleRender::Init() {
	LOG("Creating Renderer context");

	InitSDL();
	glewInit();
	InitOpenGL();
	InitFrustum();

	if (vsyncEnabled && SDL_GL_SetSwapInterval(1) < 0) {
		LOG("Error: VSync couldn't be enabled \n %s", SDL_GetError());
	}

	App->program->LoadPrograms();
	CreateFrameBuffer();

	return true;
}

update_status ModuleRender::PreUpdate() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update() {

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// TODO: this should affect only the window
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(App->program->textureProgram);
	ModelTransform(App->program->textureProgram);
	ProjectionMatrix(App->program->textureProgram);
	ViewMatrix(App->program->textureProgram);
	App->model->DrawModels();

	glUseProgram(App->program->basicProgram);
	ModelTransform(App->program->basicProgram);
	ProjectionMatrix(App->program->basicProgram);
	ViewMatrix(App->program->basicProgram);
	DrawReferenceDebug();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate() {

	App->editor->RenderGUI();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

void ModuleRender::DrawReferenceDebug() {

	// White grid
	int gridColor = glGetUniformLocation(App->program->basicProgram, "vColor");
	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glUniform4fv(gridColor, 1, white);

	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f) {
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	
	/// AXIS X Y Z
	glLineWidth(2.0f);

	// red X
	int xAxis = glGetUniformLocation(App->program->basicProgram, "vColor");
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, red);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();

	// green Y
	int yAxis = glGetUniformLocation(App->program->basicProgram, "vColor");
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	int zAxis = glGetUniformLocation(App->program->basicProgram, "vColor");
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	glLineWidth(1.0f);

	glUseProgram(0);
}

void ModuleRender::SetScreenNewScreenSize() {
	glViewport(0, 0, App->window->width, App->window->height);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	CreateFrameBuffer();
}

void ModuleRender::CreateFrameBuffer() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->width, App->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error: Framebuffer issue");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::ViewMatrix(unsigned programUsed) {
	int viewLocation = glGetUniformLocation(programUsed, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &viewMatrix[0][0]);
}

void ModuleRender::ProjectionMatrix(unsigned programUsed) {
	int projLocation = glGetUniformLocation(programUsed, "proj");
	glUniformMatrix4fv(projLocation, 1, GL_TRUE, &frustum.ProjectionMatrix()[0][0]);
}

void ModuleRender::ModelTransform(unsigned programUsed) {
	math::float4x4 model = math::float4x4::identity;
	int modelLocation = glGetUniformLocation(programUsed, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &model[0][0]);
}

// We avoid the viewMatrix calc if camera not moving
void ModuleRender::LookAt(math::float3& cameraPos, math::float3& target) {
	math::float3 front(target - cameraPos); front.Normalize();
	// We are not implementing roll, so we will calculate the up again mantaining the verticalitiy
	math::float3 side(front.Cross(App->camera->up)); side.Normalize();
	math::float3 up(side.Cross(front));

	viewMatrix[0][0] = side.x; viewMatrix[0][1] = side.y; viewMatrix[0][2] = side.z;
	viewMatrix[1][0] = up.x; viewMatrix[1][1] = up.y; viewMatrix[1][2] = up.z;
	viewMatrix[2][0] = -front.x; viewMatrix[2][1] = -front.y; viewMatrix[2][2] = -front.z;
	viewMatrix[0][3] = -side.Dot(cameraPos); viewMatrix[1][3] = -up.Dot(cameraPos); viewMatrix[2][3] = front.Dot(cameraPos);
	viewMatrix[3][0] = 0.0f; viewMatrix[3][1] = 0.0f; viewMatrix[3][2] = 0.0f; viewMatrix[3][3] = 1.0f;
}

// Initialization
void ModuleRender::InitFrustum() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
}

void ModuleRender::InitSDL() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);
	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);
}

void ModuleRender::InitOpenGL() {
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glViewport(0, 0, App->window->width, App->window->height);
}

void ModuleRender::DrawGUI() {

	ImGui::SliderFloat3("Background color", App->renderer->bgColor, 0.0f, 1.0f);

}
