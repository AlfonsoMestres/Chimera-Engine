#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() {
	cameraPos = math::float3(0.0f, 0.0f, 3.0f);
	cameraFront = math::float3(0.0f, 0.0f, -1.0f);
	cameraUp = math::float3(0.0f, 1.0f, 0.0f);

	cameraSpeed = 17.0f;
	rotationSpeed = 65.0f;
	mouseSensitivity = 0.5f;

	yaw = -90.0f;
	pitch = 0.0f; 
	fov = 45.0f;
}

// Destructor
ModuleCamera::~ModuleCamera() {}

// Called before render is available
bool ModuleCamera::Init()
{
	InitFrustum();
	return true;
}

// Called every draw update
update_status ModuleCamera::PreUpdate()
{

	// This will have the target point as reference, we need to handle absolute values from the camera instead of the plane
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
	} else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		pitch += rotationSpeed * App->deltaTime;
		RotateCamera();
	} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		pitch -= rotationSpeed * App->deltaTime;
		RotateCamera();
	} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		yaw -= rotationSpeed * App->deltaTime;
		RotateCamera();
	} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		yaw += rotationSpeed * App->deltaTime;
		RotateCamera();
	} 

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
		cameraSpeed = cameraSpeed * 3;
		rotationSpeed = rotationSpeed * 3;
	} else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		cameraSpeed = cameraSpeed / 3;
		rotationSpeed = rotationSpeed / 3;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		FocusObject(sceneCenter);		
	}

	// TODO:  ImGui::IsMouseHoveringAnyWindow(), issue clicking on imgui changing camera
	// https://github.com/ocornut/imgui/issues/52
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		// TODO: Blinking problem
		/*SDL_ShowCursor(SDL_DISABLE);*/
		iPoint mousePos = App->input->GetMousePosition();
		if(lastX != mousePos.x || lastY != mousePos.y)
			MouseUpdate(mousePos.x, mousePos.y);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
		// SDL_ShowCursor(SDL_ENABLE);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		// SDL_ShowCursor(SDL_DISABLE);
	} else if (App->input->GetMouseWheelUsed() == KEY_UP) {
		Zooming(true);
	} else if (App->input->GetMouseWheelUsed() == KEY_DOWN) {
		Zooming(false);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_UP) {
		// LOG("LEAVING WITH CAMERA");
	}

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::MoveCamera(CameraMovement cameraSide) {

	float normCameraSpeed = cameraSpeed * App->deltaTime;

	switch (cameraSide) {
		case Upwards:
			cameraPos += cameraUp.Normalized() * normCameraSpeed;
			break;
		case Downwards:
			cameraPos -= cameraUp.Normalized() * normCameraSpeed;
			break;
		case Left:
			cameraPos += cameraUp.Cross(cameraFront).Normalized() * normCameraSpeed;
			break;
		case Right:
			cameraPos -= cameraUp.Cross(cameraFront).Normalized() * normCameraSpeed;
			break;
		case Forward:
			cameraPos += normCameraSpeed * cameraFront.Normalized();
			break;
		case Backwards:
			cameraPos -= normCameraSpeed * cameraFront.Normalized();
			break;
	}
}

void ModuleCamera::RotateCamera() {

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 rotation;
	rotation.x = SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	rotation.y = SDL_sinf(math::DegToRad(pitch));
	rotation.z = SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	cameraFront = rotation.Normalized();
}

math::float4x4 ModuleCamera::LookAt(math::float3& cameraPos, math::float3& cameraFront, math::float3& cameraUp) {
	math::float3 f(cameraFront); f.Normalize();
	math::float3 s(f.Cross(cameraUp)); s.Normalize();
	math::float3 u(s.Cross(f));

	math::float4x4 matrix;
	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(cameraPos); matrix[1][3] = -u.Dot(cameraPos); matrix[2][3] = f.Dot(cameraPos);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

math::float4x4 ModuleCamera::ProjectionMatrix()
{
	return frustum.ProjectionMatrix();
}

void ModuleCamera::InitFrustum() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	SetVerticalFOV(fovY);
}
// TODO: Set fovs not working 100% times
void ModuleCamera::SetHorizontalFOV(float& fovXDegrees) {
	fovX = fovXDegrees;
	frustum.horizontalFov = math::DegToRad(fovX);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5) * (screenHeight / screenWidth));
}

void ModuleCamera::SetVerticalFOV(float& fovYDegrees) {
	fovY = fovYDegrees;
	frustum.verticalFov = math::DegToRad(fovY);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (screenWidth / screenHeight));
}

void ModuleCamera::SetScreenNewScreenSize(float newWidth, float newHeight) {
	screenWidth = newWidth;
	screenHeight = newHeight;
	screenRatio = (float)(screenWidth / screenHeight);

	SetHorizontalFOV(fovX);
	SetVerticalFOV(fovY);
}

void ModuleCamera::MouseUpdate(int mouseXpos, int mouseYpos) 
{
	LOG("%d %d", mouseXpos, mouseYpos);
	if (firstMouse)
	{
		lastX = mouseXpos;
		lastY = mouseYpos;
		firstMouse = false;
	}

	int xoffset = mouseXpos - lastX;
	int yoffset = lastY - mouseYpos;
	lastX = mouseXpos;
	lastY = mouseYpos;

	
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 front;
	front.x = SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	front.y = SDL_sinf(math::DegToRad(pitch));
	front.z = SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	cameraFront = front.Normalized();
}

void ModuleCamera::Zooming(bool positive) {
	if(positive)
		fovX += 10.0;
	else
		fovX -= 10.0;
	
	if (fovX > 100.0f) {
		fovX = 100.0f;
	} else if (fovX < 0.0f) {
		fovX = 0.0f;
	}

	zoomValue = 45.0f / fovX;

	SetHorizontalFOV(fovX);

	if(zoomValue != 1.0f)
		App->editor->showZoomMagnifier = true;
	else 
		App->editor->showZoomMagnifier = false;

}

void ModuleCamera::FocusObject(math::float3 objectCenterPos) {
	cameraFront = objectCenterPos - cameraPos;
	// TOA rule
	// TODO: fix this so we can move the camera from the center instead of the last point stored when pressed F
	pitch = math::RadToDeg(SDL_tanf(cameraFront.y / cameraFront.x));
	yaw = math::RadToDeg(SDL_tanf(cameraFront.z / cameraFront.x)) - 90;

}