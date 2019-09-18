#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__


#include "Globals.h"
#include "Module.h"

#include "imgui-1.72b/imgui.h"

#include "imgui-1.72b/examples/imgui_impl_sdl.h"
#include "imgui-1.72b/examples/imgui_impl_opengl2.h"

#include "glut/glut.h"
//#include "imgui-1.72b/examples/libs/glfw/include/GLFW/glfw3.h"
//#include "imgui-1.72b/examples/libs/glfw/include/GLFW/glfw3native.h"


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
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	//update_status PostUpdate(float dt);


private:
	ImGuiIO* io = nullptr;

};

#endif // !__MODULEGUI_H__

