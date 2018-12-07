#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* goContainer, const aiMatrix4x4& transform) : Component(goContainer, ComponentType::TRANSFORM) { 
	AddTransform(transform);
}

ComponentTransform::ComponentTransform(ComponentTransform* duplicatedTransform) : Component(duplicatedTransform->goContainer, ComponentType::TRANSFORM){
	position = duplicatedTransform->position;
	rotation = duplicatedTransform->rotation;
	scale = duplicatedTransform->scale;
	eulerRotation = duplicatedTransform->eulerRotation;
}

ComponentTransform::~ComponentTransform() { }

void ComponentTransform::AddTransform(const aiMatrix4x4& transform) {
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion airotation;
	transform.Decompose(scaling, airotation, translation);

	position = { translation.x, translation.y, translation.z };
	scale = { scaling.x, scaling.y, scaling.z };
	rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);
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

void ComponentTransform::DrawProperties() {
	if (ImGui::CollapsingHeader("Local Transformation")) {
		ImGui::DragFloat3("Position", (float*)&position, 0.1f, -1000.f, 1000.f);

		ImGui::DragFloat3("Rotation", (float*)&eulerRotation, 0.5f, -180, 180.f);

		rotation = rotation.FromEulerXYZ(math::DegToRad(eulerRotation.x),
		math::DegToRad(eulerRotation.y), math::DegToRad(eulerRotation.z));

		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f, 0.01f, 100.f);

		ImGui::Separator();
	}
}
