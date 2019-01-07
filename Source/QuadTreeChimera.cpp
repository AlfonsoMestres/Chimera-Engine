#include "QuadTreeChimera.h"
#include "Globals.h"
#include "GameObject.h"
#include "ComponentMesh.h"

QuadTreeChimera::QuadTreeChimera() { 
	quadLimits = math::AABB(math::float3(-20.0f, -20.0f, -20.0f), math::float3(20.0f, 20.0f, 20.0f));
	InitQuadTree(quadLimits, true);
}

QuadTreeChimera::~QuadTreeChimera() {
	Clear(true);
}

void QuadTreeChimera::InitQuadTree(const math::AABB& aabb, bool clearAllGameObjects) {
	Clear(clearAllGameObjects);
	root = new QuadTreeNode(aabb);
}

void QuadTreeChimera::Insert(GameObject* gameObject, bool addQuadList) {
	assert(root != nullptr);

	if (gameObject->bbox.Intersects(root->aabb)) {
		if (addQuadList) {
			allGO.push_back(gameObject);
		}

		root->Insert(gameObject);
	} else {
		ExpandLimits(gameObject);
	}
}

void QuadTreeChimera::ExpandLimits(GameObject* gameObject) {

	//TODO: check if extremepoint is correct
	math::float3 extremePoint = gameObject->bbox.ExtremePoint(quadLimits.CenterPoint());
	expansionValue = 0.0f;

	if (extremePoint.x < 0.0f && extremePoint.z < 0.0f) {
		//TOP LEFT
		if (quadLimits.minPoint.x > extremePoint.x) {
			expansionValue = quadLimits.minPoint.x - extremePoint.x;
		}

		if (quadLimits.minPoint.z > extremePoint.z) {
			expansionValue = quadLimits.minPoint.z - extremePoint.z;
		}
	} else if (extremePoint.x > 0.0f && extremePoint.z < 0.0f) {
		//TOP RIGHT
		if (quadLimits.maxPoint.x < extremePoint.x) {
			expansionValue = extremePoint.x - quadLimits.maxPoint.x;
		}

		if (quadLimits.minPoint.z > extremePoint.z) {
			expansionValue = quadLimits.minPoint.z - extremePoint.z;
		}
	} else if (extremePoint.x > 0.0f && extremePoint.z > 0.0f) {
		//BOTTOM RIGHT
		if (quadLimits.minPoint.x < extremePoint.x) {
			expansionValue = extremePoint.x - quadLimits.maxPoint.x;
		}

		if (quadLimits.maxPoint.z < extremePoint.z) {
			expansionValue = extremePoint.z - quadLimits.minPoint.z;
		}
	} else {
		if (quadLimits.minPoint.x > extremePoint.x) {
			expansionValue = quadLimits.minPoint.x - extremePoint.x;
		}

		if (quadLimits.maxPoint.z < extremePoint.z) {
			expansionValue = extremePoint.z - quadLimits.minPoint.z;
		}
	}

	quadLimits.maxPoint.x += expansionValue;
	quadLimits.maxPoint.z += expansionValue;
	quadLimits.minPoint.x -= expansionValue;
	quadLimits.minPoint.z -= expansionValue;

	InitQuadTree(quadLimits, false);
}

void QuadTreeChimera::Remove(GameObject* gameObject) {
	if (root != nullptr) {
		allGO.remove(gameObject);

		InitQuadTree(quadLimits);

		for (std::list<GameObject*>::iterator it = allGO.begin(); it != allGO.end(); ++it) {
			Insert(*it);
		}
	}
}

void QuadTreeChimera::Clear(bool clearAllGameObjects) {
	if (clearAllGameObjects) {
		allGO.clear(); //TODO: check this
	}

	delete root;
	root = nullptr;
}

QuadTreeNode::QuadTreeNode() {
	childs[0] = nullptr;
	childs[1] = nullptr;
	childs[2] = nullptr;
	childs[3] = nullptr;
}

QuadTreeNode::QuadTreeNode(const math::AABB& aabb) : aabb(aabb) {
	childs[0] = nullptr;
	childs[1] = nullptr;
	childs[2] = nullptr;
	childs[3] = nullptr;
}

QuadTreeNode::~QuadTreeNode() {
	delete childs[0];
	childs[0] = nullptr;
	delete childs[1];
	childs[1] = nullptr;
	delete childs[2];
	childs[2] = nullptr;
	delete childs[3];
	childs[3] = nullptr;
}

void QuadTreeNode::Insert(GameObject* gameObject) {
	if (IsLeaf() && (goList.size() < maxItems)) {
		goList.push_back(gameObject);
	} else {
		if (IsLeaf()) {
			CreateChilds();
		}

		goList.push_back(gameObject);
		RecalculateSpace();
	}
}

void QuadTreeNode::CreateChilds() {
	math::AABB newAABB;
	math::float3 aabbSize(aabb.Size());
	math::float3 newSize(aabbSize.x * 0.5f, aabbSize.y, aabbSize.z * 0.5f);
	math::float3 aabbCenter(aabb.CenterPoint());
	math::float3 newCenter(aabbCenter);

	newCenter.x = aabbCenter.x + aabbSize.x * 0.25f;
	newCenter.z = aabbCenter.z + aabbSize.z * 0.25f;
	newAABB.SetFromCenterAndSize(newCenter, newSize);
	childs[0] = new QuadTreeNode(newAABB);
	childs[0]->parent = this;

	newCenter.x = aabbCenter.x + aabbSize.x * 0.25f;
	newCenter.z = aabbCenter.z - aabbSize.z * 0.25f;
	newAABB.SetFromCenterAndSize(newCenter, newSize);
	childs[1] = new QuadTreeNode(newAABB);
	childs[1]->parent = this;

	newCenter.x = aabbCenter.x - aabbSize.x * 0.25f;
	newCenter.z = aabbCenter.z - aabbSize.z * 0.25f;
	newAABB.SetFromCenterAndSize(newCenter, newSize);
	childs[2] = new QuadTreeNode(newAABB);
	childs[2]->parent = this;

	newCenter.x = aabbCenter.x - aabbSize.x * 0.25f;
	newCenter.z = aabbCenter.z + aabbSize.z * 0.25f;
	newAABB.SetFromCenterAndSize(newCenter, newSize);
	childs[3] = new QuadTreeNode(newAABB);
	childs[3]->parent = this;
}

void QuadTreeNode::RecalculateSpace() {
	for (std::list<GameObject*>::iterator iterator = goList.begin(); iterator != goList.end();) {
		GameObject* gameObject = *iterator;
		bool intersects[4];
		for (int i = 0; i < 4; ++i) {
			ComponentMesh* componentMesh = (ComponentMesh*)(gameObject)->GetComponent(ComponentType::MESH);
			if (componentMesh != nullptr) {
				intersects[i] = childs[i]->aabb.Intersects(gameObject->bbox);
			}
		}

		if (intersects[0] && intersects[1] && intersects[2] && intersects[3]) {
			++iterator;
		} else {
			// Not inside the box, expand limits
			iterator = goList.erase(iterator);
			for (int i = 0; i < 4; ++i) {
				if (intersects[i]) {
					childs[i]->Insert(gameObject);
				}
			}
		}
	}
}

bool QuadTreeNode::IsLeaf() const {
	return childs[0] == nullptr;
}