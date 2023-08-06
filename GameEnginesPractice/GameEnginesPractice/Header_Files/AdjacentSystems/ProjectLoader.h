#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
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

	void SaveProject(std::string outFilePath, std::vector<LoadedObject*>* objectsToSave);
	void LoadProject();

private:
	std::vector<Pawn>* ReadLevelField(const nlohmann::json& projectJson);
	void ReadLevelEntities(const nlohmann::json& entitiesJson);
	void SaveLevelEntities();

	void ReadLevelLights(const nlohmann::json& lightsJson);
	void SaveLevelLights();

	void ReadLevelTerrain(const nlohmann::json& terrainJson);
	void SaveLevelTerrain();

	void ReadLevelCameras(const nlohmann::json& camerasJson);
	void SaveLevelCameras();

	RenderEngine* m_pRenderEngine;
	EntityManager* m_pEntityManager;
	FileSystem* m_pFileSystem;
};