#include "Config.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* goContainer, const math::float4x4& transform) : Component(goContainer, ComponentType::TRANSFORM) {
	AddTransform(transform);
}

ComponentTransform::ComponentTransform(const ComponentTransform& duplicatedTransform) : Component(duplicatedTransform){
	position = duplicatedTransform.position;
	rotation = duplicatedTransform.rotation;
	scale = duplicatedTransform.scale;
	eulerRotation = duplicatedTransform.eulerRotation;
}

ComponentTransform::~ComponentTransform() { }

Component* ComponentTransform::Duplicate() {
	return new ComponentTransform(*this);
}

void ComponentTransform::AddTransform(const math::float4x4& transform) {
	math::float3 translation;
	math::float3 scaling;
	math::Quat aiRotation;
	transform.Decompose(translation, aiRotation, scaling);

	position = { translation.x, translation.y, translation.z };
	scale = { scaling.x, scaling.y, scaling.z };
	rotation = math::Quat(aiRotation.x, aiRotation.y, aiRotation.z, aiRotation.w);
	RotationToEuler();
}

void ComponentTransform::SetRotation(const Quat& rot) {
	rotation = rot;
	RotationToEuler();
}

void ComponentTransform::RotationToEuler() {
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation.x = math::RadToDeg(eulerRotation.x);
	eulerRotation.y = math::RadToDeg(eulerRotation.y);
	eulerRotation.z = math::RadToDeg(eulerRotation.z);
}

void ComponentTransform::SetPosition(const float3& pos) {
	position = pos;
}

void ComponentTransform::SetLocalToWorld(const math::float4x4& localTrans) {
	math::float4x4 world = localTrans;
	world.Decompose(position, rotation, scale);
	RotationToEuler();
}

void ComponentTransform::SetWorldToLocal(const math::float4x4& parentTrans) {
	math::float4x4 world = math::float4x4::FromTRS(position, rotation, scale);
	math::float4x4 local = parentTrans.Inverted() * world;
	local.Decompose(position, rotation, scale);
	RotationToEuler();
}

void ComponentTransform::DrawProperties(bool enabled) {

	if (ImGui::CollapsingHeader("Local Transform")) {

		if (!enabled) {
			//https://github.com/ocornut/imgui/issues/211
			/*ImGui::PushItemFlag(ImGuiSelectableFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);*/
		}

		if (ImGui::DragFloat3("Position", (float*)&position, 0.1f, -1000.f, 1000.f)) {
			edited = true;
		}

		if (ImGui::DragFloat3("Rotation", (float*)&eulerRotation, 0.5f, -180, 180.f)) {
			edited = true;
		}

		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f, 0.01f, 100.f)) {
			edited = true;
		}

		rotation = rotation.FromEulerXYZ(math::DegToRad(eulerRotation.x), math::DegToRad(eulerRotation.y), math::DegToRad(eulerRotation.z));

		if (edited) {
			goContainer->ComputeBBox();
			edited = false;
		}

		if (!enabled) {
			/*ImGui::PopItemFlag();
			ImGui::PopStyleVar();*/
		}

		ImGui::Separator();
	}
}

/* RapidJson storage */
void ComponentTransform::Save(Config* config) {
	config->StartObject();

	config->AddComponentType("componentType", componentType);
	config->AddString("parentUuid", parentUuid);
	config->AddString("uuid", uuid);
	config->AddFloat3("position", position);
	config->AddQuat("rotation", rotation);
	config->AddFloat3("eulerRotation", eulerRotation);
	config->AddFloat3("scale", scale);

	config->EndObject();
}

void ComponentTransform::Load(Config* config, rapidjson::Value& value) {
	uuid = config->GetString("uuid", value);
	position = config->GetFloat3("position", value);
	eulerRotation = config->GetFloat3("eulerRotation", value);
	scale = config->GetFloat3("scale", value);
	rotation = config->GetQuat("rotation", value);
}
