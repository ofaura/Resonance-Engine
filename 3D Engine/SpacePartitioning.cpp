#include "SpacePartitioning.h"
#include "GameObject.h"
#include "C_Mesh.h"

#include "mmgr/mmgr.h"

#include <vector>

SpacePartitioning::SpacePartitioning(AABB limits, uint subdivision)
{
	TreeBox = limits;
	subdivisions = subdivision;
}

SpacePartitioning::~SpacePartitioning(){}

void SpacePartitioning::Clear() {
	TreeBox.SetNegativeInfinity();
	subdivisions = 0;
	
	if (!TreeChilds.empty())
	{
		for (auto it : TreeChilds)
		{
			it->Clear();
			RELEASE(it);
		}
		TreeChilds.clear();
	}
	
	for (auto j : gameobjects)
	{
		j = nullptr;
	}
	gameobjects.clear();
}

void SpacePartitioning::Insert(GameObject * gameobject) {
	if (gameobject->GetComponent(COMPONENT_TYPE::MESH) == nullptr)
	{
		LOG("%s Mesh is null, cnat add to space partitioning", gameobject->name.c_str());
		return;
	}

	if (TreeBox.Intersects(gameobject->Globalbbox)) {
		if (!TreeChilds.empty()) {
			for (auto qtChildIt : TreeChilds) {
				qtChildIt->Insert(gameobject);
			}
		}
		else if (subdivisions > MAX_SUBDIVISIONS) {
			gameobjects.push_back(gameobject);
			return;
		}
		else {

			gameobjects.push_back(gameobject);
			if (MAX_ELEMS < gameobjects.size())
			{
				if (TreeChilds.empty())
					Subdivide();

				for (auto childsIt : TreeChilds)
				{

					for (auto gobjsIt : gameobjects)
					{
						childsIt->Insert(gobjsIt);
					}
				}

				gameobjects.clear();
			}

		}
	}
}

void SpacePartitioning::Intersect(std::vector<GameObject*>& objects, const AABB& primBox) const
{
	if (primBox.Intersects(TreeBox))
	{
		for (std::vector<GameObject*>::const_iterator it = this->gameobjects.begin(); it != this->gameobjects.end(); ++it)
		{
			if (primBox.Intersects((*it)->Globalbbox))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (TreeChilds[i] != nullptr) TreeChilds[i]->Intersect(objects, primBox);
	}
}

void SpacePartitioning::Intersect(std::map<float, GameObject*>& objects, const Ray & ray) const
{
	if (ray.Intersects(TreeBox))
	{
		float hit_near, hit_far;
		for (std::vector<GameObject*>::const_iterator it = this->gameobjects.begin(); it != this->gameobjects.end(); ++it)
		{
			if (ray.Intersects((*it)->Globalbbox), hit_near, hit_far)
				objects[hit_near] = *it;
		}

		for (int i = 0; i < 4; ++i)
			if (TreeChilds[i] != nullptr) TreeChilds[i]->Intersect(objects, ray);
	}
}

void SpacePartitioning::Intersect(std::vector<GameObject*>& objects, Frustum frustum)
{
	if (TreeBox.Intersects(frustum))
	{
		for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
		{
			if ((*it) != nullptr)
			{

				if (frustum.Intersects((*it)->Globalbbox)) {
					objects.push_back((*it));
				}
			}
		}

		if (!TreeChilds.empty())
		{
			for (int i = 0; i < 4; i++)
			{
				TreeChilds[i]->Intersect(objects, frustum);
			}
		}
	}
}


void SpacePartitioning::Subdivide() {

	float x = TreeBox.Size().x;
	float y = TreeBox.Size().y;
	float z = TreeBox.Size().z;

	float3 centerXZ = { TreeBox.HalfSize().x, y, TreeBox.HalfSize().z };

	//NW
	TreeChilds.push_back(new SpacePartitioning(AABB(TreeBox.minPoint, TreeBox.minPoint + centerXZ), subdivisions + 1));

	//NE
	float3 neMin = TreeBox.minPoint + float3(TreeBox.HalfSize().x, 0.0f, 0.0f);
	TreeChilds.push_back(new SpacePartitioning(AABB(neMin, neMin + centerXZ), subdivisions + 1));

	//SW
	float3 swMin = TreeBox.minPoint + float3(0.0f, 0.0f, TreeBox.HalfSize().z);
	TreeChilds.push_back(new SpacePartitioning(AABB(swMin, swMin + centerXZ), subdivisions + 1));

	//SE
	TreeChilds.push_back(new SpacePartitioning(AABB(TreeBox.maxPoint - centerXZ, TreeBox.maxPoint), subdivisions + 1));
}


void SpacePartitioning::Draw() {

	float3 points[8];
	TreeBox.GetCornerPoints(points);
	glBegin(GL_LINES);
	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));


	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

	glEnd();

	for (auto it : TreeChilds)
	{
		it->Draw();
	}
}
