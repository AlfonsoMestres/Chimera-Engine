#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* goParent) : Component(goParent, ComponentType::CAMERA) {
	InitFrustum(goParent);
}

ComponentCamera::~ComponentCamera() { }

void ComponentCamera::InitFrustum(GameObject* goParent) {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = goParent != nullptr ? 100.0f : 1000.0f;

	SetVerticalFOV(fovY);
}

void ComponentCamera::SetHorizontalFOV(float fovXDegrees) {
	fovX = fovXDegrees;
	frustum.horizontalFov = math::DegToRad(fovXDegrees);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5) * ((float)screenHeight / (float)screenWidth));
}

void ComponentCamera::SetVerticalFOV(float fovYDegrees) {
	fovY = fovYDegrees;
	frustum.verticalFov = math::DegToRad(fovYDegrees);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)screenWidth / (float)screenHeight));
}

void ComponentCamera::DrawProperties() {

	if (ImGui::CollapsingHeader("Camera properties")) {
		float camPos[3] = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
		ImGui::InputFloat3("Camera position", camPos, "%.3f");
		cameraPosition = math::float3(camPos[0], camPos[1], camPos[2]);
		float vectorFront[3] = { cameraFront.x, cameraFront.y, cameraFront.z };
		ImGui::InputFloat3("Vector front", vectorFront, "%.3f");
		cameraFront = math::float3(vectorFront[0], vectorFront[1], vectorFront[2]);
		float vectorUp[3] = { cameraUp.x, cameraUp.y, cameraUp.z };
		ImGui::InputFloat3("Vector up", vectorUp, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::Separator();
		ImGui::InputFloat("Pitch", &pitch, 0, 0, 0);
		ImGui::InputFloat("Yaw", &yaw, 0, 0, 0);
	}

	if (ImGui::CollapsingHeader("Camera configurations")) {
		ImGui::SliderFloat("Near Plane", &frustum.nearPlaneDistance, 0.1f, frustum.farPlaneDistance);
		ImGui::SliderFloat("Far Plane", &frustum.farPlaneDistance, 0.1f, 1000.0f);
	}
}

Component* ComponentCamera::Duplicate() {
	return new ComponentCamera(*this);
}

math::float4x4 ComponentCamera::ProjectionMatrix() {
	return frustum.ProjectionMatrix();
}

math::float4x4 ComponentCamera::LookAt(math::float3& cameraPosition, math::float3& cameraFront, math::float3& cameraUp) {
	cameraFront.Normalize();
	math::float3 cameraSide = cameraFront.Cross(cameraUp); cameraSide.Normalize();
	math::float3 auxCameraUp = cameraSide.Cross(cameraFront);

	frustum.pos = cameraPosition;
	frustum.front = cameraFront;
	frustum.up = auxCameraUp;

	return frustum.ViewMatrix();
}

math::float4x4 ComponentCamera::LookAt(math::float3& cameraPos, math::float3& target) {
	math::float3 front(target - cameraPos); front.Normalize();
	// We are not implementing roll, so we will calculate the up again mantaining the verticalitiy
	math::float3 side(cameraFront.Cross(cameraUp)); side.Normalize();
	math::float3 up(side.Cross(front));

	math::float4x4 viewMatrix(math::float4x4::zero);
	viewMatrix[0][0] = side.x; viewMatrix[0][1] = side.y; viewMatrix[0][2] = side.z;
	viewMatrix[1][0] = up.x; viewMatrix[1][1] = up.y; viewMatrix[1][2] = up.z;
	viewMatrix[2][0] = -front.x; viewMatrix[2][1] = -front.y; viewMatrix[2][2] = -front.z;
	viewMatrix[0][3] = -side.Dot(cameraPos); viewMatrix[1][3] = -up.Dot(cameraPos); viewMatrix[2][3] = front.Dot(cameraPos);
	viewMatrix[3][0] = 0.0f; viewMatrix[3][1] = 0.0f; viewMatrix[3][2] = 0.0f; viewMatrix[3][3] = 1.0f;

	return viewMatrix;
}

void ComponentCamera::SetScreenNewScreenSize(unsigned width, unsigned height) {
	screenWidth = width;
	screenHeight = height;
	screenRatio = (float)screenWidth / (float)screenHeight;

	SetHorizontalFOV(fovX);
	SetVerticalFOV(fovY);
}

void ComponentCamera::UpdatePitchYaw() {
	pitch = -math::RadToDeg(sinf(-cameraFront.y));
	yaw = math::RadToDeg(atan2f(cameraFront.z, cameraFront.x)) + 90.0f;

	if (math::IsNan(pitch)) {
		pitch = 0.0f;
	}

	if (math::IsNan(yaw)) {
		yaw = 0.0f;
	}
}