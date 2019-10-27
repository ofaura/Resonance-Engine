#ifndef __Inspector__H__
#define __Inspector__H__

#include "EditorManager.h"

class GameObject;

class Inspector : public EditorElement
{
public:

	Inspector(bool is_visible = true);
	virtual ~Inspector();

public:
	void Start();
	void Draw();
	void CleanUp();

private:

	bool changedName = false;

};

#endif __Inspector__H__
