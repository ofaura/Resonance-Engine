#ifndef __About__H__
#define __About__H__

#include "EditorManager.h"

class About : public EditorElement
{
public:

	About(bool is_visible = true);
	virtual ~About();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	int   window_width = 0;
	int   window_height = 0;
};

#endif __About__H__
