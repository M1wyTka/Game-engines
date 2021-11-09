#pragma once

#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorManager2.h"

#include "GeneralDefines.h"

#include "SceneObject.h"
#include "SceneObjectProducer.h"

#include "ResourceManager.h"

#include "RenderSystems/Direct3D11/OgreD3D11Plugin.h"

#include "RenderThread.h"
#include <memory>

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

	RenderThread* GetRT() const { return m_pRT.get(); }
	SceneObject* RT_CreateSceneObject(Ogre::String actorName, Ogre::String meshName);

	HWND GetWinHandle() { return m_pHWND; }

	bool IsInitialized() { return m_bIsInitialized; }

private:
	bool SetOgreConfig();

	void RT_Init();
	void RT_SetupDefaultCamera();
	void RT_SetupDefaultCompositor();
	void RT_LoadDefaultResources();
	void RT_LoadOgreHead();
	void RT_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos);
	void RT_UpdateActorScale(SceneObject* actor, Ogre::Vector3 scale);
	void RT_SetupDefaultLight();
	void RT_OscillateCamera(float time);
	void RT_MoveCamera(Ogre::Vector3 pos);

	std::unique_ptr<Ogre::Root> m_pRoot;
	std::unique_ptr<Ogre::Window> m_pRenderWindow;
	std::unique_ptr<Ogre::SceneManager> m_pSceneManager;
	std::unique_ptr<Ogre::Camera> m_pCamera;
	std::unique_ptr<Ogre::CompositorWorkspace> m_pWorkspace;
	std::unique_ptr<Ogre::D3D11Plugin> m_pD3D11Plugin;

	ResourceManager* m_pResourceManager;
	std::unique_ptr<SceneObjectProducer> m_pSceneObjectProducer;

	std::unique_ptr<RenderThread> m_pRT;
	HWND m_pHWND;

	bool m_bIsInitialized;
	bool m_bQuit;
};
