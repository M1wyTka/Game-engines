#include "Render/RenderEngine.h"

RenderEngine::RenderEngine(ResourceManager* pResourceManager) :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_pRT(nullptr),
	m_pSceneObjectProducer(nullptr),
	m_bIsInitialized(false),
	m_bQuit(false),
	m_pResourceManager(pResourceManager)
{
	m_pRT = std::unique_ptr<RenderThread>(new RenderThread(this));

	m_pRT->RC_Init();
	m_pRT->RC_SetupDefaultCamera();
	m_pRT->RC_SetupDefaultCompositor();
	m_pRT->RC_LoadDefaultResources();

	m_pRT->RC_LoadOgreHead();
	m_pRT->RC_SetupDefaultLight();

	m_pRT->Start();
}

RenderEngine::~RenderEngine()
{
	//SAFE_OGRE_DELETE(m_pRoot);
}

bool RenderEngine::SetOgreConfig()
{
#ifdef _DEBUG
	constexpr bool bAlwaysShowConfigWindow = true;
	if (bAlwaysShowConfigWindow || !m_pRoot->restoreConfig())
#else
	if (!m_pRoot->restoreConfig())
#endif
	{
		if (!m_pRoot->showConfigDialog())
		{
			return false;
		}
	}

	return true;
}

void RenderEngine::Update()
{
	Ogre::WindowEventUtilities::messagePump();

	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
}

void RenderEngine::RT_Init()
{
	m_pRoot = std::unique_ptr<Ogre::Root>(new Ogre::Root());
	m_pD3D11Plugin = std::unique_ptr<Ogre::D3D11Plugin>(new Ogre::D3D11Plugin());

	m_pRoot->installPlugin(m_pD3D11Plugin.get());

	if (!SetOgreConfig())
	{
		m_bQuit = true;
		return;
	}

	m_pRoot->initialise(false);

	// Creating window
	Ogre::uint32 width = 1280;
	Ogre::uint32 height = 720;
	Ogre::String sTitleName = "Game Engine";

	m_pRenderWindow = std::unique_ptr<Ogre::Window>(Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false));

	// Scene manager
	m_pSceneManager = std::unique_ptr<Ogre::SceneManager>(m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 2));
	m_pSceneObjectProducer = std::unique_ptr<SceneObjectProducer>(new SceneObjectProducer(m_pSceneManager.get()));
}

void RenderEngine::RT_SetupDefaultCamera()
{
	m_pCamera = std::unique_ptr<Ogre::Camera>(m_pSceneManager->createCamera("Main Camera"));

	m_pCamera->setPosition(Ogre::Vector3(150, 150, 150));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(0.2f);
	m_pCamera->setFarClipDistance(1000.0f);
	m_pCamera->setAutoAspectRatio(true);
}

void RenderEngine::RT_SetupDefaultCompositor()
{
	Ogre::CompositorManager2* compositorManager = m_pRoot->getCompositorManager2();

	const Ogre::String workspaceName("WorkSpace");

	if (!compositorManager->hasWorkspaceDefinition(workspaceName))
	{
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::Blue);
	}
	
	m_pWorkspace = std::unique_ptr<Ogre::CompositorWorkspace>
					(compositorManager->addWorkspace(m_pSceneManager.get(), m_pRenderWindow->getTexture(), m_pCamera.get(), workspaceName, true));
}

void RenderEngine::RT_LoadDefaultResources()
{
	m_pResourceManager->LoadDefaultResources();
}

void RenderEngine::RT_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos)
{
	actor->SetPosition(pos);
}

void RenderEngine::RT_UpdateActorScale(SceneObject* actor, Ogre::Vector3 scale)
{
	actor->SetScale(scale.x, scale.y, scale.z);
}

void RenderEngine::RT_LoadOgreHead()
{
	m_bIsInitialized = true;
}

void RenderEngine::RT_SetupDefaultLight()
{
	// Lightning
	Ogre::Light* light = m_pSceneManager->createLight();
	Ogre::SceneNode* lightNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
}

void RenderEngine::RT_OscillateCamera(float time)
{
	//m_pCamera->moveRelative(Ogre::Vector3(time*10, 0, 0));
	//m_pCamera->setPosition(Ogre::Vector3(150 * time * 10, 150, 150));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void RenderEngine::RT_MoveCamera(Ogre::Vector3 pos) 
{
	m_pCamera->move(pos);
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

SceneObject* RenderEngine::RT_CreateSceneObject(Ogre::String actorName, Ogre::String meshName) 
{
	return m_pSceneObjectProducer->Produce(actorName, meshName);
}
