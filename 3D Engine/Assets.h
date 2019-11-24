#ifndef __Assets__H__
#define __Assets__H__

#include "EditorManager.h"

class Assets : public EditorElement
{
public:
	Assets(bool is_visible = true);
	virtual ~Assets();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	string currentDirectory;
	string parentDirectory;
	string name;

	vector<string> files;
	vector<string> directories;
};

#endif __Assets__H__