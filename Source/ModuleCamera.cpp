#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleTime.h"

ModuleCamera::ModuleCamera() { }

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init() {
	UpdatePitchYaw();
	App->renderer->LookAt(cameraPos, (cameraPos + front));
	return true;
}

update_status ModuleCamera::PreUpdate() {

	if (App->editor->SceneFocused()) {

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			CameraMovementKeyboard();
			SDL_ShowCursor(SDL_DISABLE);
			RotateCamera(App->input->GetMousePosition());
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
			SDL_ShowCursor(SDL_ENABLE);
			firstMouse = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
			FocusSelectedObject();
		}

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				SDL_ShowCursor(SDL_DISABLE);
				RotateCamera(App->input->GetMousePosition(), true);
			} else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				SDL_ShowCursor(SDL_ENABLE);
				firstMouse = true;
			}
		} else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP) {
			SDL_ShowCursor(SDL_ENABLE);
		}

		Zoom();

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
			cameraSpeed = cameraSpeed * 2;
			rotationSpeed = rotationSpeed * 2;
		} else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
			cameraSpeed = cameraSpeed * 0.5f;
			rotationSpeed = rotationSpeed * 0.5f;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp() {

	return true;
}

void ModuleCamera::MoveCamera(CameraMovement cameraSide) {

	float normMoveSpeed = cameraSpeed * App->time->realDeltaTime;

	switch (cameraSide) {
		case Upwards:
			cameraPos += up * normMoveSpeed;
			break;
		case Downwards:
			cameraPos -= up * normMoveSpeed;
			break;
		case Left:
			cameraPos += up.Cross(front) * normMoveSpeed;
			break;
		case Right:
			cameraPos -= up.Cross(front) * normMoveSpeed;
			break;
		case Forward:
			cameraPos += front * normMoveSpeed;
			break;
		case Backwards:
			cameraPos -= front * normMoveSpeed;
			break;
		default:
			break;
	}

	App->renderer->LookAt(cameraPos, (cameraPos + front));
}

void ModuleCamera::RotateCamera(const fPoint& mousePosition, bool orbit) {

	if (firstMouse) {
		lastX = mousePosition.x;
		lastY = mousePosition.y;
		firstMouse = false;
	}

	float xoffset = mousePosition.x - lastX;
	float yoffset = lastY - mousePosition.y;
	lastX = mousePosition.x;
	lastY = mousePosition.y;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	pitch = math::Clamp(pitch, -80.0f, 80.0f);

	if (orbit) {
		// TODO: not orbiting correctly
		math::float3 cameraTarget(cameraPos + front * 5);
		float distanceToOrbit = cameraTarget.Length();

		cameraPos.x = sin(math::DegToRad(yaw)) * cos(math::DegToRad(pitch)) * distanceToOrbit;
		cameraPos.y = sin(math::DegToRad(pitch)) * distanceToOrbit;
		cameraPos.z = -cos(math::DegToRad(yaw)) * cos(math::DegToRad(pitch)) * distanceToOrbit;
		front = (cameraTarget - cameraPos).Normalized();
	} else {
		// We are facing -z so we invert the x/z trigonometry and set the yaw to 0
		math::float3 rotation;
		rotation.x = sin(math::DegToRad(yaw)) * cos(math::DegToRad(pitch));
		rotation.y = sin(math::DegToRad(pitch)) * cos(math::DegToRad(pitch));
		rotation.z = -cos(math::DegToRad(yaw)) * cos(math::DegToRad(pitch));
		front = rotation.Normalized();
	}

	App->renderer->LookAt(cameraPos, (cameraPos + front));
}

void ModuleCamera::Zoom() {

	const int wheelSlide = App->input->GetMouseWheel();
	if (wheelSlide != 0) {
		float zoomValue = App->renderer->frustum.verticalFov + -wheelSlide * 20.0f * App->time->realDeltaTime;
		float newAngleFov = math::Clamp(zoomValue, math::DegToRad(minFov), math::DegToRad(maxFov));
		App->renderer->frustum.verticalFov = newAngleFov;
		App->renderer->frustum.horizontalFov = 2.0f * atanf(tanf(newAngleFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}

}

void ModuleCamera::FocusSelectedObject() {

	if (goSelected == nullptr) {
		front = (cameraPos - math::float3(0.0f, 0.0f, 0.0f)).Normalized();
	} else {
		// If GO contains an AABB, push the camera outside if needed and focus its center
		Component* component = goSelected->GetComponent(ComponentType::MESH);
		if (component != nullptr) {
			ComponentMesh* meshComponent = (ComponentMesh*)component;

			// Closest point returns the same point if the selected object is inside
			while (meshComponent->bbox.ClosestPoint(cameraPos).Equals(cameraPos)) {
				cameraPos = cameraPos.Mul(2.0f);
			}

			front = (meshComponent->bbox.CenterPoint() - cameraPos).Normalized();
		} else {
			component = goSelected->GetComponent(ComponentType::TRANSFORM);
			if (component != nullptr) {
				ComponentTransform* transformComponent = (ComponentTransform*)component;
				front = (transformComponent->position - cameraPos).Normalized();
			}
		}
		
		// if GO does NOT contains an AABB but does have a transform, focus its center
	}


	App->renderer->LookAt(cameraPos, (cameraPos + front));
	UpdatePitchYaw();
}

void ModuleCamera::UpdatePitchYaw() {
	pitch = -math::RadToDeg(sinf(-front.y));
	yaw = math::RadToDeg(atan2f(front.z, front.x)) + 90.0f;

	if (math::IsNan(pitch))
		pitch = 0.0f;

	if (math::IsNan(yaw))
		yaw = 0.0f;
}

void ModuleCamera::CameraMovementKeyboard() {
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		MoveCamera(Upwards);
	} else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		MoveCamera(Downwards);
	} else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		MoveCamera(Left);
	} else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		MoveCamera(Right);
	} else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		MoveCamera(Forward);
	} else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		MoveCamera(Backwards);
	} 
}

void ModuleCamera::DrawGUI() {
	ImGui::Text("Position "); ImGui::SameLine();
	ImGui::Text("X: %.2f", cameraPos.x, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Y: %.2f", cameraPos.y, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Z: %.2f", cameraPos.z, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Forward "); ImGui::SameLine();
	ImGui::Text("X: %.2f", front.x, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Y: %.2f", front.y, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Z: %.2f", front.z, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Rotation "); ImGui::SameLine();
	ImGui::Text("Pitch: %.2f", pitch, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
	ImGui::Text("Yaw: %.2f", yaw, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();

	ImGui::Separator();
	ImGui::InputFloat("Movement Speed", &cameraSpeed, 1.0f, 100.0f);
	ImGui::InputFloat("Rotation Speed", &rotationSpeed, 1.0f, 100.0f);
	ImGui::InputFloat("Mouse sensitivity", &mouseSensitivity, 1.0f, 100.0f);
	
	float fov = math::RadToDeg(App->renderer->frustum.verticalFov);
	if (ImGui::SliderFloat("FOV", &fov, 40, 120)) {
		App->renderer->frustum.verticalFov = math::DegToRad(fov);
		App->renderer->frustum.horizontalFov = 2.f * atanf(tanf(App->renderer->frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}

	ImGui::InputFloat("zNear", &App->renderer->frustum.nearPlaneDistance, 5, 50);
	ImGui::InputFloat("zFar", &App->renderer->frustum.farPlaneDistance, 5, 50);
}
