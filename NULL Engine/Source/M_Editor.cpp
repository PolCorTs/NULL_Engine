#include "Profiler.h"
#include "OpenGL.h"																	// ATTENTION: It was included AFTER ImGui.h
#include "ImGui.h"
#include "ImGui/include/imgui_internal.h"

#include "Application.h"															// ATTENTION: Globals.h already included in Module.h
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Camera3D.h"
#include "M_Input.h"
#include "M_Scene.h"
#include "M_FileSystem.h"

#include "Importer.h"

#include "GameObject.h"

#include "EditorPanel.h"
#include "E_MainMenuBar.h"
#include "E_Toolbar.h"
#include "E_Configuration.h"
#include "E_Hierarchy.h"
#include "E_Inspector.h"
#include "E_Console.h"
#include "E_Project.h"
#include "E_Scene.h"
#include "E_Game.h"
#include "E_ImGuiDemo.h"
#include "E_About.h"
#include "E_LoadFile.h"

#include "M_Editor.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/glew/libx86/glew32.lib")

M_Editor::M_Editor(bool is_active) : Module("Editor", is_active),
clear_color		(0.0f, 0.0f, 0.0f, 1.0f),
main_menu_bar	(nullptr),
toolbar			(nullptr),
configuration	(nullptr),
hierarchy		(nullptr),
inspector		(nullptr),
console			(nullptr),
project			(nullptr),
scene			(nullptr),
game			(nullptr),
imgui_demo		(nullptr),
about			(nullptr),
load_file		(nullptr)
{
	main_menu_bar	= new E_MainMenuBar();
	toolbar			= new E_Toolbar();
	configuration	= new E_Configuration();
	hierarchy		= new E_Hierarchy();
	inspector		= new E_Inspector();
	console			= new E_Console();
	project			= new E_Project();
	scene			= new E_Scene();
	game			= new E_Game();
	imgui_demo		= new E_ImGuiDemo();
	about			= new E_About();
	load_file		= new E_LoadFile();

	AddEditorPanel(main_menu_bar);
	AddEditorPanel(toolbar);
	AddEditorPanel(configuration);
	AddEditorPanel(hierarchy);
	AddEditorPanel(inspector);
	AddEditorPanel(console);
	AddEditorPanel(project);
	AddEditorPanel(game);
	AddEditorPanel(scene);
	AddEditorPanel(imgui_demo);
	AddEditorPanel(about);
	AddEditorPanel(load_file);

	show_configuration		= true;
	show_hierarchy			= true;
	show_inspector			= true;
	show_console			= true;
	show_imgui_demo			= false;
	show_about_popup		= false;
	show_close_app_popup	= false;
	show_load_file_popup	= false;
}

M_Editor::~M_Editor()
{

}

bool M_Editor::Init(ParsonNode& config)
{
	bool ret = true;

	return ret;
}

bool M_Editor::Start()
{
	bool ret = true;

	InitializeImGui();
	
	return ret;
}

UPDATE_STATUS M_Editor::PreUpdate(float dt)
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	EditorShortcuts();
	CheckShowHideFlags();

	return ret;
}

UPDATE_STATUS M_Editor::Update(float dt)
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	return ret;
}

UPDATE_STATUS M_Editor::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Editor PostUpdate", Profiler::Color::IndianRed);
	
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;
	
	ImGuiIO& io = ImGui::GetIO();

	//Start Dear ImGui's frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	if (BeginRootWindow(io, "Root window", true, ImGuiWindowFlags_MenuBar))
	{
		bool draw = true;
		for (uint i = 0; i < editor_panels.size(); ++i)
		{
			if (editor_panels[i]->IsActive())
			{
				draw = editor_panels[i]->Draw(io);

				if (!draw)
				{
					ret = UPDATE_STATUS::STOP;
					LOG("[EDITOR] Exited through %s Panel", editor_panels[i]->GetName());
					break;
				}
			}
		}
		
		ImGui::End();
	}
	
	return ret;
}

bool M_Editor::CleanUp()
{
	for (uint i = 0; i < editor_panels.size(); ++i)
	{
		editor_panels[i]->CleanUp();
		RELEASE(editor_panels[i]);
	}

	editor_panels.clear();
	
	// ImGui CleanUp()
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	return true;
}

bool M_Editor::LoadConfiguration(ParsonNode& root)
{
	bool ret = true;

	return ret;
}

bool M_Editor::SaveConfiguration(ParsonNode& root) const
{
	bool ret = true;

	return ret;
}

// -------------- EDITOR METHODS --------------
bool M_Editor::GetEvent(SDL_Event* event) const
{
	return ImGui_ImplSDL2_ProcessEvent(event);											
}

void M_Editor::AddEditorPanel(EditorPanel* panel)
{
	editor_panels.push_back(panel);
}

void M_Editor::EditorShortcuts()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
	{
		show_configuration = !show_configuration;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
	{
		show_hierarchy = !show_hierarchy;
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN)
	{
		show_inspector = !show_inspector;
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN)
	{
		show_console = !show_console;
	}

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_STATE::KEY_DOWN)
	{
		show_imgui_demo = !show_imgui_demo;
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_STATE::KEY_DOWN)
	{
		show_about_popup = !show_about_popup;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN)
	{
		if (show_about_popup || show_load_file_popup)
		{
			show_about_popup		= false;
			show_load_file_popup	= false;
		}
		else
		{
			show_close_app_popup = !show_close_app_popup;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_STATE::KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_DOWN)
		{
			App->scene->SaveScene();
		}

		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
		{
			show_load_file_popup = true;
		}
	}
}

void M_Editor::CheckShowHideFlags()
{	
	show_configuration		?	configuration->Enable()	: configuration->Disable();					// Engine Configuration
	show_hierarchy			?	hierarchy->Enable()		: hierarchy->Disable();						// Hierarchy
	show_inspector			?	inspector->Enable()		: inspector->Disable();						// Inspector
	show_console			?	console->Enable()		: console->Disable();						// Console
	show_project			?	project->Enable()		: project->Disable();						// Project
	show_imgui_demo			?	imgui_demo->Enable()	: imgui_demo->Disable();					// ImGui Demo
	show_about_popup		?	about->Enable()			: about->Disable();							// About Popup
	show_load_file_popup	?	load_file->Enable()		: load_file->Disable();						// Load File
}

bool M_Editor::EditorIsBeingHovered() const
{
	for (uint i = 0; i < editor_panels.size(); ++i)
	{
		if (editor_panels[i]->IsHovered())
		{
			return true;
		}
	}

	return false;
}

bool M_Editor::EditorSceneIsBeingClicked() const
{
	return App->editor->scene->IsClicked();
}

bool M_Editor::RenderEditorPanels() const
{
	// Rendering all ImGui elements
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Updating and rendering additional platform windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return true;
}

bool M_Editor::InitializeImGui() const
{
	bool ret = true;

	// Setting up Dear ImGui's context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();																// Needs to be called multiple times during a frame to update IO correctly.
	(void)io;

	io.IniFilename = CONFIGURATION_PATH "imgui.ini";

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;										// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;											// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;											// Enable MultiViewport / Platform Windows

	ImGui::StyleColorsDark();																	// Setting up Dear ImGui's style.
	ImGuiStyle& style = ImGui::GetStyle();														// Tweaking the platform windows to look identical to regular ones.

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)										// Setting some configuration parameters in case the ViewportsEnable flag is enabled. 
	{																							// 
		style.WindowRounding = 0.0f;															// Setting the windows to have no rounding on their corners.
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;												// Setting the windows Alpha to 255, making them completely opaque.
	}																							// -----------------------------------------------

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->context);				// Setting up Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init(0);																	// -------------------------------------

	return ret;
}

bool M_Editor::GetShowWorldGrid() const
{
	return App->renderer->GetDrawWorldGrid();
}

bool M_Editor::GetShowWorldAxis() const
{
	return App->renderer->GetDrawWorldAxis();
}

bool M_Editor::GetShowPrimitiveExamples() const
{
	return App->renderer->GetDrawPrimitiveExamples();
}

void M_Editor::SetShowWorldGrid(bool set_to)
{
	App->renderer->SetDrawWorldGrid(set_to);
}

void M_Editor::SetShowWorldAxis(bool set_to)
{
	App->renderer->SetDrawWorldAxis(set_to);
}

void M_Editor::SetShowPrimitiveExamples(bool set_to)
{
	App->renderer->SetDrawPrimtiveExamples(set_to);
}

void M_Editor::UpdateFrameData(int frames, int ms)
{
	configuration->UpdateFrameData(frames, ms);
}

void M_Editor::AddConsoleLog(const char* log)
{
	if (editor_panels.size() > 0)
	{
		if (console != nullptr)
		{
			console->AddLog(log);
		}
	}
}

void M_Editor::AddInputLog(uint key, uint state)
{
	char input[128];
	const char* states[] = { "IDLE", "DOWN", "REPEAT", "UP" };										// We add the 4 main key states. In practice "IDLE" will not be displayed.
	
	if (configuration != nullptr)
	{
		if (key < App->input->GetMaxNumScancodes())
		{
			const char* key_name = SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)key));		// Through the scancode it is possible to get a string with the name of the key.

			sprintf_s(input, 128, "[KEY] %02u %s - %s\n", key, key_name, states[state]);
		}
		else
		{
			uint mouse_button = key - App->input->GetMaxNumScancodes();

			sprintf_s(input, 128, "[MOUSE] %02u - %s\n", mouse_button, states[state]);
		}

		configuration->AddInputLog(input);
	}
}

GameObject* M_Editor::GetSceneRootThroughEditor() const
{
	return App->scene->GetSceneRoot();
}

void M_Editor::SetSceneRootThroughEditor(GameObject* game_object)
{
	App->scene->SetSceneRoot(game_object);
}

GameObject* M_Editor::GetSelectedGameObjectThroughEditor() const
{
	return App->scene->GetSelectedGameObject();
}

void M_Editor::SetSelectedGameObjectThroughEditor(GameObject* game_object)
{
	App->scene->SetSelectedGameObject(game_object);
}

void M_Editor::DeleteSelectedGameObject()
{
	App->scene->DeleteSelectedGameObject();
}

void M_Editor::CreateGameObject(const char* name, GameObject* parent)
{
	App->scene->CreateGameObject(name, parent);
}

void M_Editor::SetCurrentCameraThroughEditor(GameObject* game_object)
{
	App->camera->SetCurrentCamera(game_object);
}

void M_Editor::SetMasterCameraThroughEditor()
{
	App->camera->SetCurrentCamera(App->scene->GetMasterCamera());
}

bool M_Editor::SelectedIsSceneRoot() const
{
	return (App->scene->GetSelectedGameObject() == App->scene->GetSceneRoot());
}

void M_Editor::LoadFileThroughEditor(const char* path)
{
	std::string extension = App->file_system->GetFileExtension(path);

	if (extension == "json" || extension == "JSON")
	{
		App->scene->LoadScene(path);
	}
	else
	{
		Importer::ImportFile(path);
	}
}

bool M_Editor::BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags)
{
	bool ret = true;
	
	ImGuiViewport* viewport = ImGui::GetWindowViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
					| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
					| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ret = ImGui::Begin(window_id, &ret, window_flags);
	ImGui::PopStyleVar(3);

	if (docking)
	{	
		BeginDockspace(io, window_id, ImGuiDockNodeFlags_PassthruCentralNode);
	}

	return ret;
}	
	
void M_Editor::BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags, ImVec2 size)
{
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dckspace_id = ImGui::GetID(dockspace_id);
		ImGui::DockSpace(dckspace_id, size, docking_flags);
	}
}