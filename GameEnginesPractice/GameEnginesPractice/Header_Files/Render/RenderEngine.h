#pragma once

#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorManager2.h"
#include "OgreRay.h"
#include "OgreSceneQuery.h"

#include "GeneralDefines.h"

#include "SceneObject.h"
#include "SceneObjectProducer.h"
#include "ResourceManager.h"

#include "OGRE/RenderSystems/Direct3D11/OgreD3D11Plugin.h"
#include "OGRE/RenderSystems/GL3Plus/OgreGL3PlusPlugin.h"

#include "RenderThread.h"
#include <memory>
#include <filesystem>

#include "SDL.h"
#include "SDL_opengl.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef _WINDOWS
#include "SDL_syswm.h"
#endif 

class RenderEngine
{
	friend class RenderThread;

public:
	RenderEngine(ResourceManager* pResourceManager);
	~RenderEngine();
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine& operator=(const RenderEngine&) = delete;

	void Update();

	bool GetQuit() { return m_bQuit; }
	void SetQuit(bool bQuit) { m_bQuit = bQuit; }
	
	void RT_SetCurrentMouseState(bool isPressed, Ogre::Vector2 mousePos);
	SceneObject* RT_CreateSceneObject(Ogre::String actorName, Ogre::String meshName);
	std::vector<SceneObject*>* GetRenderedObjects() { return m_pSceneObjectProducer->GetLoadedObjects(); }

	RenderThread* GetRT() const { return m_pRT.get(); }
	Ogre::Camera* GetMainCamera() { return m_pCamera.get(); }

	bool IsFrozen() { return m_bIsFreeze; }
	
	bool IsInitialized() { return m_bIsInitialized; }

	void RT_SDLClenup();

private:
	bool SetOgreConfig();

	void RT_Init();
	void RT_SetupDefaultCamera();
	void RT_SetupDefaultCompositor();
	void RT_LoadDefaultResources();
	void RT_LoadOgreHead();
	void RT_SetupDefaultLight();

	void RT_InitSDL();

	void RaycastToMouse();

	void StartGuiUpdate();
	void EndGuiUpdate();
	void DisplaySelectionParameters();
	void DisplayAllScripts();
	void DisplayFreezeBtn();
	void DisplayMenuBar();

	std::unique_ptr<Ogre::Root> m_pRoot;
	std::unique_ptr<Ogre::Window> m_pRenderWindow;
	std::unique_ptr<Ogre::SceneManager> m_pSceneManager;
	std::unique_ptr<Ogre::Camera> m_pCamera;
	std::unique_ptr<Ogre::CompositorWorkspace> m_pWorkspace;
	std::unique_ptr<Ogre::D3D11Plugin> m_pD3D11Plugin;
	std::unique_ptr<Ogre::GL3PlusPlugin> m_pGL3PlusPlugin;

	ResourceManager* m_pResourceManager;
	std::unique_ptr<SceneObjectProducer> m_pSceneObjectProducer;
	Ogre::SceneNode* m_pCurSelection;
	bool m_bSelectionChanged;

	std::unique_ptr<RenderThread> m_pRT;
	SDL_Window* m_SDL_Window;
	SDL_GLContext m_GL_Context;

	bool m_bIsInitialized;
	bool m_bQuit;
	bool m_bIsFreeze;

	bool m_bIsMousePressed;
	Ogre::Vector2 m_vecMousePos;
};
