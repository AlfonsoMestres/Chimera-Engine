#ifndef __QUADTREECHIMERA_H__
#define __QUADTREECHIMERA_H__

#include "Geometry/AABB.h"
#include <list>
#include <vector>

class GameObject;

class QuadTreeNode 
{
	public:
		QuadTreeNode();
		QuadTreeNode(const math::AABB& aabb);
		~QuadTreeNode();

		void Insert(GameObject* gameObject);
		void CreateChilds();
		void RecalculateSpace();
		bool IsLeaf() const;

		template<typename TYPE>
		void CollectIntersections(std::vector<GameObject*>& gameObjectList, const TYPE& primitive) const;

	public:
		math::AABB				aabb;
		std::list<GameObject*>	goList;
		QuadTreeNode*			parent = nullptr;
		QuadTreeNode*			childs[4];
		int						maxItems = 1;
		float					minSize = 1000.0f;
};

class QuadTreeChimera
{
	public:
		QuadTreeChimera();
		~QuadTreeChimera();

		void InitQuadTree(const math::AABB& aabb, bool clearAllGameObjects = false);
		void Insert(GameObject* gameObject, bool addToAllGameObjects = false);
		void Remove(GameObject* gameObject);
		void Clear();

		template<typename TYPE>
		void CollectIntersections(std::vector<GameObject*>& gameObjectList, const TYPE& primitive) const;

		void ExpandLimits(GameObject* gameObject);

	public:
		float					expansionValue = 0.0f;
		math::AABB				quadLimits;
		QuadTreeNode*			root = nullptr;
		std::list<GameObject*>	goList;

};


template<typename TYPE>
inline void QuadTreeChimera::CollectIntersections(std::vector<GameObject*>& gameObject, const TYPE& primitive) const {
	if (root != nullptr) {
		root->CollectIntersections(gameObject, primitive);
	}
}

template<typename TYPE>
inline void QuadTreeNode::CollectIntersections(std::vector<GameObject*>& gameObject, const TYPE& primitive) const {
	if (primitive.Intersects(aabb)) {
		for (std::list<GameObject*>::const_iterator it = goList.begin(); it != goList.end(); ++it) {
			if (primitive.Intersects((*it)->bbox)) {
				gameObject.push_back(*it);
			}
		}
		for (int i = 0; i < 4; ++i) {
			if (childs[i] != nullptr) {
				childs[i]->CollectIntersections(gameObject, primitive);
			}
		}
	}
}

#endif
