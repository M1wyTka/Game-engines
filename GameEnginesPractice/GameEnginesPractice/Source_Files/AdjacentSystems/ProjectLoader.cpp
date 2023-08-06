#include "AdjacentSystems/ProjectLoader.h"
#include "SceneObject.h"
#include "OgreLogManager.h"
#include "Ogre.h"


ProjectLoader::ProjectLoader(RenderEngine* renderEngine, EntityManager* entMgr) :
	m_pRenderEngine(renderEngine),
	m_pEntityManager(entMgr)
{
	
};

ProjectLoader::~ProjectLoader() 
{
	
};

std::vector<Pawn>* ProjectLoader::GetLevelEntities(const std::string& projectFilePath)
{
	nlohmann::json jf = m_pFileSystem->GetProjectJson();
	std::vector<Pawn>* pwns = ReadLevelField(jf);

	return pwns;
}

std::vector<Pawn>* ProjectLoader::ReadLevelField(const nlohmann::json& projectJson)
{
	std::vector<Pawn>* pwns = new std::vector<Pawn>();
	for (const auto& item : projectJson.items())
	{
		if (item.key() == "level")
		{
			for (const auto& val : item.value().items())
			{
				std::string str = val.key();
				if (val.key() == "entities")
				{
					for (int i = 0; i < val.value().size(); i++)
					{
						
						Ogre::LogManager::getSingleton().logMessage(val.value()[i]["name"].get<std::string>() + " : " + val.value()[i]["script"].get<std::string>());
						pwns->emplace_back(val.value()[i]["name"].get<std::string>(), val.value()[i]["script"].get<std::string>());

						Ogre::String name = val.value()[i]["name"].get<std::string>();
						Ogre::String script = val.value()[i]["script"].get<std::string>();
						Ogre::String meshName = val.value()[i]["components"]["mesh"].get<std::string>();
						auto pos = val.value()[i]["components"]["position"];

						m_pEntityManager->CreateEntity(script,
						{ name, meshName, true },
	 	 				{ Ogre::Quaternion(Ogre::Quaternion::IDENTITY) , Ogre::Vector3(pos[0], pos[1], pos[2]) , Ogre::Vector3(20.f, 1.f, 1.f) });
						////HERE
					}
				}

				if (val.key() == "lights") 
				{
					for (int i = 0; i < val.value().size(); i++)
					{
						std::string s = val.value()[i]["name"].get<std::string>();
						int t = val.value()[i]["lightType"].get<int>();
						Ogre::LogManager::getSingleton().logMessage(s + " : " + std::to_string(t));
						//pwns->emplace_back(val.value()[i]["name"].get<std::string>(), val.value()[i]["lightType"].get<int>());
						////HERE
					}
				}

				if (val.key() == "cameras") 
				{
					
				}
			}
		}
	}
	return pwns;
}

void ProjectLoader::ReadLevelLights(const nlohmann::json& projectJson)
{

}

void ProjectLoader::ReadLevelTerrain(const nlohmann::json& projectJson)
{

}

void ProjectLoader::SaveProject(std::string outFilePath, std::vector<LoadedObject*>* objectsToSave)
{
	nlohmann::json arr = nlohmann::json::array();
	/*"name": "lol",
		"script" : "pawn.lua",
		"components" : {
		"mesh": "ogrehead.mesh",
			"position" : [
				1.0,
					0.0,
					0.0
			] ,
			"velocity" : [
				0.0,
					0.0,
					0.0
			]
	}*/
	std::vector<Pawn> pwns;
	for (const auto& item : *objectsToSave) 
	{
		nlohmann::json position = nlohmann::json::array({ item->object->GetPosition().x, item->object->GetPosition().y, item->object->GetPosition().z });
		nlohmann::json ent = {
			{"name", item->initialName },
			{"script", "pawn.lua"},
			{"components", {
				{ "mesh", item->initialMeshName },
				{ "position", position }
				}
			}
		};
		arr.push_back({ ent });
	}
	nlohmann::json LevelJSON = { { "level", { {"entities", arr }} } };
	m_pFileSystem->SaveProjectJson(LevelJSON);
}