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
	selectedCamera = sceneCamera;

	selectedCamera->cameraPosition = math::float3(0.0f, 3.0f, 40.0f);
	selectedCamera->InitFrustum();
	sceneCamera->UpdatePitchYaw();
	/*sceneCamera->LookAt();*/

	return true;
}

update_status ModuleCamera::PreUpdate() {

	if (App->editor->SceneFocused()) {

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
			CameraMovementKeyboard();
			SDL_ShowCursor(SDL_DISABLE);
			RotateCamera(App->input->GetMousePosition());
		} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
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

//TODO: ComponentCamera to ModuleCamera
void ModuleCamera::MoveCamera(CameraMovement cameraSide) {

	float normMoveSpeed = sceneCamera->cameraSpeed * App->time->realDeltaTime;

	switch (cameraSide) {
		case Upwards:
			sceneCamera->cameraPosition += sceneCamera->cameraUp * normMoveSpeed;
			break;
		case Downwards:
			sceneCamera->cameraPosition -= sceneCamera->cameraUp * normMoveSpeed;
			break;
		case Left:
			sceneCamera->cameraPosition += sceneCamera->cameraUp.Cross(sceneCamera->cameraFront) * normMoveSpeed;
			break;
		case Right:
			sceneCamera->cameraPosition -= sceneCamera->cameraUp.Cross(sceneCamera->cameraFront) * normMoveSpeed;
			break;
		case Forward:
			sceneCamera->cameraPosition += sceneCamera->cameraFront * normMoveSpeed;
			break;
		case Backwards:
			sceneCamera->cameraPosition -= sceneCamera->cameraFront * normMoveSpeed;
			break;
		default:
			break;
	}

	sceneCamera->LookAt(sceneCamera->cameraPosition, (sceneCamera->cameraPosition + sceneCamera->cameraFront));
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

	sceneCamera->yaw += xoffset;
	sceneCamera->pitch += yoffset;

	sceneCamera->pitch = math::Clamp(sceneCamera->pitch, -80.0f, 80.0f);

	if (orbit) {
		// TODO: not orbiting correctly
		math::float3 cameraTarget(sceneCamera->cameraPosition + sceneCamera->cameraFront * 5);
		float distanceToOrbit = cameraTarget.Length();

		sceneCamera->cameraPosition.x = sin(math::DegToRad(sceneCamera->yaw)) * cos(math::DegToRad(sceneCamera->pitch)) * distanceToOrbit;
		sceneCamera->cameraPosition.y = sin(math::DegToRad(sceneCamera->pitch)) * distanceToOrbit;
		sceneCamera->cameraPosition.z = -cos(math::DegToRad(sceneCamera->yaw)) * cos(math::DegToRad(sceneCamera->pitch)) * distanceToOrbit;
		sceneCamera->cameraFront = (cameraTarget - sceneCamera->cameraPosition).Normalized();
	} else {
		// We are facing -z so we invert the x/z trigonometry and set the yaw to 0
		math::float3 rotation;
		rotation.x = sin(math::DegToRad(sceneCamera->yaw)) * cos(math::DegToRad(sceneCamera->pitch));
		rotation.y = sin(math::DegToRad(sceneCamera->pitch)) * cos(math::DegToRad(sceneCamera->pitch));
		rotation.z = -cos(math::DegToRad(sceneCamera->yaw)) * cos(math::DegToRad(sceneCamera->pitch));
		sceneCamera->cameraFront = rotation.Normalized();
	}

	sceneCamera->LookAt(sceneCamera->cameraPosition, (sceneCamera->cameraPosition + sceneCamera->cameraFront));
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

void ModuleCamera::FocusSelectedObject() {

	if (goSelected == nullptr) {
		sceneCamera->cameraFront = (sceneCamera->cameraPosition - math::float3(0.0f, 0.0f, 0.0f)).Normalized();
	} else {
		// If GO contains an AABB, push the camera outside if needed and focus its center
		Component* component = goSelected->GetComponent(ComponentType::MESH);
		if (component != nullptr) {
			ComponentMesh* meshComponent = (ComponentMesh*)component;

			// Closest point returns the same point if the selected object is inside
			while (meshComponent->bbox.ClosestPoint(sceneCamera->cameraPosition).Equals(sceneCamera->cameraPosition)) {
				sceneCamera->cameraPosition = sceneCamera->cameraPosition.Mul(2.0f);
			}

			sceneCamera->cameraFront = (meshComponent->bbox.CenterPoint() - sceneCamera->cameraPosition).Normalized();
		} else {
			component = goSelected->GetComponent(ComponentType::TRANSFORM);
			if (component != nullptr) {
				ComponentTransform* transformComponent = (ComponentTransform*)component;
				sceneCamera->cameraFront = (transformComponent->position - sceneCamera->cameraPosition).Normalized();
			}
		}
		
		// if GO does NOT contains an AABB but does have a transform, focus its center
	}


	sceneCamera->LookAt(sceneCamera->cameraPosition, (sceneCamera->cameraPosition + sceneCamera->cameraFront));
	sceneCamera->UpdatePitchYaw();
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

//TODO: We need to handle the GameCameras and the SceneCam separately. We can't remove the 
void ModuleCamera::DrawGUI() {
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
	ImGui::Text("Yaw: %.2f", sceneCamera->yaw, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();

	ImGui::Separator();
	ImGui::InputFloat("Movement Speed", &sceneCamera->cameraSpeed, 1.0f, 100.0f);
	ImGui::InputFloat("Rotation Speed", &sceneCamera->rotationSpeed, 1.0f, 100.0f);
	ImGui::InputFloat("Mouse sensitivity", &mouseSensitivity, 1.0f, 100.0f);
	
	float fov = math::RadToDeg(sceneCamera->frustum.verticalFov);
	if (ImGui::SliderFloat("FOV", &fov, 40, 120)) {
		sceneCamera->frustum.verticalFov = math::DegToRad(fov);
		sceneCamera->frustum.horizontalFov = 2.f * atanf(tanf(sceneCamera->frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
	}

	ImGui::InputFloat("zNear", &sceneCamera->frustum.nearPlaneDistance, 5, 50);
	ImGui::InputFloat("zFar", &sceneCamera->frustum.farPlaneDistance, 5, 50);
}
