#include "RenderEngine.h"

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
	m_pRT = new RenderThread(this);

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
	SAFE_OGRE_DELETE(m_pRoot);
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
	m_pRoot = OGRE_NEW Ogre::Root();
	m_pD3D11Plugin = OGRE_NEW Ogre::D3D11Plugin();

	m_pRoot->installPlugin(m_pD3D11Plugin);

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

	m_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false);

	// Scene manager
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 2);
	m_pSceneObjectProducer = new SceneObjectProducer(m_pSceneManager);
}

void RenderEngine::RT_SetupDefaultCamera()
{
	m_pCamera = m_pSceneManager->createCamera("Main Camera");

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

	m_pWorkspace = compositorManager->addWorkspace(m_pSceneManager, m_pRenderWindow->getTexture(), m_pCamera, workspaceName, true);
}

void RenderEngine::RT_LoadDefaultResources()
{
	m_pResourceManager->LoadDefaultResources();
}

void RenderEngine::RT_UpdateActorPosition(SceneObject* actor, Ogre::Vector3 pos)
{
	actor->SO_SetPosition(pos);
}

void RenderEngine::RT_LoadOgreHead()
{
	//OgreHead = m_pSceneObjectProducer->Produce("Ogre", "Cube.mesh"); 
	//OgreHead->SO_SetPosition(Ogre::Vector3(0, 0, 50));
	//OgreHead->SO_SetScale(0.5, 0.5, 10);

	//Cube = m_pSceneObjectProducer->Produce("cube", "Cube.mesh");
	//Cube->SO_SetPosition(Ogre::Vector3(50, 0, 0));
	//Cube->SO_SetScale(10, 0.5, 0.5);

	//Barrel = m_pSceneObjectProducer->Produce("bar", "Cube.mesh");
	//Barrel->SO_SetPosition(Ogre::Vector3(0, 50, 0));
	//Barrel->SO_SetScale(0.5, 10, 0.5);

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
	std::lock_guard<std::mutex> lock(creation);
	return m_pSceneObjectProducer->Produce(actorName, meshName);

	//return new SceneObject(*m_pSceneManager, meshName);
}
