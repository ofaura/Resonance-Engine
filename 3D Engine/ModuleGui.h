#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Globals.h"
#include "Module.h"

#include "imgui-1.72b/imgui.h"

#include "imgui-1.72b/examples/imgui_impl_sdl.h"
#include "imgui-1.72b/examples/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"


class ModuleGUI : public Module
{
public:
	// CONSTRUCTOR
	ModuleGUI(Application* app, bool start_enabled = true);
	//DESTRUCTOR
	~ModuleGUI();

public:
	bool Init();
	bool Start();
	void Draw();
	
	update_status Update(float dt);

private:
	ImGuiIO* io = nullptr;

	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_close_app_window = true;

};

#endif // !__MODULEGUI_H__

