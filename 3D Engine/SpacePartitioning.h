#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "Globals.h"

#include "Math.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"

#include <map>
#include <list>
#include <vector>

constexpr auto MAX_ELEMS = 1;
constexpr auto MAX_SUBDIVISIONS = 10;

class GameObject;

class SpacePartitioning
{
public:

	SpacePartitioning(AABB limits, uint subdivision);
	~SpacePartitioning();


	void Clear();
	void Insert(GameObject* gameobject);

	void Intersect(std::vector<GameObject*>& gameobjects, const AABB& bBox) const;
	void Intersect(std::map<float, GameObject*>& objects, const Ray& bBox) const;
	void Intersect(std::vector<GameObject*>& objects, Frustum frustum);

	void Subdivide();

	void Draw();

public:

	std::vector<GameObject*> gameobjects;
	std::vector<SpacePartitioning*> TreeChilds;
	AABB TreeBox;

private:

	uint subdivisions = 0;

};
#endif // !__QUADTREE_H__