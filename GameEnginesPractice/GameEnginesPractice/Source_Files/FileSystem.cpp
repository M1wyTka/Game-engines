#include "FileSystem.h"

FileSystem::FileSystem()
{
	constexpr char cNativeSlash = static_cast<char>(e_cNativeSlash);
	const std::string cNativeDoubleDot = "..";
	// ../../../../Media/
	m_strMediaRoot = "..";
	m_strMediaRoot.push_back(cNativeSlash);
	m_strMediaRoot.append(cNativeDoubleDot);
	m_strMediaRoot.push_back(cNativeSlash);
	m_strMediaRoot.append(cNativeDoubleDot);
	m_strMediaRoot.push_back(cNativeSlash);
	m_strMediaRoot.append(cNativeDoubleDot);
	m_strMediaRoot.push_back(cNativeSlash);
	m_strMediaRoot.append("Media");
	m_strMediaRoot.push_back(cNativeSlash);
}

FileSystem::~FileSystem()
{
}

const std::string& FileSystem::GetMediaRoot()
{
	return m_strMediaRoot;
}