#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "QuadTreeChimera.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "SDL.h"
#include "GL/glew.h"
#include "debugdraw.h"
#include "imgui_internal.h"
#include "Math/float4x4.h"

ModuleRender::ModuleRender() { }

// Destructor
ModuleRender::~ModuleRender() { }

// Called before render is available
bool ModuleRender::Init() {
	LOG("Creating Renderer context");

	InitSDL();
	glewInit();
	InitOpenGL();

	if (vsyncEnabled && SDL_GL_SetSwapInterval(1) < 0) {
		LOG("Error: VSync couldn't be enabled \n %s", SDL_GetError());
	}

	App->program->LoadPrograms();
	GenerateBlockUniforms();
	GenerateFallBackMaterial();

	return true;
}

update_status ModuleRender::PreUpdate() {
	BROFILER_CATEGORY("RenderPreUpdate()", Profiler::Color::AliceBlue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update() {
	BROFILER_CATEGORY("RenderUpdate()", Profiler::Color::Aqua);
	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->sceneCamera->fbo);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, App->camera->sceneCamera->wireFrame);
	SetProjectionMatrix(App->camera->sceneCamera);
	SetViewMatrix(App->camera->sceneCamera);

	if (frustCulling && App->camera->selectedCamera != nullptr) {
		DrawMeshes(App->camera->selectedCamera);
	} else {
		DrawMeshes(App->camera->sceneCamera);
	}

	DrawDebugData(App->camera->sceneCamera);

	if (App->camera->selectedCamera != nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, App->camera->selectedCamera->fbo);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, App->camera->selectedCamera->wireFrame);
		SetProjectionMatrix(App->camera->selectedCamera);
		SetViewMatrix(App->camera->selectedCamera);
		DrawMeshes(App->camera->selectedCamera);
		DrawDebugData(App->camera->selectedCamera);
	}

 	if (showQuad && App->camera->quadCamera != nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, App->camera->quadCamera->fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SetProjectionMatrix(App->camera->quadCamera);
		SetViewMatrix(App->camera->quadCamera);
		/*DrawMeshes(App->camera->quadCamera);*/
		DrawDebugData(App->camera->quadCamera);
	}

	//So we exclude the rest of the quad rendered background of the color selected
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate() {
	BROFILER_CATEGORY("RenderPostUpdate()", Profiler::Color::Orchid);
	App->editor->RenderGUI();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

void ModuleRender::DrawDebugData(ComponentCamera* camera) const {
	BROFILER_CATEGORY("DrawDebugData()", Profiler::Color::Orange);
	if (camera->debugDraw == false) return;

	//TODO: probably we can render every frustum and paint the active in green
	if (App->camera->selectedCamera != nullptr) {
		dd::frustum((App->camera->selectedCamera->frustum.ProjectionMatrix() * App->camera->selectedCamera->frustum.ViewMatrix()).Inverted(), dd::colors::GreenYellow);
	}

	for (auto& cam : App->camera->gameCameras) {
		dd::frustum((cam->frustum.ProjectionMatrix() * cam->frustum.ViewMatrix()).Inverted(), dd::colors::IndianRed);
	}

	//Grid and axis debug
	dd::xzSquareGrid(-42000.0f, 42000.0f, 0.0f, 100.0f, math::float3(0.65f, 0.65f, 0.65f));
	dd::axisTriad(math::float4x4::identity, 10.0f, 100.0f, 0, true);

	if (showQuad) {
		PrintQuadNode(App->scene->quadTree->root);
	}

	if (showRayCast) {
		PrintRayCast();
	}

	App->debug->Draw(camera, camera->fbo, camera->screenWidth, camera->screenHeight);
}

void ModuleRender::SetViewMatrix(ComponentCamera* camera) const {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(math::float4x4), sizeof(math::float4x4), &camera->GetViewMatrix()[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ModuleRender::SetProjectionMatrix(ComponentCamera* camera) const {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(math::float4x4), &camera->GetProjectionMatrix()[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ModuleRender::GenerateBlockUniforms() {
	unsigned uniformBlockIndexDefault = glGetUniformBlockIndex(App->program->colorProgram, "Matrices");
	unsigned uniformBlockIndexTexture = glGetUniformBlockIndex(App->program->textureProgram, "Matrices");
	unsigned uniformBlockIndexBlinn = glGetUniformBlockIndex(App->program->blinnProgram, "Matrices");

	glUniformBlockBinding(App->program->colorProgram, uniformBlockIndexDefault, 0);
	glUniformBlockBinding(App->program->textureProgram, uniformBlockIndexTexture, 0);
	glUniformBlockBinding(App->program->blinnProgram, uniformBlockIndexBlinn, 0);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(math::float4x4), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(math::float4x4));
}

void ModuleRender::InitSDL() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);
	SDL_GetWindowSize(App->window->window, &App->window->width, &App->window->height);
}

void ModuleRender::InitOpenGL() const {
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glViewport(0, 0, App->window->width, App->window->height);
}

void ModuleRender::GenerateFallBackMaterial() {
	char fallbackImage[3] = { GLubyte(255), GLubyte(255), GLubyte(255) };

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &fallback);
	glBindTexture(GL_TEXTURE_2D, fallback);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, fallbackImage);
}

void ModuleRender::DrawMeshes(ComponentCamera* camera) {
	BROFILER_CATEGORY("DrawMeshes()", Profiler::Color::Gold);
	for (std::list<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		if ((*it)->enabled) {
			if (frustCulling) {
				if (frustumCullingType == 1) {
					CullingFromQuadTree(camera, *it);
				} else {
					CullingFromFrustum(camera, *it);
				}
			} else {
					DrawWithoutCulling(*it);
			}
		}
	}
}

void ModuleRender::DrawWithoutCulling(ComponentMesh* mesh) const {
	if (mesh->goContainer->transform != nullptr) {
		if (App->scene->goSelected == mesh->goContainer) {
			dd::aabb(mesh->goContainer->bbox.minPoint, mesh->goContainer->bbox.maxPoint, math::float3(0.0f, 1.0f, 0.0f), true);
		}

		unsigned program = App->program->blinnProgram;
		ComponentMaterial* compMat = (ComponentMaterial*)mesh->goContainer->GetComponent(ComponentType::MATERIAL);

		glUseProgram(program);

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &mesh->goContainer->transform->GetGlobalTransform()[0][0]);
	
		mesh->Draw(program, compMat);

		glUseProgram(0);
	}
}

void ModuleRender::CullingFromFrustum(ComponentCamera* camera, ComponentMesh* mesh) const {
	if (App->camera->selectedCamera != nullptr && !camera->frustum.Intersects(mesh->goContainer->bbox)) {
		dd::aabb(mesh->goContainer->bbox.minPoint, mesh->goContainer->bbox.maxPoint, math::float3(0.0f, 1.0f, 0.0f), true);
	} else {
		DrawWithoutCulling(mesh);
	}
}

void ModuleRender::CullingFromQuadTree(ComponentCamera* camera, ComponentMesh* mesh) {
	quadGOCollided.clear();
	App->scene->quadTree->CollectIntersections(quadGOCollided, camera->frustum);

	for (std::list<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		if (!(*it)->goContainer->staticGo && (*it)->mesh.verticesNumber > 0 && camera->frustum.Intersects((*it)->goContainer->bbox)) {
			quadGOCollided.push_back((*it)->goContainer);
		}
	}

	for (std::vector<GameObject*>::iterator it = quadGOCollided.begin(); it != quadGOCollided.end(); ++it){
		if ((*it)->enabled) {
			DrawWithoutCulling((ComponentMesh*)(*it)->GetComponent(ComponentType::MESH));
		}
	}
}

bool ModuleRender::CleanUp() {
	glDeleteBuffers(1, &ubo);
	return true;
}

void ModuleRender::PrintQuadNode(QuadTreeNode* quadNode) const {

	if (quadNode->childs[0] != nullptr) {
		for (int i = 0; i < 4; ++i) {
			PrintQuadNode(quadNode->childs[i]);
		}
	}

	dd::aabb(quadNode->aabb.minPoint, quadNode->aabb.maxPoint, dd::colors::Yellow);
}

void ModuleRender::PrintRayCast() const {
	dd::line(App->camera->rayCast.a, App->camera->rayCast.b, math::float3(0.0f, 0.0f, 1.0f));
}

void ModuleRender::DrawImGuizmo(float width, float height, float winPosX, float winPosY) {
	ImGuizmo::SetRect(winPosX, winPosY, width, height);
	ImGuizmo::SetDrawlist();

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	ImGui::SetCursorPos({ 20,30 });

	if (App->scene->goSelected != nullptr) {
		mCurrentGizmoOperation = (ImGuizmo::OPERATION)imGuizmoOp;
		mCurrentGizmoMode = (ImGuizmo::MODE)imGuizmoMode;

		ImGuizmo::Enable(!App->scene->goSelected->staticGo);

		ComponentTransform* transform = (ComponentTransform*)App->scene->goSelected->GetComponent(ComponentType::TRANSFORM);

		math::float4x4 model = App->scene->goSelected->transform->GetGlobalTransform();
		math::float4x4 viewScene = App->camera->sceneCamera->GetViewMatrix();
		math::float4x4 projectionScene = App->camera->sceneCamera->GetProjectionMatrix();

		ImGuizmo::SetOrthographic(false);

		model.Transpose();
		ImGuizmo::Manipulate((float*)&viewScene, (float*)&projectionScene, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&model, NULL, NULL, NULL, NULL);

		if (ImGuizmo::IsUsing()) {
			model.Transpose();
			App->scene->goSelected->transform->SetGlobalTransform(model);
		}
	}
}