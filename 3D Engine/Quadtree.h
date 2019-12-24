//#ifndef _Quadtree_H__
//#define _Quadtree_H__
//
//#include <vector>
//#include "Globals.h"
//#include "MathGeoLib/include/MathGeoLib.h"
//#include "glmath.h"
//#include "GameObject.h"
//
//class Node;
//
//class Quadtree {
//
//public:
//
//	Quadtree(AABB aabb, int capacity);
//	~Quadtree();
//
//public:
//
//	Node* base;
//	bool firstnode = true;
//
//};
//
//enum class Quadpart {
//	NONE = -1,
//	BASE,
//	MIDDLE,
//	EXTREMITY
//};
//
//
//class Node {
//
//public:
//
//	Node();
//	Node(AABB aabb, Quadpart ntype, int capacity);
//	~Node();
//
//	void Clear();
//	void Split();
//	void Draw();
//	bool Insert(GameObject* gameObject);
//	std::vector<GameObject*> ObjectsInside(Frustum frustum);
//	bool Intersect(Frustum frustum, AABB aabb);
//
//
//public:
//
//	bool DontDrawChilds = true;
//	Quadpart nodeType = Quadpart::NONE;
//	AABB box;
//	Node *childs = nullptr;
//	std::vector<GameObject*> objects;
//	int capacity = 0;
//
//private:
//
//	int N_Childs = 0;
//	
//};
//
//#endif
