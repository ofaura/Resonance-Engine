#include "Console.h"
#include "Application.h"
#include "ModuleWindow.h"

Console::Console(bool is_visible) : EditorElement(is_visible) {}

Console::~Console() {}

void Console::Start()
{
	Clear();
}

void Console::Draw()
{
	if (!active) return;

	DrawLogs();
	
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 6));

		for (list<string>::iterator item = console_logs.begin(); item != console_logs.end(); ++item)
		{
			ImVec4 color = ImColor(255, 255, 255, 255);
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted((*item).c_str());
			ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

	ImGui::End();
}

void Console::CleanUp()
{
}

void Console::AddNewLog(string text)
{
	console_logs.push_back(text);

	if (console_logs.size() > MAX_CONSOLE_LOGS)
	{
		console_logs.pop_front();
	}
}

void Console::DrawLogs()
{
	for (list<string>::iterator item = App->log_list.begin(); item != App->log_list.end(); ++item)
	{
		AddNewLog((*item));
	}

	App->log_list.clear();
}

void Console::Clear()
{
	console_logs.clear();
}


