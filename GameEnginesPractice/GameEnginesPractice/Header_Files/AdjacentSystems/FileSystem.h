#pragma once
#include <string>
#include <filesystem>
#include <shared_mutex>

typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock>  WriteLock;
typedef std::shared_lock<Lock>  ReadLock;

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	const std::string GetMediaRoot();
	const std::string GetScriptsRoot();
	const std::string GetCodeRoot();

	const std::string JoinPaths(const std::string& strA, const std::string& strB);

private:

	inline static const std::filesystem::path m_pCurDir = std::filesystem::current_path();
	inline static const std::filesystem::path m_pRootProjDir = m_pCurDir.parent_path().parent_path().parent_path().parent_path();
	inline static const std::filesystem::path m_pMediaRoot = m_pRootProjDir / std::filesystem::path("Media");
	inline static const std::filesystem::path m_pCodeDir = m_pRootProjDir / std::filesystem::path("GameEnginesPractice");
	inline static const std::filesystem::path m_pScriptsRoot = m_pCodeDir / std::filesystem::path("Scripts");
};