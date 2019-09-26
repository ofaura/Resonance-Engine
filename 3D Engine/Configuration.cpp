#include "Configuration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui-1.72b/imgui.h"

Configuration::Configuration() {}

Configuration::~Configuration() {}

void Configuration::Start()
{
}

void Configuration::Draw()
{
	App->window->GetWindowSize(window_width, window_height);
	ImGui::SetNextWindowPos(ImVec2(100, 18), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)window_width / 5, (float)window_height - 18), ImGuiCond_Once);

	if (ImGui::Begin("Configuration", NULL, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SliderInt("Width", &width, 1, 1920);	
		ImGui::SliderInt("Height", &height, 1, 1080);
		ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);

		SDL_SetWindowBrightness(App->window->window, brightness);
		SDL_SetWindowSize(App->window->window, width, height);


		if (ImGui::Checkbox("Full Screen", &fullscreen))
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &resizable))
			SDL_SetWindowResizable(App->window->window, SDL_TRUE);

		

		

		
		ImGui::End();
	}


}

void Configuration::CleanUp()
{
}
