#include "EditorManager.h"
#include "Application.h"
#include "Hierarchy.h"
#include "Configuration.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "About.h"
#include "Console.h"
#include "Game.h"
#include "Inspector.h"
#include "ModuleSceneIntro.h"

#include "imgui-1.72b/examples/imgui_impl_sdl.h"
#include "imgui-1.72b/examples/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"

#include "Brofiler/Brofiler.h"

EditorManager::EditorManager(bool start_enabled) : Module("Editor", start_enabled) {}

EditorManager::~EditorManager() {}

bool EditorManager::Init(json file)
{
	hierarchy = new Hierarchy(true);
	configuration = new Configuration(true);
	about = new About(false);
	console = new Console(true);
	game = new Game(true);
	inspector = new Inspector(true);

	AddEditorElement(hierarchy);
	AddEditorElement(configuration);
	AddEditorElement(about);
	AddEditorElement(console);
	AddEditorElement(game);
	AddEditorElement(inspector);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->DisplaySize.x = SCREEN_WIDTH;             // set the current display width
	io->DisplaySize.y = SCREEN_HEIGHT;             // set the current display height here
	 // Build and load the texture atlas into a texture
	 // (In the examples/ app this is usually done within the ImGui_ImplXXX_Init() function from one of the demo Renderer)
	int width, height;
	unsigned char* pixels = NULL;
	io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return true;
}

bool EditorManager::Start()
{
	for (list<EditorElement*>::iterator item = editor_elements.begin(); item != editor_elements.end(); ++item)
	{
		(*item)->Start();
	}

	return true;
}

bool EditorManager::CleanUp()
{
	for (list<EditorElement*>::iterator item = editor_elements.begin(); item != editor_elements.end(); ++item)
	{
		(*item)->CleanUp();
		delete (*item);
	}

	editor_elements.clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

update_status EditorManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("EditorManager PreUpdate", Profiler::Color::MediumVioletRed)

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status EditorManager::Update(float dt)
{
	BROFILER_CATEGORY("EditorManager Update", Profiler::Color::MediumVioletRed)

	update_status ret = UPDATE_CONTINUE;
	
	bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	/*if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)*/
		dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	// Main Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				ret = UPDATE_STOP;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Style Editor")) 
				show_style_editor = true;		

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Configuration", NULL, &configuration->active);

			ImGui::MenuItem("Hierarchy", NULL, &hierarchy->active);
			
			ImGui::MenuItem("Game", NULL, &game->active);

			ImGui::MenuItem("Console", NULL, &console->active);

			ImGui::MenuItem("Inspector", NULL, &inspector->active);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObjects"))
		{
			if (ImGui::MenuItem("Cube"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::CUBE, vec3(0,0,0), vec3(1,1,1));
	
			if (ImGui::MenuItem("Tetrahedron"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::TETRAHEDRON, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Octohedron"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::OCTOHEDRON, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Dodecahedron"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::DODECAHEDRON, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Icosahedron"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::ICOSAHEDRON, vec3(0, 0, 0), vec3(1, 1, 1));
			
			if (ImGui::MenuItem("Sphere"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::SPHERE, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Cylinder"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::CYLINDER, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Cone"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::CONE, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Plane"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::PLANE, vec3(0, 0, 0), vec3(1, 1, 1));

			if (ImGui::MenuItem("Torus"))
				App->scene_intro->CreateObject3D(SHAPE_TYPE::TORUS, vec3(0, 0, 0), vec3(1, 1, 1));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show Demo"))
				show_demo_window = true;

			if (ImGui::MenuItem("Documentation"))
				ShellExecuteA(NULL, "open", "https://github.com/ofaura/3D-Engine/wiki", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Download latest"))
				ShellExecuteA(NULL, "open", "https://github.com/ofaura/3D-Engine/releases", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Report a bug"))
				ShellExecuteA(NULL, "open", "https://github.com/ofaura/3D-Engine/issues", NULL, NULL, SW_SHOWNORMAL);

			ImGui::MenuItem("About", NULL, &about->active);
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if (show_style_editor)
		StyleEditor();

	test_io = &io;

	for (list<EditorElement*>::iterator item = editor_elements.begin(); item != editor_elements.end(); ++item)
	{
		(*item)->Draw();
	}

	return ret;
}

update_status EditorManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("EditorManager PostUpdate", Profiler::Color::MediumVioletRed)

	return UPDATE_CONTINUE;
}

void EditorManager::HandleInput(SDL_Event * event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

void EditorManager::Draw()
{
	ImGui::Render();
	// Rendering
	glViewport(0, 0, (int)test_io->DisplaySize.x, (int)test_io->DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const ImVec4 EditorManager::GetGameWindowRect()
{
	return game->GetWindowRect();
}

void EditorManager::AddLOG(char* log, ...)
{
	// FIXME-OPT
	char buf[1024];
	va_list args;
	va_start(args, log);
	vsnprintf(buf, IM_ARRAYSIZE(buf), log, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	App->log_list.push_back(Strdup(buf));
}


void EditorManager::AddEditorElement(EditorElement* element)
{
	editor_elements.push_back(element);
}

void EditorManager::StyleEditor(ImGuiStyle* ref)
{
	ImGui::Begin("Style Editor", &show_style_editor);
	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
	ImGuiStyle& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;

	// Default to using internal storage as reference
	static bool init = true;
	if (init && ref == NULL)
		ref_saved_style = style;
	init = false;
	if (ref == NULL)
		ref = &ref_saved_style;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

	bool window_border = (style.WindowBorderSize > 0.0f);
	bool child_border = (style.ChildBorderSize > 0.0f);
	bool frame_border = (style.FrameBorderSize > 0.0f);
	bool popup_border = (style.PopupBorderSize > 0.0f);
	bool tab_border = (style.TabBorderSize > 0.0f);


	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Sizes"))
		{
			ImGui::Text("Borders");
			if (ImGui::Checkbox("WindowBorder", &window_border))
				style.WindowBorderSize = window_border ? 1.0f : 0.0f;
			if (ImGui::Checkbox("ChildBorderSize", &child_border))
				style.ChildBorderSize = child_border ? 1.0f : 0.0f;
			if (ImGui::Checkbox("PopupBorder", &popup_border))
				style.PopupBorderSize = popup_border ? 1.0f : 0.0f;
			if (ImGui::Checkbox("FrameBorder", &frame_border))
				style.FrameBorderSize = frame_border ? 1.0f : 0.0f;
			if (ImGui::Checkbox("TabBorderSize", &tab_border))
				style.TabBorderSize = tab_border ? 1.0f : 0.0f;
			ImGui::Text("Rounding");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::Text("Alignment");
			ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
			ImGui::Combo("WindowMenuButtonPosition", (int*)&style.WindowMenuButtonPosition, "Left\0Right\0");
			ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
			ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
			ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
			ImGui::Text("Safe Area Padding"); ImGui::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
			ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Colors"))
		{
			static int output_dest = 0;
			static bool output_only_modified = true;
			if (ImGui::Button("Export Unsaved"))
			{
				if (output_dest == 0)
					ImGui::LogToClipboard();
				else
					ImGui::LogToTTY();
				ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const ImVec4& col = style.Colors[i];
					const char* name = ImGui::GetStyleColorName(i);
					if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
						ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
				}
				ImGui::LogFinish();
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
			ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

			static ImGuiTextFilter filter;
			filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

			static ImGuiColorEditFlags alpha_flags = 0;
			ImGui::RadioButton("Opaque", &alpha_flags, 0); ImGui::SameLine();
			ImGui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ImGui::SameLine();
			ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf); ImGui::SameLine();
			HelpMarker("In the color list:\nLeft-click on colored square to open color picker,\nRight-click to open edit options menu.");

			ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
			ImGui::PushItemWidth(-160);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName(i);
				if (!filter.PassFilter(name))
					continue;
				ImGui::PushID(i);
				ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
				if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
				{
					// Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
					// Read the FAQ and misc/fonts/README.txt about using icon fonts. It's really easy and super convenient!
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i];
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) style.Colors[i] = ref->Colors[i];
				}
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				ImGui::TextUnformatted(name);
				ImGui::PopID();
			}
			ImGui::PopItemWidth();
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Rendering"))
		{
			ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines); ImGui::SameLine(); HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");
			ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, "%.2f", 2.0f);
			if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;
			ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::PopItemWidth();

	ImGui::End();
}

void EditorManager::HelpMarker(const char * desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
