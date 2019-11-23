#include "Quadtree.h"
#include "GameObject.h"
#include "mmgr/mmgr.h"

Quadtree::Quadtree( AABB aabb, int capacity)
{
	rootNode = new TreeNode(aabb, NODE_TYPE::ROOT, capacity);
	rootNode->capacity = capacity;
	rootNode->aabb = aabb;

}

Quadtree::~Quadtree() { delete[] rootNode; }

//--------------------------------------------------------------------

TreeNode::TreeNode(){}

TreeNode::TreeNode(AABB aabb, NODE_TYPE ntype, int capacity) : aabb(aabb), nodeType(ntype), capacity(capacity){}

TreeNode::~TreeNode(){}

void TreeNode::Split()
{
	isLeaf = false;
	AABB newAABBs[4];
	float3 maxPoint, minPoint;

	//NORTH-WEST subnode
	maxPoint = { aabb.MaxX(), aabb.MaxY(), aabb.MaxZ() };
	minPoint = { (aabb.MaxX() + aabb.MinX()) / 2 , aabb.MinY(), (aabb.MaxZ() + aabb.MinZ()) / 2 };
	newAABBs[0].minPoint = minPoint;
	newAABBs[0].maxPoint = maxPoint;

	//NORTH-EAST subnode
	maxPoint = { (aabb.MaxX() + aabb.MinX()) / 2, aabb.MaxY(), aabb.MaxZ() };
	minPoint = { aabb.MinX(), aabb.MinY(), (aabb.MaxZ() + aabb.MinZ()) / 2 };
	newAABBs[1].minPoint = minPoint;
	newAABBs[1].maxPoint = maxPoint;

	//SOUTH-WEST subnode
	maxPoint = { aabb.MaxX(), aabb.MaxY(),(aabb.MaxZ() + aabb.MinZ()) / 2 };
	minPoint = { (aabb.MaxX() + aabb.MinX()) / 2, aabb.MinY(), aabb.MinZ() };
	newAABBs[2].minPoint = minPoint;
	newAABBs[2].maxPoint = maxPoint;

	//SOUTH-EAST subnode
	maxPoint = { (aabb.MaxX() + aabb.MinX()) / 2, aabb.MaxY(),(aabb.MaxZ() + aabb.MinZ()) / 2 };
	minPoint = { aabb.MinX(), aabb.MinY(), aabb.MinZ() };
	newAABBs[3].minPoint = minPoint;
	newAABBs[3].maxPoint = maxPoint;

	nodes = new TreeNode[4];

	this;

	for (int i = 0; i < 4; ++i)
		nodes[i] = TreeNode(newAABBs[i], NODE_TYPE::LEAF, capacity);

	if (nodeType != NODE_TYPE::ROOT)
		nodeType = NODE_TYPE::BRANCH;

	nodesAmount = 4;

}

bool TreeNode::Insert(GameObject* gameObject)
{

	int nodesContaining = 0;
	int container = 0;

	switch (nodeType)
	{
	case NODE_TYPE::ROOT:
		if (nodesAmount == 0)
		{
			objects.push_back(gameObject);

			if (objects.size() > capacity)
			{
				Split();

				std::vector<GameObject*> auxVector = objects;
				//objects.clear();

				for (int a = 0; a < auxVector.size(); ++a)
				{
					nodesContaining = 0;
					container = 0;
					for (int i = 0; i < nodesAmount; ++i)
					{
						if (nodes[i].aabb.Intersects(auxVector[a]->Globalbbox))
						{
							nodesContaining++;
							container = i;
						}
						if (nodesContaining > 1)
							break;
					}

					if (nodesContaining == 1)
						nodes[container].Insert(auxVector[a]);

					else if (nodesContaining > 1)
						objects.push_back(auxVector[a]);
				}

			}
		}

		else
		{
			for (int i = 0; i < nodesAmount; ++i)
			{
				if (nodes[i].aabb.Intersects(gameObject->Globalbbox))
				{
					nodesContaining++;
					container = i;
				}
				if (nodesContaining > 1)
					break;
			}

			if (nodesContaining == 1)
				nodes[container].Insert(gameObject);

			else if (nodesContaining > 1)
				objects.push_back(gameObject);
		}

		break;
	case NODE_TYPE::BRANCH:

		for (int i = 0; i < nodesAmount; ++i)
		{
			if (nodes[i].aabb.Intersects(gameObject->Globalbbox))
			{
				nodesContaining++;
				container = i;
			}
			if (nodesContaining > 1)
				break;
		}

		if (nodesContaining == 1)
			nodes[container].Insert(gameObject);

		else if (nodesContaining > 1)
			objects.push_back(gameObject);

		break;

	case NODE_TYPE::LEAF:

		objects.push_back(gameObject);

		if (objects.size() > capacity)
		{
			Split();

			std::vector<GameObject*> auxVector = objects;
			//objects.clear();

			for (int a = 0; a < auxVector.size(); ++a)
			{
				nodesContaining = 0;
				container = 0;
				for (int i = 0; i < nodesAmount; ++i)
				{
					if (nodes[i].aabb.Intersects(auxVector[a]->Globalbbox))
					{
						nodesContaining++;
						container = i;
					}
					if (nodesContaining > 1)
						break;
				}

				if (nodesContaining == 1)
					nodes[container].Insert(auxVector[a]);

				else if (nodesContaining > 1)
					objects.push_back(auxVector[a]);
			}

		}


		break;
	}
	return true;

}

std::vector<GameObject*> TreeNode::CollectChilldren(Frustum frustum)
{
	std::vector<GameObject*> ret;
	std::vector<GameObject*> auxVec;

	if (!Intersect(frustum, aabb))
		return ret;

	for (int i = 0; i < objects.size(); ++i)
		ret.push_back(objects[i]);

	for (int i = 0; i < nodesAmount; ++i)
	{
		auxVec = nodes[i].CollectChilldren(frustum);
		for (int j = 0; j < auxVec.size(); ++j)
			ret.push_back(auxVec[j]);
	}

	return ret;
}

void TreeNode::Clear()
{
	if (nodes != NULL)
		delete[] nodes;

	objects.clear();
}

bool TreeNode::Intersect(Frustum frustum, AABB aabb)
{
	Plane planes[6];
	float3 corners[8];

	frustum.GetPlanes(planes);
	aabb.GetCornerPoints(corners);

	for (int p = 0; p < 6; ++p)
	{
		int aux = 8;

		for (int c = 0; c < 8; ++c)
		{
			if (planes[p].IsOnPositiveSide(corners[c]))
				aux--;
		}
		if (aux == 0)
			return false;
	}
	return true;

}

void TreeNode::Draw()
{
	float3 points[8];


	aabb.GetCornerPoints(points);

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

	if (!this->isLeaf)
	{
		for (int i = 0; i < nodesAmount; ++i)
			nodes[i].Draw();
	}
}