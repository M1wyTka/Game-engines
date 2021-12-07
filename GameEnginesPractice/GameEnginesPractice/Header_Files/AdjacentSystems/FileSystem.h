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

	const std::string JoinPaths(const std::string& strA, const std::string& strB);

private:

	inline static const std::filesystem::path m_pCurDir = std::filesystem::current_path();
	inline static const std::filesystem::path m_pMediaRoot = std::filesystem::path("D:\\MIPT\\Game-engines\\GameEnginesPractice\\Media\\").make_preferred();
	inline static const std::filesystem::path m_pScriptsRoot = std::filesystem::path("Scripts");
};