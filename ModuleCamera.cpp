#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() {
	target = math::float3(0.0f, 0.0f, 0.0f);
	eye = math::float3(2.0f, 2.0f, 2.0f);
	up = math::float3(0.0f, 1.0f, 0.0f);
	pitch = -45.0f; // store the pitch so we can clamp correctly
	yaw = 0.0f;
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
		RotateCamera(NegativePitch);
	} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		RotateCamera(PositivePitch);
	} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		RotateCamera(PositiveYaw);
	} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		RotateCamera(NegativeYaw);
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
		// Right
		cameraSpeed = cameraSpeed * 3;
	} else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		cameraSpeed = cameraSpeed / 3;
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN) {
		MouseUpdate(App->input->GetMousePosition().x, App->input->GetMousePosition().y);
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		LOG("ENTERED MOVING CAMERA");
		SDL_ShowCursor(SDL_DISABLE);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
		LOG("LEAVING MOVING CAMERA");
		SDL_ShowCursor(SDL_ENABLE);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		LOG("LOOKING AROUND WITH CAMERA");
		SDL_ShowCursor(SDL_DISABLE);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
		LOG("LEAVING LOOKING AROUND WITH CAMERA");
		SDL_ShowCursor(SDL_ENABLE);
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN) {
		LOG("ZOOMING WITH CAMERA");
	} else if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_UP) {
		LOG("LEAVING WITH CAMERA");
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


math::float4x4 ModuleCamera::ProjectionMatrix()
{
	math::float4x4 projectMatrix;
	projectMatrix = frustum.ProjectionMatrix();

	return projectMatrix;
}

void ModuleCamera::MoveCamera(CameraMovement cameraSide) {
	switch (cameraSide) {
		case Upwards:
			// Because we want to go to the Y axis, even if we did a pitch
			eye += math::float3(0.0f, 1.0f, 0.0f) * cameraSpeed;
			target += math::float3(0.0f, 1.0f, 0.0f) * cameraSpeed;
			break;
		case Downwards:
			eye -= math::float3(0.0f, 1.0f, 0.0f) * cameraSpeed;
			target -= math::float3(0.0f, 1.0f, 0.0f) * cameraSpeed;
			break;
		case Left:
			eye -= sidew * cameraSpeed;
			target -= sidew * cameraSpeed;
			break;
		case Right:
			eye += sidew * cameraSpeed;
			target += sidew * cameraSpeed;
			break;
		case Forward:
			eye += forw * cameraSpeed;
			target += forw * cameraSpeed;
			break;
		case Backwards:
			eye -= forw * cameraSpeed;
			target -= forw * cameraSpeed;
			break;
	}
}

void ModuleCamera::RotateCamera(CameraRotation cameraRotation) {
	math::float3 direction;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	LOG("%f", pitch);

	switch (cameraRotation) {
	case PositivePitch:
		pitch += rotationSpeed;
		forw.y = SDL_sin(degreesToRadians(pitch));
		forw.x = SDL_cos(degreesToRadians(pitch));
		forw.z = SDL_cos(degreesToRadians(pitch));
		forw.Normalize();
		target += forw * cameraSpeed;
		break;
	case NegativePitch:
		pitch -= rotationSpeed;
		forw.y = SDL_sin(degreesToRadians(pitch));
		forw.x = SDL_cos(degreesToRadians(pitch));
		forw.z = SDL_cos(degreesToRadians(pitch));
		forw.Normalize();
		target -= forw * cameraSpeed;
		break;
	// TODO: Check yaw
	case PositiveYaw:
		yaw += rotationSpeed;
		forw.y = SDL_cos(degreesToRadians(pitch)) * SDL_cos(degreesToRadians(yaw));
		forw.x = SDL_sin(degreesToRadians(pitch));
		forw.z = SDL_cos(degreesToRadians(pitch)) * SDL_sin(degreesToRadians(yaw));
		forw.Normalize();
		target += forw * cameraSpeed;
		eye += forw * cameraSpeed;
		break;
	case NegativeYaw:
		yaw -= rotationSpeed;
		forw.y = SDL_cos(degreesToRadians(pitch)) * SDL_cos(degreesToRadians(yaw));
		forw.x = SDL_sin(degreesToRadians(pitch));
		forw.z = SDL_cos(degreesToRadians(pitch)) * SDL_sin(degreesToRadians(yaw));
		forw.Normalize();
		target -= forw * cameraSpeed;
		eye -= forw * cameraSpeed;
		break;
	}
}

math::float4x4 ModuleCamera::LookAt(math::float3& target, math::float3& eye, math::float3& up)
{
	math::float4x4 matrix;

	forw = math::float3(target - eye); forw.Normalize();
	sidew = math::float3(forw.Cross(up)); sidew.Normalize();
	upw = math::float3(sidew.Cross(forw));

	matrix[0][0] = sidew.x; matrix[0][1] = sidew.y; matrix[0][2] = sidew.z;
	matrix[1][0] = upw.x; matrix[1][1] = upw.y; matrix[1][2] = upw.z;
	matrix[2][0] = -forw.x; matrix[2][1] = -forw.y; matrix[2][2] = -forw.z;
	matrix[0][3] = -sidew.Dot(eye); matrix[1][3] = -upw.Dot(eye); matrix[2][3] = forw.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

void ModuleCamera::InitFrustum() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;					   // TODO: Change this to -1 * instead
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * screenRatio);
}

//void ModuleCamera::ChangeScreenRatio(int screenWidth, int screenHeight) {
//	screenRatio = screenWidth / screenHeight;
//}

void ModuleCamera::SetFOV() {
//	halfwidth = tan(horizontal_angle / 2);
//	halfheight = tan(vertical_angle / 2);
//	if (halfwidth / halfheight > aspect_ratio) {
//		// use horizontal angle to set fovy
//		fovy = 2 * atan(halfwidth / aspect_ratio);
//	}
//	else {
//		// use vertical angle to set fovy
//		fovy = vertical_angle;
//	}
//	fovy = fovy * 180 / PI;
}

void ModuleCamera::MouseUpdate(int mouseXpos, int mouseYpos) 
{
	if (firstMouse)
	{
		lastX = mouseXpos;
		lastY = mouseYpos;
		firstMouse = false;
	}

	float xoffset = mouseXpos - lastX;
	float yoffset = lastY - mouseYpos;
	lastX = mouseXpos;
	lastY = mouseYpos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 front;
	front.x = SDL_cosf(yaw) * SDL_cosf(pitch);
	front.y = SDL_sinf(pitch);
	front.z = SDL_sinf(yaw) * SDL_cosf(pitch);
	forw = front.Normalized();
}