#include "AdjacentSystems/FileSystem.h"
#include <fstream>

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

nlohmann::json FileSystem::GetProjectJson()
{
	std::ifstream ifs(m_pLoadProjectFilePath.string());

	nlohmann::json projJson;
	ifs >> projJson;

	return projJson;
}

void FileSystem::SaveProjectJson(nlohmann::json& projectJson)
{
	std::ofstream file(m_pSaveProjectFilePath);
	file << projectJson;
}

const std::string FileSystem::JoinPaths(const std::string& strA, const std::string& strB) 
{
	std::filesystem::path A = std::filesystem::path(strA).make_preferred();
	std::filesystem::path B = std::filesystem::path(strB).make_preferred();
	return  (A / B).string();
}

	