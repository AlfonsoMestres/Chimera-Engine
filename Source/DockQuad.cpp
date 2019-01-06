#include "DockQuad.h"
#include "Application.h"
#include "ModuleRender.h"

DockQuad::DockQuad() { }

DockQuad::~DockQuad() { }

void DockQuad::Draw() {

	if (!ImGui::Begin("QuadTree", &enabled)) {
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Draw", &App->renderer->showQuad);

	//TODO: render a preview of the upper side from the quadtree

	focus = ImGui::IsWindowFocused();
	ImGui::End();
}

bool DockQuad::IsFocused() const { return focus; }
