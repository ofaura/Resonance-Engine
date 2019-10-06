#ifndef __Game__H__
#define __Game__H__

#include "EditorManager.h"
#include <float.h>

class Game : public EditorElement
{
public:

	Game(bool is_visible = true);
	virtual ~Game();

public:
	void Start();
	void Draw();
	void CleanUp();
	const ImVec4 GetWindowRect();

private:
	ImVec2 size;
	ImVec2 new_size = { 0,0 };
	ImVec2 position;

	bool	wireframe = false;
	bool	depth_test = true;
	bool	cull_face = true;
	bool	lighting = true;
	bool	color_material = true;
	bool	texture2D = true;
};

#endif __Game__H__
