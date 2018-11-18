#include "DockConfig.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleModel.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL.h"

DockConfig::DockConfig() { }

DockConfig::~DockConfig() { }

void DockConfig::Draw() {
	if (!ImGui::Begin("Configuration", &enabled)) {
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Application")) {
		DrawFPSgraph();
	}

	if (ImGui::CollapsingHeader("Camera")) {
		App->camera->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Input")) {
		App->input->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Textures")) {
		App->textures->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Renderer")) {
		App->renderer->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Models")) {
		App->model->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Window")) {
		App->window->DrawGUI();
	}

	ImGui::End();
}

void DockConfig::DrawFPSgraph() const {
	float total = 0.0f;
	for (unsigned i = 0u; i < fps.size(); i++) {
		total += fps[i];
	}
	char avg[32];
	sprintf_s(avg, "%s%.2f", "avg:", total / fps.size());
	ImGui::PlotHistogram("FPS", &fps[0], fps.size(), 0, avg, 0.0f, 120.0f, ImVec2(0, 80));
}

void DockConfig::AddFps(float fpsVal) {
	fps.insert(fps.begin(), fpsVal);
	if (fps.size() > NUMFPS) {
		fps.pop_back();
	}
}
