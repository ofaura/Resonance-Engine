#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"
#include "Objects3D.h"

class GameObject;
class TreeNodes;

class Hierarchy : public EditorElement
{
public:

	Hierarchy(bool is_visible = true);
	virtual ~Hierarchy();

public:

	void Start();
	void Draw();
	void CleanUp();

private:

	void AddSelectedGameObject(GameObject gameObject);
	void SetSelected(const bool& selected);

private:
	vector<GameObject> selected;
};

#endif __Hierarchy__H__
