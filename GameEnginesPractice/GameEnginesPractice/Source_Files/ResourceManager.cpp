#include "ResourceManager.h"


ResourceManager::ResourceManager(FileSystem* fileSys) : m_pFileSystem(fileSys)
{
	m_strResourceRoot = m_pFileSystem->GetMediaRoot() + "\\";
}

ResourceManager::~ResourceManager()
{
}


void ResourceManager::LoadDefaultResources()
{
	Ogre::ConfigFile cf;
	cf.load(m_strResourceRoot + m_strResourceConfigFile);

	LoadConfigSections(cf);

	LoadHlms(cf);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
}

void ResourceManager::LoadConfigSections(Ogre::ConfigFile& cf)
{
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();

		if (secName != "Hlms")
		{
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = m_pFileSystem->JoinPaths(m_strResourceRoot, i->second);
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
			}
		}
	}
}

void ResourceManager::LoadHlms(Ogre::ConfigFile& cf)
{
	// Load hlms (high level material system) files
	//Ogre::String rootHlmsFolder = m_pFileSystem->JoinPaths(m_strResourceRoot, GetRootHlmsFolder(cf));
	RegisterHlms(m_strResourceRoot);
}

Ogre::String ResourceManager::GetRootHlmsFolder(Ogre::ConfigFile& cf)
{
	// Load hlms (high level material system) files
	Ogre::String rootHlmsFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");

	if (rootHlmsFolder.empty())
		rootHlmsFolder = "./";
	else if (*(rootHlmsFolder.end() - 1) != '/')
		rootHlmsFolder += "/";
	return rootHlmsFolder;
}

void ResourceManager::RegisterHlms(Ogre::String rootHlmsFolder)
{
	//For retrieval of the paths to the different folders needed
	Ogre::String mainFolderPath;
	Ogre::StringVector libraryFoldersPaths;

	Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

	//Create & Register HlmsUnlit
		//Get the path to all the subdirectories used by HlmsUnlit
	Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
	Ogre::Archive* archiveUnlit = archiveManager.load(rootHlmsFolder + mainFolderPath,
		"FileSystem", true);

	Ogre::ArchiveVec archiveUnlitLibraryFolders;
	GetHlmArchiveVec(archiveUnlitLibraryFolders, rootHlmsFolder, libraryFoldersPaths);

	//Create and register the unlit Hlms
	Ogre::HlmsUnlit* hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
	Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);

	//Create & Register HlmsPbs
	//Do the same for HlmsPbs:
	Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
	Ogre::Archive* archivePbs = archiveManager.load(rootHlmsFolder + mainFolderPath,
		"FileSystem", true);

	//Get the library archive(s)
	Ogre::ArchiveVec archivePbsLibraryFolders;
	GetHlmArchiveVec(archivePbsLibraryFolders, rootHlmsFolder, libraryFoldersPaths);

	//Create and register
	Ogre::HlmsPbs* hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
	Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);

	//SetHlmsTextureBufferSize(hlmsPbs, hlmsUnlit);
}

void ResourceManager::GetHlmArchiveVec(Ogre::ArchiveVec& archiveVec, Ogre::String rootHlmsFolder, Ogre::StringVector libraryFoldersPaths)
{
	//Get the library archive(s)
	Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();
	Ogre::StringVector::const_iterator libraryFolderPathIt = libraryFoldersPaths.begin();;
	Ogre::StringVector::const_iterator libraryFolderPathEn = libraryFoldersPaths.end();;
	while (libraryFolderPathIt != libraryFolderPathEn)
	{
		Ogre::Archive* archiveLibrary =
			archiveManager.load(rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
		archiveVec.push_back(archiveLibrary);
		++libraryFolderPathIt;
	}
}

//void ResourceManager::SetHlmsTextureBufferSize(Ogre::HlmsPbs* hlmsPbs, Ogre::HlmsUnlit* hlmsUnlit)
//{
//	Ogre::RenderSystem* renderSystem = m_pRoot->getRenderSystem();
//	bool supportsNoOverwriteOnTextureBuffers;
//	renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV", &supportsNoOverwriteOnTextureBuffers);
//
//	if (!supportsNoOverwriteOnTextureBuffers)
//	{
//		hlmsPbs->setTextureBufferDefaultSize(512 * 1024);
//		hlmsUnlit->setTextureBufferDefaultSize(512 * 1024);
//	}
//}