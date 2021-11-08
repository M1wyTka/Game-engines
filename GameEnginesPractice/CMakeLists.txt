# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(TO_CMAKE_PATH "$ENV{OGRE_HOME}" OGRE_HOME_DIR)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
SET(SDKS_DIR "${PARENT_DIR}/SDKs")
set(CMAKE_MODULE_PATH "${OGRE_HOME_DIR}/CMake;${CMAKE_MODULE_PATH}")

project ("GameEnginesPractice")

find_package(OGRE REQUIRED)

add_subdirectory ("GameEnginesPractice")
