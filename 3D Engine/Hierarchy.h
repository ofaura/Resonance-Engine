#ifndef __Hierarchy__H__
#define __Hierarchy__H__

#include "EditorManager.h"

class Hierarchy : public EditorElement
{
public:

	Hierarchy();
	virtual ~Hierarchy();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	int   window_width = 0;
	int   window_height = 0;
};

#endif __Hierarchy__H__
