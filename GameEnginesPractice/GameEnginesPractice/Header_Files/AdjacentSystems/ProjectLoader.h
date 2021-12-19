#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "OgreLogManager.h"
#include "SceneObject.h"

struct Pawn
{
	Pawn(std::string name, std::string script) :Name(name), Script(script) {}
	std::string Name;
	std::string Script;
};

class ProjectLoader 
{
public:
	ProjectLoader() {};
	~ProjectLoader() {};
	
	std::vector<Pawn>* GetLevelEntities(const std::string& projectFilePath);
	void SaveProject(std::string outFilePath, std::vector<SceneObject*>* objectsToSave);

private:
	std::vector<Pawn>* ReadLevelField(const nlohmann::json& projectJson);

};