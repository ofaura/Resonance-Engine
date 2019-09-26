#include "Configuration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "glew/include/GL/glew.h"

Configuration::Configuration() {}

Configuration::~Configuration() {}

void Configuration::Start()
{
	if (SDL_Has3DNow())
		caps += "3DNow, ";
	if (SDL_HasAltiVec())
		caps += "AltiVec, ";
	if (SDL_HasAVX())
		caps += "AVX, ";
	if (SDL_HasAVX2())
		caps += "AVX2, ";
	if (SDL_HasMMX())
		caps += "MMX, ";
	if (SDL_HasRDTSC())
		caps += "RDTSC, ";
	if (SDL_HasSSE())
		caps += "SSE, ";
	if (SDL_HasSSE2())
		caps += "SSE2, ";
	if (SDL_HasSSE3())
		caps += "SSE3, ";
	if (SDL_HasSSE41())
		caps += "SSE41, ";
	if (SDL_HasSSE42())
		caps += "SSE42";

}

void Configuration::Draw()
{
	App->window->GetWindowSize(window_width, window_height);
	ImGui::SetNextWindowPos(ImVec2(100, 18), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)window_width / 5, (float)window_height - 18), ImGuiCond_Once);

	if (ImGui::Begin("Configuration"))
	{
		ImGui::SliderInt("Width", &width, 1, 1920);	
		ImGui::SliderInt("Height", &height, 1, 1080);
		ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);

		SDL_SetWindowBrightness(App->window->window, brightness);
		SDL_SetWindowSize(App->window->window, width, height);


		if (ImGui::Checkbox("Full Screen", &fullscreen))
			App->window->SetFullScreen(fullscreen);

		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
		
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", SDL_GetCPUCount());
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(Cache: %d kb)", SDL_GetCPUCacheLineSize());

		ImGui::Text("System RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%f Gb", SDL_GetSystemRAM() / 1024.0f);

		ImGui::Text("Caps: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), caps.c_str());

		ImGui::Separator();

		ImGui::Text("GPU: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));

		ImGui::Text("Brand: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));

		ImGui::Text("Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VERSION));
	}

	ImGui::End();

}

void Configuration::CleanUp()
{
}
