#include "Config.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* goParent) : Component(goParent, ComponentType::CAMERA) {
	InitFrustum();
	CreateFrameBuffer(App->window->width, App->window->height);
}

ComponentCamera::~ComponentCamera() { 

	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &renderTexture);

	for (std::list<ComponentCamera*>::iterator it = App->camera->gameCameras.begin(); it != App->camera->gameCameras.end(); ++it) {
		if ((*it) == this) {
			if (App->camera->selectedCamera == this) {
				App->camera->selectedCamera = nullptr;
			}
			App->camera->gameCameras.erase(it);
			return;
		}
	}

}

void ComponentCamera::InitFrustum(math::float3 camPos, math::float3 camFront, math::float3 camUp) {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = camPos;
	frustum.front = camFront;
	frustum.up = camUp;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 420.0f;
	frustum.verticalFov = math::pi / 2.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / (float)App->window->height));
}

void ComponentCamera::Update() {
	//TODO: Set up a bool to see if the transform is edited to recalculate the frustum
	if (goContainer == nullptr) return;
	if (goContainer->transform == nullptr) return;

	math::float4x4 transform = goContainer->GetGlobalTransform();
	frustum.pos = transform.TranslatePart();
	frustum.front = transform.RotatePart().Mul(math::float3::unitZ).Normalized();
	frustum.up = transform.RotatePart().Mul(math::float3::unitY).Normalized();
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

void ComponentCamera::DrawProperties(bool enabled) {

	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Camera properties")) {
		bool removed = Component::DrawComponentState(); 
		if (removed) {
			ImGui::PopID();
			return;
		}

		ImGui::Checkbox("Debug", &debugDraw);

		ImGui::RadioButton("Wireframe", &wireFrame, GL_LINE); ImGui::SameLine();
		ImGui::RadioButton("Fill", &wireFrame, GL_FILL);

		ImGui::Separator();

		if (ImGui::SliderFloat("FOV", &fovY, 40, 120)) {
			SetVerticalFOV(fovY);
		}

		ImGui::SliderFloat("zNear", &frustum.nearPlaneDistance, 0.1f, frustum.farPlaneDistance);
		ImGui::SliderFloat("zFar", &frustum.farPlaneDistance, frustum.nearPlaneDistance, 1000.0f);

		if (App->camera->selectedCamera == this) {
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::Button("Current camera");
			ImGui::PopStyleVar();
		} else {
			if (ImGui::Button("Set camera")) {
				App->camera->selectedCamera = this;
			}
		}
		ImGui::Separator();
	}
	ImGui::PopID();
}

Component* ComponentCamera::Duplicate() {
	return new ComponentCamera(*this);
}

void ComponentCamera::LookAt(math::float3 target) {
	math::float3 dir = (target - frustum.pos).Normalized();
	math::float3x3 look = math::float3x3::LookAt(frustum.front, dir, frustum.up, math::float3::unitY);
	frustum.front = look.Mul(frustum.front).Normalized();
	frustum.up = look.Mul(frustum.up).Normalized();
}

math::float4x4 ComponentCamera::GetViewMatrix() {
	math::float4x4 view = frustum.ViewMatrix();
	return view.Transposed();
}

math::float4x4 ComponentCamera::GetProjectionMatrix() {
	return frustum.ProjectionMatrix().Transposed();
}

void ComponentCamera::SetScreenNewScreenSize(unsigned width, unsigned height) {
	screenWidth = width;
	screenHeight = height;
	screenRatio = (float)screenWidth / (float)screenHeight;

	SetHorizontalFOV(fovX);
	SetVerticalFOV(fovY);
	CreateFrameBuffer(App->window->width, App->window->height);
}

void ComponentCamera::Rotate(float dx, float dy) {
	if (dx != 0) {
		math::Quat rotation = math::Quat::RotateY(math::DegToRad(-dx)).Normalized();
		frustum.front = rotation.Mul(frustum.front).Normalized();
		frustum.up = rotation.Mul(frustum.up).Normalized();
	}
	if (dy != 0) {
		math::Quat rotation = math::Quat::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(-dy)).Normalized();
		math::float3 validUp = rotation.Mul(frustum.up).Normalized();
		// Avoiding gimbal lock
		if (validUp.y > 0.0f) {
			frustum.up = validUp;
			frustum.front = rotation.Mul(frustum.front).Normalized();
		}
	}
}

void ComponentCamera::Orbit(float dx, float dy) {
	// TODO: set up the orbit when no GO is selected in front of the camera
	if (App->scene->goSelected == nullptr) return;

	math::float3 center = App->scene->goSelected->transform->position;

	if (dx != 0) {
		math::Quat rotation = math::Quat::RotateY(math::DegToRad(-dx)).Normalized();
		frustum.pos = rotation.Mul(frustum.pos);
	}
	if (dy != 0) {
		math::Quat rotation = math::Quat::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(-dy)).Normalized();
		math::float3 new_pos = rotation.Mul(frustum.pos);
		if (!(abs(new_pos.x - center.x) < 0.5f && abs(new_pos.z - center.z) < 0.5f)) {
			frustum.pos = new_pos;
		}
	}

	LookAt(center);
}

void ComponentCamera::CreateFrameBuffer(float winWidth, float winHeight) {
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error: Framebuffer issue");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* RapidJson Storage */
void ComponentCamera::Save(Config* config) {
	config->StartObject();

	config->AddComponentType("componentType", componentType);

	if (goContainer != nullptr) {
		config->AddString("goContainer", goContainer->uuid);
	}

	//TODO: we are not moving the camera anymore, we are moving his goContainer
	config->AddFloat("frustum.nearPlaneDistance", frustum.nearPlaneDistance);
	config->AddFloat("frustum.farPlaneDistance", frustum.farPlaneDistance);
	config->AddFloat3("frustum.pos", frustum.pos);
	config->AddFloat3("frustum.front", frustum.front);
	config->AddFloat3("frustum.up", frustum.up);
	config->AddBool("debugDraw", debugDraw);
	config->AddInt("wireFrame", GL_FILL);

	config->EndObject();
}

void ComponentCamera::Load(Config* config, rapidjson::Value& value) {
	frustum.nearPlaneDistance = config->GetFloat("frustum.nearPlaneDistance", value);
	frustum.farPlaneDistance = config->GetFloat("frustum.farPlaneDistance", value);
	frustum.pos = config->GetFloat3("frustum.pos", value);
	frustum.front = config->GetFloat3("frustum.front", value);
	frustum.up = config->GetFloat3("frustum.up", value);
	debugDraw = config->GetBool("debugDraw", value);
	wireFrame = config->GetInt("wireFrame", value);
}