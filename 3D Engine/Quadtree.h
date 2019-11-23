#ifndef _Quadtree_H__
#define _Quadtree_H__

#include <vector>
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"
#include "GameObject.h"

enum class NODE_TYPE {
	NONE = -1,
	ROOT,
	BRANCH,
	LEAF
};

class Node;

class Quadtree {

public:

	Quadtree(AABB aabb, int capacity);
	~Quadtree();

public:

	Node* rootNode;
	bool firstnode = true;

};

class Node {

public:

	Node();
	Node(AABB aabb, NODE_TYPE ntype, int capacity);
	~Node();

	void Clear();
	void Split();
	void Draw();
	bool Insert(GameObject* gameObject);
	std::vector<GameObject*> CollectChilldren(Frustum frustum);
	bool Intersect(Frustum frustum, AABB aabb);


public:

	bool isLeaf = true;
	NODE_TYPE nodeType = NODE_TYPE::NONE;
	AABB aabb;
	Node *nodes = nullptr;
	std::vector<GameObject*> objects;
	int capacity = 0;

private:

	int nodesAmount = 0;
	
};

#endif
