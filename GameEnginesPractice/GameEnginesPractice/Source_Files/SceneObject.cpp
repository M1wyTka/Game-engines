#include "SceneObject.h"

SceneObject::SceneObject(Ogre::SceneManager* sceneManager)
{
	m_pSceneManager = sceneManager;

	SO_LoadMeshModel("ogrehead.mesh");

	SO_LoadManagerItems();
}

SceneObject::~SceneObject()
{
}

void SceneObject::SO_LoadMeshModel()
{
	Ogre::v1::MeshPtr v1Mesh;

	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		"ogrehead.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
		IMPORT_NAME("ogrehead.mesh"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();
}

void SceneObject::SO_LoadManagerItems()
{
	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	m_pItem = m_pSceneManager->createItem(IMPORT_NAME("ogrehead.mesh"),
		Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	m_pSceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	m_pSceneNode->attachObject(m_pItem);
	m_pSceneNode->scale(0.1f, 0.1f, 0.1f);
}

void SceneObject::SO_SetPosition(Ogre::Vector3 newPos)
{
	m_pSceneNode->setPosition(newPos);
}