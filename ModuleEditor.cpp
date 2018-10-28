#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "SDL.h"

static void ShowMenuBar();
static void ShowAbout();
static void ShowHardware();
static void ShowSceneConfig();

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init() {
	const char* glsl_version = "#version 130";

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
																//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
																//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleEditor::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{

	ImGui::ShowDemoWindow();
	ShowMenuBar();

	if (showAboutMenu) {
		ShowAbout();
	} 
	
	if (showHardwareMenu){
		ShowHardware();
	}

	if (showSceneConfig){
		ShowSceneConfig();
	}

	if (requestedExit)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

static void ShowMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) { App->editor->requestedExit = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Configuration")) { App->editor->showSceneConfig = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("Hardware")) { App->editor->showHardwareMenu = true; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About")) { App->editor->showAboutMenu = true; }
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}

static void ShowAbout() {
	
	const char* MITLicense = "Copyright 2018 - QTEngine \n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions";

	ImGui::Begin("About", &App->editor->showAboutMenu);

	ImGui::Text(TITLE);
	ImGui::Text("C++/C engine for game development");
	ImGui::Separator();
	ImGui::Text("Libraries:");
	if (ImGui::MenuItem("SDL v2.0.8")) { ShellExecute(0, 0, "https://www.libsdl.org/index.php", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("Glew v2.1.0")) { ShellExecute(0, 0, "http://glew.sourceforge.net/", 0, 0, SW_SHOW); }
	if (ImGui::MenuItem("ImGui v1.66")) { ShellExecute(0, 0, "https://github.com/ocornut/imgui/tree/docking", 0, 0, SW_SHOW); }
	ImGui::Separator();
	ImGui::Text("Authors:");
	if (ImGui::MenuItem("Alfonso Mestres")) { ShellExecute(0, 0, "https://github.com/AlfonsoMestres/", 0, 0, SW_SHOW); }
	ImGui::Separator();
	ImGui::TextWrapped(MITLicense);
	ImGui::End();
}

static void ShowHardware() {
	ImGui::Begin("Hardware specs", &App->editor->showHardwareMenu);
	ImGui::Text("CPU Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "%d", SDL_GetCPUCount());
	ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "%d", SDL_GetSystemRAM());
	ImGui::End();
}

static void ShowSceneConfig() {
	ImGui::Begin("Camera", &App->editor->showSceneConfig);

	float forward[3] = { App->camera->forw.x, App->camera->forw.y, App->camera->forw.z };
	ImGui::InputFloat3("Front", forward, "%.3f");
	float up[3] = { App->camera->upw.x, App->camera->upw.y, App->camera->upw.z };
	ImGui::InputFloat3("Up", up, "%.3f");
	float eye[3] = { App->camera->eye.x, App->camera->eye.y, App->camera->eye.z };
	ImGui::InputFloat3("Position", eye, "%.3f");
	ImGui::SliderFloat("Mov Speed", &App->camera->cameraSpeed, 0.25f, 3.0f);
	ImGui::SliderFloat("Rot Speed", &App->camera->rotationSpeed, 0.25f, 2.0f);
	ImGui::Separator();
	ImGui::SliderFloat("Near Plane", &App->camera->frustum.nearPlaneDistance, 0.1f, App->camera->frustum.farPlaneDistance);
	ImGui::SliderFloat("Far Plane", &App->camera->frustum.farPlaneDistance, 0.1f, 1000.0f);
	// TODO: care with this, its not working properly
	ImGui::SliderFloat("Hotizontal FOV", &App->camera->frustum.horizontalFov, 0.01f, 1.0f);
	ImGui::SliderFloat("Vertical FOV", &App->camera->frustum.verticalFov, 0.01f, 1.0f);
	ImGui::InputFloat("AspectRatio", &App->camera->screenRatio, 0, 0, "%.3f");
	ImGui::Separator();
	ImGui::SliderFloat3("Background color", App->renderer->bgColor, 0.0f, 1.0f);
	ImGui::End();
}

void ModuleEditor::HandleInputs(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}
