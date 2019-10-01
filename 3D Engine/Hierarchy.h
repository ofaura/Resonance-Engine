#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"

class Hierarchy : public EditorElement
{
public:

	Hierarchy(bool is_visible = true);
	virtual ~Hierarchy();

public:
	void Start();
	void Draw();
	void CleanUp();
};

#endif __Hierarchy__H__
