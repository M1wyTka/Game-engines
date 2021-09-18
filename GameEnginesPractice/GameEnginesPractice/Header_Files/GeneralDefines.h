#pragma once
#include "OgreMemoryAllocatorConfig.h"

typedef unsigned long threadID;

#define SAFE_OGRE_DELETE(x) do { OGRE_DELETE(x); x = nullptr; } while(false)
#define RESOURCE_CONFIG "resources2.cfg"