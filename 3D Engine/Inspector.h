#ifndef __Inspector__H__
#define __Inspector__H__

#include "EditorManager.h"

class Inspector : public EditorElement
{
public:

	Inspector(bool is_visible = true);
	virtual ~Inspector();

public:
	void Start();
	void Draw();
	void CleanUp();
};

#endif __Inspector__H__
