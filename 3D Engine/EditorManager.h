#ifndef __EditorManager__H__
#define __EditorManager__H__

#include "Module.h"
#include "imgui-1.72b/imgui.h"
#include "SDL/include/SDL_events.h"

#include <list>

class Hierarchy;
class Configuration;
class About;
class Console;
class Game;

class EditorElement
{
public:
	EditorElement(bool is_visible = true) { active = is_visible; };
	virtual ~EditorElement() {};

	virtual void Start() {};
	virtual void Draw() {};
	virtual void CleanUp() {};

	bool active = false;
};

class EditorManager : public Module
{
public:
	
	EditorManager(bool start_enabled = true);
	~EditorManager();

public:

	bool Init(json file);
	bool Start();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void HandleInput(SDL_Event* event);

	void Draw();

private:

	void AddEditorElement(EditorElement* element);
	void StyleEditor(ImGuiStyle* ref = NULL);
	void HelpMarker(const char* desc);

public:

	ImGuiIO* test_io;

private:
	
	ImGuiIO* io;
	
	ImVec4 clear_color = ImColor(25, 25, 25, 128);
	//ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 0.50f);

	bool show_demo_window = false;
	bool show_style_editor = false;
	bool p_open = true;

	list<EditorElement*> editor_elements;

	Hierarchy*		hierarchy = nullptr;
	Configuration*	configuration = nullptr;
	About*			about = nullptr;
	Console*		console = nullptr;
	Game*			game = nullptr;
};

#endif __EditorManager__H__
