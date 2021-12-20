#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "ScriptSystem/EntityManager.h"
#include "Render/RenderEngine.h"

struct Pawn
{
	Pawn(std::string name, std::string script) :Name(name), Script(script) {}
	std::string Name;
	std::string Script;
};

class ProjectLoader 
{
public:
	ProjectLoader(RenderEngine* renderEngine, EntityManager* entMgr);
	~ProjectLoader();
	
	std::vector<Pawn>* GetLevelEntities(const std::string& projectFilePath);

	void SaveProject(std::string outFilePath, std::vector<SceneObject*>* objectsToSave);
	void LoadProject();

private:
	std::vector<Pawn>* ReadLevelField(const nlohmann::json& projectJson);
	void ReadLevelLights(const nlohmann::json& projectJson);
	void ReadLevelTerrain(const nlohmann::json& projectJson);

	RenderEngine* m_pRenderEngine;
	EntityManager* m_pEntityManager;
	FileSystem* m_pFileSystem;
};