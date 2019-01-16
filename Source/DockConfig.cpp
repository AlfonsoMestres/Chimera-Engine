#include "DockConfig.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

DockConfig::DockConfig() : fps(LOGSSIZE), ms(LOGSSIZE), mem(LOGSSIZE),
gameFps(LOGSSIZE), gameMs(LOGSSIZE) { }

DockConfig::~DockConfig() {
	fps.clear();
	ms.clear();
	mem.clear();
	gameFps.clear();
	gameMs.clear();
}

void DockConfig::Draw() {
	if (!ImGui::Begin("Configuration", &enabled, ImGuiWindowFlags_NoFocusOnAppearing)) {
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Application")) {

		ImGui::DragInt("Scale factor", &App->scene->scaleFactor);

		ImGui::PushItemWidth(200.0f);
		static int framerateCap = App->time->maxFps;

		// (ImGui::DragFloat3("Position", (float*)&position, 10.0f, -100000.f, 100000.f)
		if (ImGui::SliderInt("MaxFPS", &framerateCap, 1, 120)) {
			App->time->maxFps = framerateCap;
		}

		ImGui::PopItemWidth();

		if (ImGui::Checkbox("Vsync", &App->renderer->vsyncEnabled)) {
			if (App->renderer->vsyncEnabled) {
				SDL_GL_SetSwapInterval(1);
			} else {
				SDL_GL_SetSwapInterval(0);
			}
		}

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps[fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, title, 0.0f, 120.0f, ImVec2(0, 80));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms[ms.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(0, 80));

		/*sMStats stats = m_getMemoryStatistics();
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
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);*/
	}

	if (ImGui::CollapsingHeader("Time")) {
		char title[35];
		sprintf_s(title, 25, "Framerate %0.1f", gameFps[gameFps.size() - 1]);
		ImGui::PlotLines("##framerate", &gameFps[0], gameFps.size(), 0, title, 0.0f, 200.0f, ImVec2(300, 50));
		sprintf_s(title, 25, "Milliseconds %0.1f", gameMs[gameMs.size() - 1]);
		ImGui::PlotLines("##framerate", &gameMs[0], gameMs.size(), 0, title, 0.0f, 40.0f, ImVec2(300, 50));

		ImGui::SliderFloat("Time Scale", &App->time->gameTimeScale, 0.1f, 5.0f, "%0.1f");

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Time since App start: %f seconds", App->time->realTime);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Frames since App start: %u", App->time->realFrameCount);

		if (App->time->gameState == GameState::STOP) {
			ImGui::TextDisabled("Time since Game start: 0.0000000 seconds");
			ImGui::TextDisabled("Frames since Game start: 0");
		} else {
			ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.3f, 0.7f), "Time since Game start: %f seconds", App->time->gameTime);
			ImGui::TextColored(ImVec4(0.3f, 0.5f, 0.3f, 0.7f), "Frames since Game start: %u", App->time->totalFrames);
		}
	}

	if (ImGui::CollapsingHeader("Camera")) {
		App->camera->DrawGUI();
	}

	if (ImGui::CollapsingHeader("Textures")) {
		App->textures->DrawGUI();
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

void DockConfig::AddGameFps(float fpsVal, float msVal) {
	gameFps.insert(gameFps.begin(), fpsVal);
	gameMs.insert(gameMs.begin(), msVal);

	if (gameFps.size() > LOGSSIZE) {
		gameFps.pop_back();
	}

	if (gameMs.size() > LOGSSIZE) {
		gameMs.pop_back();
	}
}

void DockConfig::AddMemory(float memVal) {

	for (unsigned i = 0u; i < LOGSSIZE - 1; ++i) {
		mem[i] = mem[i + 1];
	}

	mem[LOGSSIZE - 1] = memVal;
}
