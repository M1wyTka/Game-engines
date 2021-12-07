#pragma once
#include "Ogre.h"

#include "GeneralDefines.h"

#include "OgreHlms.h"
#include "OgreHlmsUnlit.h"
#include "OgreHlmsPbs.h"
#include "AdjacentSystems/FileSystem.h"

class ResourceManager
{
public:
	ResourceManager(FileSystem* fileSys);
	~ResourceManager();

	void LoadDefaultResources();

private:

	inline static const std::string m_strResourceConfigFile = "resources.cfg";
	
	FileSystem* m_pFileSystem;

	void LoadConfigSections(Ogre::ConfigFile& cf);
	void LoadHlms(Ogre::ConfigFile& cf);
	Ogre::String GetRootHlmsFolder(Ogre::ConfigFile& cf);
	void RegisterHlms(Ogre::String rootHlmsFolder);
	void GetHlmArchiveVec(Ogre::ArchiveVec& archiveVec, Ogre::String rootHlmsFolder, Ogre::StringVector libraryFoldersPaths);
	//void SetHlmsTextureBufferSize(Ogre::HlmsPbs* hlmsPbs, Ogre::HlmsUnlit* hlmsUnlit);

	std::string m_strResourceRoot;
};

