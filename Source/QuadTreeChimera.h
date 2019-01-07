#ifndef __QUADTREECHIMERA_H__
#define __QUADTREECHIMERA_H__

#include "Geometry/AABB.h"
#include <list>

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
		void Clear(bool clearAllGameObjects);

		void ExpandLimits(GameObject* gameObject);

	public:
		float					expansionValue = 0.0f;
		math::AABB				quadLimits;
		QuadTreeNode*			root = nullptr;
		std::list<GameObject*>	allGO;

};

#endif
