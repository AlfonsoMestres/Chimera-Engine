#include "DockTime.h"
#include "ModuleTime.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"

DockTime::DockTime() { }

DockTime::~DockTime() { }

void DockTime::Draw() {

	ImGui::Begin("Time", &enabled, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(-(App->window->width - size.x) * 0.5f, -(App->window->height - size.y) * 0.5f));

	ImGui::BeginMenuBar();

	if (App->time->gameState == GameState::STOP){
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0.5f, 0.5f, 0.7f });
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.3f, 0.5f, 0.3f, 0.7f });
	}

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.2f });

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 85) / 2);
	if (ImGui::ArrowButton("Play", ImGuiDir_Right)) {
		if (App->time->gameState == GameState::STOP) {
			App->time->StartGameClock();
			App->scene->SaveScene();
		} else {
			App->time->StopGameClock();
			App->scene->LoadScene();
		}
	}

	ImGui::PopStyleColor(2);
	if (App->time->gameState == GameState::PAUSE) {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.3f, 0.5f, 0.3f, 0.7f });
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0.5f, 0.5f, 0.7f });
	}

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.2f });
	ImGui::SameLine();

	if (ImGui::Button("||", { 23,19 })) {
		if (App->time->gameState == GameState::RUN) {
			App->time->PauseGameClock(true);
		} else if (App->time->gameState == GameState::PAUSE) {
			App->time->PauseGameClock(false);
		}
	}

	ImGui::PopStyleColor(2);
	ImGui::SameLine();

	if (App->time->gameState == GameState::PAUSE) {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.3f, 0.5f, 0.3f, 0.7f });
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0.5f, 0.5f, 0.7f });
	}

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 1.0f, 1.0f, 0.2f });
	if (App->time->nextFrame) {
		App->time->nextFrame = false;
		App->time->PauseGameClock(true);
	}

	if (ImGui::Button("->", { 23,19 }) && App->time->gameState == GameState::PAUSE) {
		App->time->nextFrame = true;
		App->time->PauseGameClock(false);
	}

	ImGui::PopStyleColor(2);
	ImGui::EndMenuBar();

	ImGui::End();

}
