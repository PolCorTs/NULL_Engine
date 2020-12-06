#ifndef __E_TOOLBAR_H__
#define __E_TOOLBAR_H__

#include "EditorPanel.h"

class E_Toolbar : public EditorPanel
{
public:
	E_Toolbar();
	~E_Toolbar();

	bool Draw		(ImGuiIO& io) override;
	bool CleanUp	() override;

private:
	bool FileMainMenuItem			();
	bool EditMainMenuItem			();
	bool WindowMainMenuItem			();
	bool ViewMainMenuItem			();
	bool GameObjectsMainMenuItem	();
	bool HelpMainMenuItem			();

	bool CloseAppPopup				();
};

#endif // !__E_TOOLBAR_H__