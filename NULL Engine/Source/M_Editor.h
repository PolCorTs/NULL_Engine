#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/include/imgui.h"
#include "Module.h"

class Configuration;

class GameObject;

class E_Panel;
class E_Toolbar;
class E_EngineConfiguration;
class E_Hierarchy;
class E_Inspector;
class E_Console;
class E_ImGuiDemo;
class E_About;

enum class IMGUI_STYLE
{
	CLASSIC,
	LIGHT,
	DARK,
	OTHER
};

class M_Editor : public Module
{
public:
	M_Editor(bool is_active = true);
	~M_Editor();

	bool			Init				(Configuration& config) override;
	bool			Start				() override;
	UPDATE_STATUS	PreUpdate			(float dt) override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			LoadConfiguration	(Configuration& root) override;
	bool			SaveConfiguration	(Configuration& root) const override;

public:
	bool			GetEvent							(SDL_Event* event) const;				// Will return false if there was no event to read.

	void			AddGuiPanel							(E_Panel* panel);						// Will add the E_Panel* passed as argument to the gui_panels vector.

	void			EditorShortcuts						();										// All the shortcuts related to the editor have been gathered here.
	void			CheckShowHideFlags					();										// Will check whether or not each of the panels must be enabled or disabled.

	bool			EditorIsBeingHovered				() const;								// Will check whether or not any of the editor panels is being hovered.
	bool			RenderGuiPanels						() const;								// Will call ImGui::Render() to render all the panels on the screen.
	bool			InitializeImGui						() const;								// Creates an ImGui Context and sets an initial configuration for it.

public:																							// --- Panel/Window Methods. Acts as an interface between other modules and the panels.
	bool			GetShowWorldGrid					() const;								// Will return true if the World Grid is being currently shown.
	bool			GetShowWorldAxis					() const;								// Will return true if the World Axis is being currently shown.
	bool			GetShowPrimitiveExamples			() const;								// Will return true if the Primitive Examples are being currently shown.
	void			SetShowWorldGrid					(bool set_to);							// Will Enable or Disable the World Grid depending on the passed argument.
	void			SetShowWorldAxis					(bool set_to);							// Will Enable or Disable the depending on the passed argument.
	void			SetShowPrimitiveExamples			(bool set_to);							// Will Enable or Disable the depending on the passed argument.

	void			UpdateFrameData						(int frames, int ms);					// Configuration: Passing the received frame data to the configuration editor module.
	void			AddInputLog							(uint key, uint state);					// Configuration: Receives an input key and a state and passes a log to the configuration editor module.

	void			AddConsoleLog						(const char* log);						// Console: Passing the received console log to the console editor module.

	GameObject*		GetRootGameObjectThroughEditor		() const;								// Hierarchy & inspector: Will return the current root GameObjcect.
	void			SetRootGameObjectThroughEditor		(GameObject* game_object);				// Hierarchy & inspector: Will set the scene's root GameObject with the passed one.
	GameObject*		GetSelectedGameObjectThroughEditor	() const;								// Hierarchy & Inspector: Will return the currently selected GameObject.
	void			SetSelectedGameObjectThroughEditor	(GameObject* game_object);				// Hierarchy & Inspector: Will set the scene's selected GameObject with the passed one.
	void			DeleteSelectedGameObject			();										// Hierarchy & Inspector: Will call the Module Scene's delete selected GameObject method.
	void			CreateGameObject					(const char* name, GameObject* parent);	// Hierarchy & Inspector: Will call the Module Scene's create GameObject method.

private:
	bool BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags = ImGuiWindowFlags_None);							// Generates a root window for docking.
	void BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_None, ImVec2 size = { 0.0f, 0.0f });	// Generates a new dockspace.

public:
	std::vector<E_Panel*>	gui_panels;												// Will store all the editor modules. Will be iterated for drawing all the panels.

	E_Toolbar*				toolbar; 
	E_EngineConfiguration*	configuration;
	E_Hierarchy*			hierarchy;
	E_Inspector*			inspector;
	E_Console*				console;
	E_ImGuiDemo*			imgui_demo;
	E_About*				about;

	ImVec4					clear_color;											// Will be used to set the clear color of the rendering environment.

	bool					show_configuration;										// Will keep track of whether or not to show the Configuration window.
	bool					show_hierarchy;											// Will keep track of whether or not to show the Hierarchy window.
	bool					show_inspector;											// Will keep track of whether or not to show the Inspector window.
	bool					show_console;											// Will keep track of whether or not to show the Console window.
	bool					show_imgui_demo;										// Will keep track of whether or not to show the ImGui Demo window.
	bool					show_about_popup;										// Will keep track of whether or not to show the About window popup.
	bool					show_close_app_popup;									// Will keep track of whether or not to show the Close App windowwindow.
};

#endif // !__M_EDITOR_H__