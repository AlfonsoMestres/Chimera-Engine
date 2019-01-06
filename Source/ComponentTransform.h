#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "Component.h"
#include "imgui.h"
#include "Math/MathFunc.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"
#include <assimp/scene.h> 

class GameObject;

class ComponentTransform : public Component
{
	public:
		ComponentTransform(GameObject* goContainer, const math::float4x4& transform);
		ComponentTransform(const ComponentTransform& duplicatedTransform);
		~ComponentTransform();

		void AddTransform(const math::float4x4& transform);
		void SetRotation(const math::Quat& rot);
		void SetPosition(const math::float3& pos);
		void RotationToEuler();

		void SetLocalToWorld(const math::float4x4& localTrans);
		void SetWorldToLocal(const math::float4x4& parentTrans);

		void		DrawProperties(bool enabled) override;
		Component*	Duplicate() override;

		void Save(Config* config) override;
		void Load(Config* config, rapidjson::Value& value) override;

	public:
		math::float3	position = math::float3::zero;
		math::Quat		rotation = math::Quat::identity;
		math::float3	eulerRotation = math::float3::zero;
		math::float3	scale = math::float3::zero;
		bool			edited = false;
};

#endif
