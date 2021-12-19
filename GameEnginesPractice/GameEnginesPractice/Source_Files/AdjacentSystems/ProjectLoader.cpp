#include "AdjacentSystems/ProjectLoader.h"

std::vector<Pawn>* ProjectLoader::GetLevelEntities(const std::string& projectFilePath)
{
	std::ifstream i(projectFilePath);
	nlohmann::json jf;
	i >> jf;

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
				if (val.key() == "entities")
					for (int i = 0; i < val.value().size(); i++)
					{
						pwns->emplace_back(val.value()[i]["name"].get<std::string>(), val.value()[i]["script"].get<std::string>());
						////HERE
					}
			}
		}
	}
	return pwns;
}

void ProjectLoader::SaveProject(std::string outFilePath, std::vector<SceneObject*>* objectsToSave)
{
	nlohmann::json arr = nlohmann::json::array();
	
	std::vector<Pawn> pwns;
	for (const auto& item : *objectsToSave) 
	{
		nlohmann::json ent = {
			{"name", item->GetName() },
			{"script", "lol"},
			{"components", {
				{"mesh", item->GetMeshName()}
				}
			}
		};
		arr.push_back({ ent });
	}
	nlohmann::json LevelJSON = { { "level", { {"entities", arr }} } };

	std::ofstream file("D:/MIPT/Game-engines/GameEnginesPractice/Media/Lol.json");
	file << LevelJSON;

}