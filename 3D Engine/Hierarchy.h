#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"
#include "Objects3D.h"

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


};

#endif __Hierarchy__H__
