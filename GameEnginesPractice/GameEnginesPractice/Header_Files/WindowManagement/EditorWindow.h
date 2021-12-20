#pragma once

#include "Render/RenderEngine.h"
#include "OgreSceneNode.h"

class EditorWindow
{
public:
	EditorWindow(RenderEngine* renderEngine, std::string criptRootPath);
	~EditorWindow();

	void Update();
	bool IsQuit() const { return m_bIsClosed; };
	bool IsFrozen() const { return m_bIsFrozen; };

	void SetSelection(Ogre::SceneNode* newSelection) { m_pCurrentSelection = newSelection; };
private:
	std::string m_strScriptRootPath;
	bool m_bIsFrozen;
	bool m_bIsClosed;

	RenderEngine* m_pRenderEngine;

	void ProcessSDLInput();

	void InitWindow();
	void InitImGui();

	void RenderGui();
	void StartGuiUpdate();
	void EndGuiUpdate();
	void DisplaySelectionParameters();
	void DisplayAllScripts();
	void DisplayFreezeBtn();
	void DisplayMenuBar();

	SDL_Window* m_SDLWindow;
	SDL_GLContext m_GLContext;
	Ogre::SceneNode* m_pCurrentSelection;
};

