#ifndef __Console__H__
#define __Console__H__

#include "EditorManager.h"
#include <list>

#define MAX_CONSOLE_LOGS 100

class Console : public EditorElement
{
public:

	Console(bool is_visible = true);
	virtual ~Console();

public:

	void Start();
	void Draw();
	void CleanUp();

private:
	list<char*> console_logs;

private:

	void AddNewLog(char* text);
	void DrawLogs();
	void Clear();
};

#endif __Console__H__
