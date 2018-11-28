#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "imgui.h"
#include "assimp/quaternion.h"
#include "Math/MathFunc.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"

// TODO: fix quaternion extern symbol

class ComponentTransform : public Component
{
	public:
		ComponentTransform(GameObject* goContainer, const aiMatrix4x4 &transform);
		~ComponentTransform();

		void AddTransform(const aiMatrix4x4 &transform);
		void SetRotation(const math::Quat & rot);
		void SetPosition(const float3 & pos);
		void RotationToEuler();

	public:
		math::float3 position = math::float3::zero;
		math::Quat rotation = math::Quat::identity;
		math::float3 eulerRotation = math::float3::zero;
		math::float3 scale = math::float3::zero;
};

#endif
