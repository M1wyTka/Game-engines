# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CPM_USE_NAMED_CACHE_DIRECTORIES TRUE)
set(FETCHCONTENT_QUIET off)

#file(TO_CMAKE_PATH "$ENV{OGRE_HOME}" OGRE_HOME_DIR)

#1
#file(TO_CMAKE_PATH "$ENV{OGRE_HOME}" OGRE_HOME_DIR)
#set(OGRE_DIR "D:/Programming/Modules/CPP_Modules/OgreOne/out/build/sdk/CMake")
#set(OGRE_HOME_DIR "D:/Programming/Modules/CPP_Modules/OgreOne/out")
#set(CMAKE_MODULE_PATH "${OGRE_HOME_DIR}/CMake;${CMAKE_MODULE_PATH}")

#2
file(TO_CMAKE_PATH "$ENV{OGRE_HOME}" OGRE_HOME_DIR)
set(OGRE_DIR "D:/Programming/Modules/CPP_Modules/OgreTwo/Ogre/ogre-next/build/sdk/CMake")
set(OGRE_PLACE "D:/Programming/Modules/CPP_Modules/OgreTwo/Ogre/ogre-next")
#set(OGRE_HOME_DIR "D:/Programming/Modules/CPP_Modules/OgreTwo/Ogre/ogre-next")
set(CMAKE_MODULE_PATH "${OGRE_HOME_DIR}/CMake;${CMAKE_MODULE_PATH}")

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(SDKS_DIR "${PARENT_DIR}/SDKs")


project ("GameEnginesPractice")
include(CPM.cmake)
add_subdirectory ("GameEnginesPractice")
