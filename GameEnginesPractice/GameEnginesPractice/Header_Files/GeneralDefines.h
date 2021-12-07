#pragma once
#include "OgreMemoryAllocatorConfig.h"

typedef unsigned long threadID;

#define SAFE_OGRE_DELETE(x) do { OGRE_DELETE(x); x = nullptr; } while(false)

#define MEDIA_PATH "D:\\MIPT\\Game-engines\\GameEnginesPractice\\Media"
#define THREAD_NUM 2