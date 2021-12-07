#include "AdjacentSystems/FileSystem.h"

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

const std::string FileSystem::GetMediaRoot()
{
	return m_pMediaRoot.string();
}

const std::string FileSystem::GetScriptsRoot()
{
	return (m_pMediaRoot / m_pScriptsRoot).string();
}

const std::string FileSystem::JoinPaths(const std::string& strA, const std::string& strB) 
{
	std::filesystem::path A = std::filesystem::path(strA).make_preferred();
	std::filesystem::path B = std::filesystem::path(strB).make_preferred();
	return  (A / B).string();
}