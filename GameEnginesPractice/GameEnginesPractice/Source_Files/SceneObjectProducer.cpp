#include "SceneObjectProducer.h"

SceneObjectProducer::SceneObjectProducer(Ogre::SceneManager* sceneManager) : m_pSceneManager(sceneManager)
{

}

SceneObjectProducer::~SceneObjectProducer() 
{
	
}


SceneObject* SceneObjectProducer::Produce(Ogre::String actorName, Ogre::String meshName)
{
	if (meshName.empty())
		return Produce(actorName);

	Ogre::String name = actorName;
	
	int meshInstanceNum = 0;

	if (m_mpUsedNames.find(actorName) != m_mpUsedNames.end())
	{
		m_mpUsedNames.emplace(actorName, ++m_mpUsedNames[actorName]);
		name = actorName + std::to_string(m_mpUsedNames[actorName]);
	}
	else 
	{
		m_mpUsedNames.insert({ actorName, 0 });
	}

	if (m_mpUsedMeshes.find(meshName) != m_mpUsedMeshes.end())
	{
		m_mpUsedMeshes.emplace(meshName, ++m_mpUsedMeshes[meshName]);
		meshInstanceNum = m_mpUsedMeshes[meshName];
	}
	else
	{
		m_mpUsedMeshes.insert({ meshName, 0 });
	}

	Ogre::MeshPtr meshToUse = LoadMeshModel(meshName, meshInstanceNum);
	Ogre::Item* item = LoadItem(meshToUse);
	Ogre::SceneNode* node = LoadNode(item);

	//m_pLogicVisualMapping.insert({ node, ent });

	return new SceneObject(name, node);
}

SceneObject* SceneObjectProducer::Produce(Ogre::String actorName)
{
	Ogre::String name = actorName;
	if (m_mpUsedNames.find(actorName) != m_mpUsedNames.end())
	{
		m_mpUsedNames.emplace(actorName, ++m_mpUsedNames[actorName]);
		name = actorName + std::to_string(m_mpUsedNames[actorName]);
	}
	else
	{
		m_mpUsedNames.insert({ actorName, 0 });
	}
	Ogre::SceneNode* sceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);

	//m_pLogicVisualMapping.insert({ sceneNode, ent });

	return new SceneObject(name, sceneNode);
}

Ogre::MeshPtr SceneObjectProducer::LoadMeshModel(Ogre::String meshName, int instanceNum)
{
	Ogre::v1::MeshPtr v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		meshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	Ogre::String v2Name = meshName + std::to_string(instanceNum);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	Ogre::MeshPtr meshToUse = Ogre::MeshManager::getSingleton().createManual(
		v2Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	meshToUse->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();

	return meshToUse;
}

Ogre::Item* SceneObjectProducer::LoadItem(Ogre::MeshPtr mesh)
{
	Ogre::Item* item = m_pSceneManager->createItem(mesh->getName(),
		Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	return item;
}

Ogre::SceneNode* SceneObjectProducer::LoadNode(Ogre::Item* item)
{
	Ogre::SceneNode* sceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	sceneNode->attachObject(item);
	return sceneNode;
}