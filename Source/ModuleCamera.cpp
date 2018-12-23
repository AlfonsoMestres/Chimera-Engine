#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleTime.h"

ModuleCamera::ModuleCamera() { }

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {

	sceneCamera = new ComponentCamera(App->scene->root);
	sceneCamera->cameraPosition = math::float3(0.0f, 20.0f, 30.0f);
	sceneCamera->InitFrustum();
	sceneCamera->debugDraw = true;

	return true;
}

update_status ModuleCamera::PreUpdate() {

	if (App->editor->SceneFocused()) {

		MovementSpeed();

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			Move();
			SDL_ShowCursor(SDL_DISABLE);
			sceneCamera->Rotate(sceneCamera->rotationSpeed * App->input->GetMouseMotion().x, sceneCamera->rotationSpeed * App->input->GetMouseMotion().y);
		} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
			SDL_ShowCursor(SDL_ENABLE);
		}

		FocusSelectedObject();

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				SDL_ShowCursor(SDL_DISABLE);
				sceneCamera->Orbit(sceneCamera->rotationSpeed * App->input->GetMouseMotion().x, sceneCamera->rotationSpeed * App->input->GetMouseMotion().y);
			} else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				SDL_ShowCursor(SDL_ENABLE);
			}
		} else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP) {
			SDL_ShowCursor(SDL_ENABLE);
		}

		Zoom();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update() {

	if (selectedCamera != nullptr && selectedCamera->enabled) {
		selectedCamera->Update();
	}
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp() {

	return true;
}

void ModuleCamera::Zoom() {

	const int wheelSlide = App->input->GetMouseWheel();
	if (wheelSlide != 0) {
		float zoomValue = sceneCamera->frustum.verticalFov + -wheelSlide * 20.0f * App->time->realDeltaTime;
		float newAngleFov = math::Clamp(zoomValue, math::DegToRad(sceneCamera->minFov), math::DegToRad(sceneCamera->maxFov));
		sceneCamera->frustum.verticalFov = newAngleFov;
		sceneCamera->frustum.horizontalFov = 2.0f * atanf(tanf(newAngleFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}

}

void ModuleCamera::SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight) {
	selectedCamera->SetScreenNewScreenSize(newWidth, newHeight);
}

//TODO: change this to frustum properties, also make the camera to remain in the same location
void ModuleCamera::FocusSelectedObject() {

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->scene->goSelected != nullptr) {
		math::AABB& bbox = App->scene->goSelected->bbox;
		math::float3 HalfSize = bbox.HalfSize();
		float distX = HalfSize.x / tanf(sceneCamera->frustum.horizontalFov*0.5f);
		float distY = HalfSize.y / tanf(sceneCamera->frustum.verticalFov*0.5f);
		float camDist = MAX(distX, distY) + HalfSize.z; //camera distance from model

		math::float3 center = bbox.FaceCenterPoint(5);
		sceneCamera->frustum.pos = center + math::float3(0, 0, camDist);
		sceneCamera->frustum.front = -math::float3::unitZ;
		sceneCamera->frustum.up = math::float3::unitY;
	}

}

void ModuleCamera::MovementSpeed() {
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
		sceneCamera->cameraSpeed = sceneCamera->cameraSpeed * 2;
		sceneCamera->rotationSpeed = sceneCamera->rotationSpeed * 2;
	} else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		sceneCamera->cameraSpeed = sceneCamera->cameraSpeed * 0.5f;
		sceneCamera->rotationSpeed = sceneCamera->rotationSpeed * 0.5f;
	}
}

void ModuleCamera::Move() {

	float distance = 5.0f * App->time->realDeltaTime;
	float3 movement = float3::zero;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) {
		distance *= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_Q)) {
		movement += math::float3::unitY*distance;
	}
	if (App->input->GetKey(SDL_SCANCODE_E)) {
		movement -= math::float3::unitY*distance;
	}
	if (App->input->GetKey(SDL_SCANCODE_S)) {
		movement -= sceneCamera->frustum.front*distance;
	}
	if (App->input->GetKey(SDL_SCANCODE_W)) {
		movement += sceneCamera->frustum.front*distance;
	}
	if (App->input->GetKey(SDL_SCANCODE_A)) {
		movement -= sceneCamera->frustum.WorldRight()*distance;
	}
	if (App->input->GetKey(SDL_SCANCODE_D)) {
		movement += sceneCamera->frustum.WorldRight()*distance;
	}

	sceneCamera->frustum.Translate(movement);
}

void ModuleCamera::DrawGUI() {

	ImGui::Checkbox("Debug", &sceneCamera->debugDraw);

	ImGui::Text("Position "); ImGui::SameLine();
	ImGui::Text("X: %.2f", sceneCamera->cameraPosition.x, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Y: %.2f", sceneCamera->cameraPosition.y, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Z: %.2f", sceneCamera->cameraPosition.z, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Forward "); ImGui::SameLine();
	ImGui::Text("X: %.2f", sceneCamera->cameraFront.x, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Y: %.2f", sceneCamera->cameraFront.y, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Z: %.2f", sceneCamera->cameraFront.z, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Rotation "); ImGui::SameLine();
	ImGui::Text("Pitch: %.2f", sceneCamera->pitch, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Yaw: %.2f", sceneCamera->yaw, ImGuiInputTextFlags_ReadOnly); 
	
	float fov = math::RadToDeg(sceneCamera->frustum.verticalFov);
	if (ImGui::SliderFloat("FOV", &fov, 40, 120)) {
		sceneCamera->frustum.verticalFov = math::DegToRad(fov);
		sceneCamera->frustum.horizontalFov = 2.f * atanf(tanf(sceneCamera->frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}

	ImGui::InputFloat("zNear", &sceneCamera->frustum.nearPlaneDistance, 5, 50);
	ImGui::InputFloat("zFar", &sceneCamera->frustum.farPlaneDistance, 5, 50);
}
