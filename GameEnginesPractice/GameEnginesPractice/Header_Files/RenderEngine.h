#pragma once

#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"

#include "GeneralDefines.h"

#include "SceneObject.h"
#include "SceneObjectProducer.h"

#include "ResourceManager.h"

#include "Compositor/OgreCompositorManager2.h"

#include "RenderSystems/Direct3D11/OgreD3D11Plugin.h"

#include "RenderThread.h"
#include <mutex>

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

	RenderThread* GetRT() const { return m_pRT; }
	SceneObject* CreateSceneObject(Ogre::String actorName, Ogre::String meshName);

private:
	bool SetOgreConfig();

	void RT_Init();
	void RT_SetupDefaultCamera();
	void RT_SetupDefaultCompositor();
	void RT_LoadDefaultResources();
	void RT_LoadOgreHead();
	void RT_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos);
	void RT_SetupDefaultLight();
	void RT_OscillateCamera(float time);

	Ogre::Root* m_pRoot;
	Ogre::Window* m_pRenderWindow;
	Ogre::SceneManager* m_pSceneManager;
	Ogre::Camera* m_pCamera;
	Ogre::CompositorWorkspace* m_pWorkspace;
	Ogre::D3D11Plugin* m_pD3D11Plugin;
	SceneObject* OgreHead;
	SceneObject* Cube;
	SceneObject* Barrel;

	ResourceManager* m_pResourceManager;

	SceneObjectProducer* m_pSceneObjectProducer;
	std::mutex creation; // protects m_pSceneObjectProducer

	RenderThread* m_pRT;

	bool m_bQuit;
};
