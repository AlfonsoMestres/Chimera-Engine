#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "SDL.h"

static void ShowMenuBar();
static void ShowAbout();
static void ShowHardware();

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

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}
