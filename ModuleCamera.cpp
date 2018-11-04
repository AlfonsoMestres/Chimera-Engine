#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() {
	cameraPos = math::float3(0.0f, 0.0f, 6.0f);
	cameraFront = math::float3(0.0f, 0.0f, -1.0f);
	cameraUp = math::float3(0.0f, 1.0f, 0.0f);

	zoomSpeed = 10.0f;
	cameraSpeed = 17.0f;
	rotationSpeed = 65.0f;
	mouseSensitivity = 0.5f;
}

// Destructor
ModuleCamera::~ModuleCamera() {}

// Called before render is available
bool ModuleCamera::Init()
{
	InitFrustum();
	UpdatePitchYaw();
	return true;
}

// Called every draw update
update_status ModuleCamera::PreUpdate() {

	CameraMovementKeyboard();
	
	CameraMovementMouse();

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		FocusObject(sceneCenter);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN) {
		orbiting = true;
	} else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP) {
		orbiting = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
		cameraSpeed = cameraSpeed * 3;
		rotationSpeed = rotationSpeed * 3;
	} else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		cameraSpeed = cameraSpeed / 3;
		rotationSpeed = rotationSpeed / 3;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp() {

	return true;
}

void ModuleCamera::MoveCamera(CameraMovement cameraSide) {

	float normCameraSpeed = cameraSpeed * App->deltaTime;

	switch (cameraSide) {
		case Upwards:
			cameraPos += cameraUp * normCameraSpeed;
			break;
		case Downwards:
			cameraPos -= cameraUp * normCameraSpeed;
			break;
		case Left:
			cameraPos += cameraUp.Cross(cameraFront) * normCameraSpeed;
			break;
		case Right:
			cameraPos -= cameraUp.Cross(cameraFront) * normCameraSpeed;
			break;
		case Forward:
			cameraPos += cameraFront.Normalized() * normCameraSpeed;
			break;
		case Backwards:
			cameraPos -= cameraFront.Normalized() * normCameraSpeed;
			break;
	}
}

void ModuleCamera::RotateCameraKeyBoard(CameraMovement cameraSide) {

	float normRotationSpeed = rotationSpeed * App->deltaTime;

	switch (cameraSide) {
		case Upwards:
			pitch += normRotationSpeed;
			break;
		case Downwards:
			pitch -= normRotationSpeed;
			break;
		case Left:
			yaw -= normRotationSpeed;
			break;
		case Right:
			yaw += normRotationSpeed;
			break;
	}

	pitch = math::Clamp(pitch, -80.0f, 80.0f);

	math::float3 rotation;
	if (orbiting) {
		/*math::Quat rotacion = math::Quat::RotateY(math::DegToRad(normRotationSpeed));
		math::float3 puntoARotar = cameraPos - ((cameraFront) * 3);
		puntoARotar = rotacion * puntoARotar;
		cameraPos = puntoARotar + ((cameraFront) * 3);
		LookAt(cameraPos, cameraFront, cameraUp);*/
		math::float3 cameraTarget = cameraPos + cameraFront * 5;
		cameraPos.x = cameraPos.x + 5 * SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraPos.y = cameraPos.y + 5 * SDL_sinf(math::DegToRad(pitch));
		cameraPos.z = cameraPos.z + 5 * -SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraFront = (cameraPos - cameraTarget).Normalized();
	} else {
		rotation.x = SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		rotation.y = SDL_sinf(math::DegToRad(pitch));
		rotation.z = -SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraFront = rotation;
	}
}

void ModuleCamera::RotateCameraMouse(const iPoint& mousePosition) {

	if (firstMouse) {
		lastX = mousePosition.x;
		lastY = mousePosition.y;
		firstMouse = false;
	}

	int xoffset = mousePosition.x - lastX;
	int yoffset = lastY - mousePosition.y;
	lastX = mousePosition.x;
	lastY = mousePosition.y;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	pitch = math::Clamp(pitch, -80.0f, 80.0f);


	math::float3 rotation;
	if (orbiting) {
		math::float3 cameraTarget = cameraPos + cameraFront * 5;
		cameraPos.x = cameraPos.x + 5 * SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraPos.y = cameraPos.y + 5 * SDL_sinf(math::DegToRad(pitch));
		cameraPos.z = cameraPos.z + 5 * -SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraFront = (cameraPos - cameraTarget).Normalized();
	} else {
		rotation.x = SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		rotation.y = SDL_sinf(math::DegToRad(pitch));
		rotation.z = -SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
		cameraFront = rotation.Normalized();
	}
}

math::float4x4 ModuleCamera::LookAt(math::float3& cameraPos, math::float3& cameraFront, math::float3& cameraUp) {
	math::float3 f(cameraFront.Normalized());
	math::float3 s(f.Cross(cameraUp)); s.Normalize();
	math::float3 u(s.Cross(f)); // TODO: cameraUp not updating to editor

	math::float4x4 matrix;
	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(cameraPos); matrix[1][3] = -u.Dot(cameraPos); matrix[2][3] = f.Dot(cameraPos);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

math::float4x4 ModuleCamera::ProjectionMatrix() {
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

void ModuleCamera::SetHorizontalFOV(float& fovXDegrees) {
	fovX = fovXDegrees;
	frustum.horizontalFov = math::DegToRad(fovX);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5) * ((float)screenHeight / (float)screenWidth));
}

void ModuleCamera::SetVerticalFOV(float& fovYDegrees) {
	fovY = fovYDegrees;
	frustum.verticalFov = math::DegToRad(fovY);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)screenWidth / (float)screenHeight));
}

void ModuleCamera::SetScreenNewScreenSize(unsigned newWidth, unsigned newHeight) {
	screenWidth = newWidth;
	screenHeight = newHeight;
	screenRatio = (float)screenWidth / (float)screenHeight;

	SetHorizontalFOV(fovX);
	SetVerticalFOV(fovY);
}

void ModuleCamera::Zooming() {
	fovX = math::Clamp(fovX, 15.0f, 85.0f);

	zoomValue = 45.0f / fovX;

	SetHorizontalFOV(fovX);

	if(zoomValue != 1.0f)
		App->editor->showZoomMagnifier = true;
	else 
		App->editor->showZoomMagnifier = false;
}

void ModuleCamera::FocusObject(math::float3& objectCenterPos) {
	cameraFront = objectCenterPos - cameraPos;
	UpdatePitchYaw();
}

void ModuleCamera::UpdatePitchYaw() {
	pitch = -math::RadToDeg(SDL_atanf(cameraFront.y / cameraFront.z));
	yaw = -math::RadToDeg(SDL_atanf(cameraFront.x / cameraFront.z));

	if (math::IsNan(pitch))
		pitch = 0.0f;

	if (math::IsNan(yaw))
		yaw = 0.0f;

}

void ModuleCamera::CameraMovementMouse() {

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		SDL_ShowCursor(SDL_DISABLE);
		RotateCameraMouse(App->input->GetMousePosition());
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
		SDL_ShowCursor(SDL_ENABLE);
		firstMouse = true;
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_X1) == KEY_DOWN) {
 		fovX += zoomSpeed;
		Zooming();
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_X2) == KEY_DOWN) {
		fovX -= zoomSpeed;	
		Zooming();
	}
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
	} else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		RotateCameraKeyBoard(Upwards);
	} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		RotateCameraKeyBoard(Downwards);
	} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		RotateCameraKeyBoard(Left);
	} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		RotateCameraKeyBoard(Right);
	}
}