//#include "Quadtree.h"
//#include "GameObject.h"
//#include "mmgr/mmgr.h"
//
//Quadtree::Quadtree( AABB box, int capacity)
//{
//
//	base = new Node(box, Quadpart::BASE, capacity);
//	base->capacity = capacity;
//	base->box = box;
//
//}
//
//Quadtree::~Quadtree() { delete[] base; }
//
////--------------------------------------------------------------------
//
//Node::Node(){}
//Node::Node(AABB box, Quadpart ntype, int capacity) : box(box), nodeType(ntype), capacity(capacity){}
//Node::~Node(){}
//
//void Node::Split()
//{
//
//	DontDrawChilds = false;
//	AABB childsbox[4];
//	float3 max[4];
//	float3 min[4];
//	childs = new Node[4];
//
//
//	max[0] = { box.MaxX(), box.MaxY(), box.MaxZ() };
//	max[1] = { (box.MaxX() + box.MinX()) / 2, box.MaxY(), box.MaxZ() };
//	max[2] = { box.MaxX(), box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
//	max[3] = { (box.MaxX() + box.MinX()) / 2, box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
//
//
//	min[0] = { (box.MaxX() + box.MinX()) / 2 , box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
//	min[1] = { box.MinX(), box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
//	min[2] = { (box.MaxX() + box.MinX()) / 2, box.MinY(), box.MinZ() };
//	min[3] = { box.MinX(), box.MinY(), box.MinZ() };
//	
//	
//	childsbox[0].minPoint = min[0];
//	childsbox[0].maxPoint = max[0];
//	childsbox[1].minPoint = min[1];
//	childsbox[1].maxPoint = max[1];
//	childsbox[2].minPoint = min[2];
//	childsbox[2].maxPoint = max[2];
//	childsbox[3].minPoint = min[3];
//	childsbox[3].maxPoint = max[3];
//
//
//	childs[0] = Node(childsbox[0], Quadpart::EXTREMITY, capacity);
//	childs[1] = Node(childsbox[1], Quadpart::EXTREMITY, capacity);
//	childs[2] = Node(childsbox[2], Quadpart::EXTREMITY, capacity);
//	childs[3] = Node(childsbox[3], Quadpart::EXTREMITY, capacity);
//
//	if (nodeType != Quadpart::BASE) nodeType = Quadpart::MIDDLE;
//
//	N_Childs = 4;
//
//}
//
//bool Node::Insert(GameObject* gameObject)
//{
//
//	int numberofnodes = 0;
//	int auxcounter = 0;
//
//	switch (nodeType)
//	{
//
//	case Quadpart::BASE:
//
//		if (N_Childs == 0)
//		{
//			objects.push_back(gameObject);
//
//			if (objects.size() > capacity)
//			{
//				Split();
//				for (int a = 0; a < objects.size(); ++a)
//				{
//					numberofnodes = 0;
//					auxcounter = 0;
//
//					for (int i = 0; i < N_Childs; ++i)
//					{
//
//						if (childs[i].box.Intersects(objects[a]->Globalbbox))
//						{
//							numberofnodes++;
//							auxcounter = i;
//						}
//
//						if (numberofnodes > 1)
//							break;	
//					}
//
//					if (numberofnodes == 1)
//						childs[auxcounter].Insert(objects[a]);
//					
//					else if (numberofnodes > 1)
//						objects.push_back(objects[a]);	
//				}
//			}
//		}
//
//		else
//		{
//			for (int i = 0; i < N_Childs; ++i)
//			{
//				if (childs[i].box.Intersects(gameObject->Globalbbox))
//				{
//					numberofnodes++;
//					auxcounter = i;
//				}
//				if (numberofnodes > 1)
//					break;
//			}
//
//			if (numberofnodes == 1)
//				childs[auxcounter].Insert(gameObject);
//			
//			else if (numberofnodes > 1)
//				objects.push_back(gameObject);		
//		}
//		break;
//	}
//
//
//	return true;
//
//}
//
//std::vector<GameObject*> Node::ObjectsInside(Frustum frustum)
//{
//	std::vector<GameObject*> ret;
//	std::vector<GameObject*> auxVec;
//
//	if (!Intersect(frustum, box))
//	{
//		return ret;
//	}
//		
//
//	for (int i = 0; i < objects.size(); ++i)
//	{
//		ret.push_back(objects[i]);
//	}
//		
//
//	for (int i = 0; i < N_Childs; ++i)
//	{
//		auxVec = childs[i].ObjectsInside(frustum);
//
//		for (int j = 0; j < auxVec.size(); ++j)
//		{
//			ret.push_back(auxVec[j]);
//		}
//
//	}
//	return ret;
//}
//
//void Node::Clear()
//{
//	if (childs != NULL)
//		delete[] childs;
//
//	objects.clear();
//}
//
//bool Node::Intersect(Frustum frustum, AABB aabb)
//{
//	Plane planes[6];
//	float3 corners[8];
//
//	frustum.GetPlanes(planes);
//	aabb.GetCornerPoints(corners);
//
//	for (int p = 0; p < 6; ++p)
//	{
//		int inCount = 8;
//
//		for (int c = 0; c < 8; ++c)
//		{
//			if (planes[p].IsOnPositiveSide(corners[c]))
//				inCount--;
//		}
//		if (inCount == 0)
//			return false;
//	}
//	return true;
//
//}
//
//void Node::Draw()
//{
//	float3 points[8];
//
//
//	box.GetCornerPoints(points);
//
//		glBegin(GL_LINES);
//		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
//		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
//		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
//		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
//		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
//		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
//		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
//		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));
//
//		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
//		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
//		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
//		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
//		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
//		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
//		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
//		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));
//
//		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
//		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
//		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
//		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
//		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
//		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
//		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
//		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));
//
//		glEnd();
//
//	if (!this->DontDrawChilds)
//	{
//		for (int i = 0; i < N_Childs; ++i)
//			childs[i].Draw();
//	}
//}