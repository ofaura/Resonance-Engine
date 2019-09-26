#ifndef __EditorManager__H__
#define __EditorManager__H__

#include "Module.h"
#include "imgui-1.72b/imgui.h"

#include <list>

class Hierarchy;
class Configuration;
class About;

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

	bool Init();
	bool Start();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);

private:

	void AddEditorElement(EditorElement* element);
	void StyleEditor(ImGuiStyle* ref = NULL);
	void HelpMarker(const char* desc);

private:
	ImGuiIO* io;
	
	bool show_demo_window = false;
	bool show_style_editor = false;

	list<EditorElement*> editor_elements;

	Hierarchy*		hierarchy = nullptr;
	Configuration*	configuration = nullptr;
	About*			about = nullptr;
};

#endif __EditorManager__H__
