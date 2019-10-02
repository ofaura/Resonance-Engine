#ifndef __Game__H__
#define __Game__H__

#include "EditorManager.h"

class Game : public EditorElement
{
public:

	Game(bool is_visible = true);
	virtual ~Game();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	ImVec2 size;
	ImVec2 saved_size;
	ImVec2 position;
};

#endif __Game__H__
