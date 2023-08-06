#pragma once
#include <string>
#include <filesystem>
#include <shared_mutex>
#include <nlohmann/json.hpp>

typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock>  WriteLock;
typedef std::shared_lock<Lock>  ReadLock;

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	const std::string GetMediaRootPath() {	return m_pMediaRoot.string(); };
	const std::string GetScriptsRootPath() { return m_pScriptsRoot.string(); };
	const std::string GetCodeRootPath() { return m_pCodeDir.string(); };
	const std::string GetProjectLoadFilePath () { return m_pLoadProjectFilePath.string(); };
	const std::string GetProjectSaveFilePath () { return m_pSaveProjectFilePath.string(); };

	nlohmann::json GetProjectJson();
	void SaveProjectJson(nlohmann::json& projectJson);

	const std::string JoinPaths(const std::string& strA, const std::string& strB);

private:

	inline static const std::filesystem::path m_pCurDir = std::filesystem::current_path();
	inline static const std::filesystem::path m_pRootProjDir = m_pCurDir.parent_path().parent_path().parent_path().parent_path();
	inline static const std::filesystem::path m_pMediaRoot = m_pRootProjDir / std::filesystem::path("Media");
	inline static const std::filesystem::path m_pCodeDir = m_pRootProjDir / std::filesystem::path("GameEnginesPractice");
	inline static const std::filesystem::path m_pScriptsRoot = m_pMediaRoot / std::filesystem::path("Scripts");
	inline static const std::filesystem::path m_pLoadProjectFilePath = m_pMediaRoot / std::filesystem::path("ProjectLoad.json");
	inline static const std::filesystem::path m_pSaveProjectFilePath = m_pMediaRoot / std::filesystem::path("ProjectSave.json");
};