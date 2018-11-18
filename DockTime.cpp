#include "DockTime.h"

DockTime::DockTime() { }


DockTime::~DockTime() { }

void DockTime::Draw() {

	ImGui::ShowDemoWindow();

	ImGui::Begin("Time", &enabled, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(-(App->window->width - size.x) * 0.5f, -(App->window->height - size.y) * 0.5f));

	ImGui::BeginMenuBar();

	if (!App->gameModeEnabled)
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.95f,0.5f,0.0f,0.7f });
	else
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f,0.5f,0.95f,0.7f });

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f,1.0f,1.0f,0.2f });


	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 85) / 2);
	if (ImGui::ArrowButton("play", ImGuiDir_Right)) {
		gameTime = 0;

		if (!App->gameModeEnabled) {
			App->gameModeEnabled = true;
		} else {
			App->gameModeEnabled = false;
			App->gamePaused = false;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("||", { 23,19 }) && App->gameModeEnabled) {
		if (!App->gamePaused) {
			App->gamePaused = true;
		} else {
			App->gamePaused = false;
		}
	}

	ImGui::PopStyleColor(2);
	ImGui::SameLine();

	if (!App->gameModeEnabled)
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,1.0f,1.0f,0.2f });
	else
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f,0.5f,0.95f,0.7f });

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f,1.0f,1.0f,0.2f });
	if (App->gameModeEnabled && !App->gamePaused && step == true) {
		App->gamePaused = true;
		step = false;
	}

	if (ImGui::Button("->", { 23,19 }) && App->gameModeEnabled && App->gamePaused) {
		App->gamePaused = false;
		step = true;
	}
	ImGui::PopStyleColor(2);
	ImGui::EndMenuBar();

	ImGui::End();

}
