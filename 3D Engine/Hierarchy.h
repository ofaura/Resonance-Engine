#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"
#include "Objects3D.h"

class GameObject;


class Hierarchy : public EditorElement
{
public:

	Hierarchy(bool is_visible = true);
	virtual ~Hierarchy();

public:

	void Start();
	void Draw();
	void CleanUp();

	void HandleHierarchyGO(GameObject* gameObject, int& id);

private:

	bool isSelected = false;
	bool openNode = false;
	GameObject* draggedGO = nullptr;
};

#endif __Hierarchy__H__
