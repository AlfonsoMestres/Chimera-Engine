#include "Globals.h"
#include <windows.h>
#include "DockAbout.h"
#include "SDL_version.h"
#include "IL/il.h"
#include "assimp/version.h"
#include "GL/glew.h"

DockAbout::DockAbout() {
	enabled = false;
}

DockAbout::~DockAbout() { }

void DockAbout::Draw() {

	if (!ImGui::Begin("About", &enabled, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), TITLE);

	ImGui::Text("Description:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), DESCRIPTION);
	ImGui::Separator();

	ImGui::Text("Author:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), AUTHOR);
	if (ImGui::SmallButton("Github")) {
		ShellExecuteA(NULL, "open", REPOSITORY, NULL, NULL, SW_SHOWNORMAL);
	};

	ImGui::Separator();

	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);

	if (ImGui::TreeNode("Libraries")) {
		ImGui::BulletText("SDL2 (version %d.%d.%d)", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
		ImGui::BulletText("ImgUI (version %s)", ImGui::GetVersion());
		ImGui::BulletText("MathGeoLib (version 1.5)");
		ImGui::BulletText("Glew (version %s)", glewGetString(GLEW_VERSION));
		ImGui::BulletText("Assimp (version %d.%d.%d)", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::SmallButton("License")) {
		ShellExecuteA(NULL, "open", LICENSE, NULL, NULL, SW_SHOWNORMAL);
	};

	ImGui::End();
}