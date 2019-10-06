#include "Configuration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Brofiler/Brofiler.h"


Configuration::Configuration(bool is_visible) : EditorElement(is_visible) {}

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

	SDL_GetVersion(&sdl_version);

	LOG_CONSOLE("\nSDL Version: %d.%d.%d", (int)sdl_version.major, (int)sdl_version.minor, (int)sdl_version.patch);
	LOG_CONSOLE("CPUs: %d   Cache: %d kb", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	LOG_CONSOLE("GPU: %s", glGetString(GL_RENDERER));
	LOG_CONSOLE("Brand: %s", glGetString(GL_VENDOR));
	LOG_CONSOLE("Version: %s \n\n",glGetString(GL_VERSION));
}

void Configuration::Draw()
{
	if (!active) return;

	if (ImGui::Begin("Configuration", &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		App->window->GetWindowSize(width, height);
		if (ImGui::SliderInt("Width", &width, 1, 1920))
			SDL_SetWindowSize(App->window->window, width, height);

		if (ImGui::SliderInt("Height", &height, 1, 1080))
			SDL_SetWindowSize(App->window->window, width, height);

		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
			SDL_SetWindowBrightness(App->window->window, brightness);

		if (ImGui::Checkbox("Full Screen", &fullscreen))
			App->window->SetFullScreen(fullscreen);

		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
		
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);
	}

	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::SliderInt("Max FPS", &App->maxFPS, 1, 200);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%d", App->maxFPS);

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", App->fps_vec[App->fps_vec.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->fps_vec[0], App->fps_vec.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", App->ms_vec[App->ms_vec.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &App->ms_vec[0], App->ms_vec.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("SDL Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255.0f, 0.0f, 255.0f, 255.00f), "%d.%d.%d", (int)sdl_version.major, (int)sdl_version.minor, (int)sdl_version.patch);
		
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

		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &video_mem_budget);
		ImGui::Text("VRAM Budget");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", float(video_mem_budget) / (1024.0f));

		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &video_mem_available);
		ImGui::Text("VRAM Available");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", float(video_mem_usage) / (1024.f));

		video_mem_usage = video_mem_budget - video_mem_available;
		ImGui::Text("VRAM Usage");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "%f", float(video_mem_available) / (1024.f));
	}

	if (ImGui::CollapsingHeader("Input"))
	{
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", App->editor->test_io->MousePos.x, App->editor->test_io->MousePos.y);
		else
			ImGui::Text("Mouse pos: <INVALID>");
		ImGui::Text("Mouse delta: (%g, %g)", App->editor->test_io->MouseDelta.x, App->editor->test_io->MouseDelta.y);
		ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(App->editor->test_io->MouseDown); i++) if (App->editor->test_io->MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, App->editor->test_io->MouseDownDuration[i]); }
		ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(App->editor->test_io->MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(App->editor->test_io->MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
		ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(App->editor->test_io->MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
	}

	ImGui::End();
}

void Configuration::CleanUp()
{
}

