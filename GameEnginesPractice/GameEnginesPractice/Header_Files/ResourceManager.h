#pragma once
#include "Ogre.h"

#include "GeneralDefines.h"

#include "OgreHlms.h"
#include "Hlms/Unlit/OgreHlmsUnlit.h"
#include "Hlms/Pbs/OgreHlmsPbs.h"

class ResourceManager
{
public:
	ResourceManager(const std::string& strResourceRoot);
	~ResourceManager();

	void LoadDefaultResources();

private:
	void LoadConfigSections(Ogre::ConfigFile& cf);
	void LoadHlms(Ogre::ConfigFile& cf);
	Ogre::String GetRootHlmsFolder(Ogre::ConfigFile& cf);
	void RegisterHlms(Ogre::String rootHlmsFolder);
	void GetHlmArchiveVec(Ogre::ArchiveVec& archiveVec, Ogre::String rootHlmsFolder, Ogre::StringVector libraryFoldersPaths);
	//void SetHlmsTextureBufferSize(Ogre::HlmsPbs* hlmsPbs, Ogre::HlmsUnlit* hlmsUnlit);

	std::string m_strResourceRoot;
};

