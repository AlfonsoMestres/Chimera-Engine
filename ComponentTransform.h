#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "imgui.h"
#include "Math/MathFunc.h"
#include "Math/float4x4.h"

class ComponentTransform : public Component
{
	public:
		ComponentTransform(GameObject* goContainer, const aiMatrix4x4 &transform);
		~ComponentTransform();

		void AddTransform(const aiMatrix4x4 &transform);

	public:
		math::float3 position = math::float3::zero;
		math::Quat rotation = math::Quat::identity;
		math::float3 eulerRotation = math::float3::zero;
		math::float3 scale = math::float3::zero;
};

#endif
