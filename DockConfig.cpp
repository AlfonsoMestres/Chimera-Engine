#include "DockConfig.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

DockConfig::DockConfig() : fps(LOGSSIZE), ms(LOGSSIZE), mem(LOGSSIZE),
gameFps(LOGSSIZE), gameMs(LOGSSIZE) { }

DockConfig::~DockConfig() { }

void DockConfig::Draw() {
	if (!ImGui::Begin("Configuration", &enabled, ImGuiWindowFlags_NoFocusOnAppearing)) {
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Application")) {
		ImGui::PushItemWidth(200.0f);
		static int framerateCap = App->framerateCap;
		if (ImGui::SliderInt("MaxFPS", &framerateCap, 1, 120)) {
			App->framerateCap = framerateCap;
		}
		ImGui::PopItemWidth();

		if (ImGui::Checkbox("Vsync", &App->renderer->vsyncEnabled)) {
			if (App->renderer->vsyncEnabled) {
				SDL_GL_SetSwapInterval(1);
			}
			else {
				SDL_GL_SetSwapInterval(0);
			}
		}

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, title, 0.0f, 120.0f, ImVec2(0, 80));

		sprintf_s(title, 25, "Milliseconds %0.1f", ms[ms.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(0, 80));

		sMStats stats = m_getMemoryStatistics();
		AddMemory((float)stats.totalReportedMemory);

		ImGui::PlotHistogram("##memory", &mem[0], mem.size(), 0, "Memory Consumption (Bytes)", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(0, 80));

		ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
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

	if (ImGui::CollapsingHeader("Window")) {
		App->window->DrawGUI();
	}

	ImGui::End();
}

void DockConfig::AddFps(float fpsVal, float msVal) {
	fps.insert(fps.begin(), fpsVal);
	ms.insert(ms.begin(), msVal);

	if (fps.size() > LOGSSIZE) {
		fps.pop_back();
	}

	if (ms.size() > LOGSSIZE) {
		ms.pop_back();
	}

}

void DockConfig::AddMemory(float memVal) {

	for (unsigned i = 0u; i < LOGSSIZE - 1; ++i) {
		mem[i] = mem[i + 1];
	}

	mem[LOGSSIZE - 1] = memVal;
}
