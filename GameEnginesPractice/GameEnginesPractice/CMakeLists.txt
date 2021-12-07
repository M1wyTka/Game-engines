# CMakeList.txt : CMake project for OgreEnginePractice, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18)



#1
#find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
#add_executable (GameEnginesPractice WIN32 "Source_Files/GameEnginesPractice.cpp" "Header_Files/GameEnginesPractice.h" "Header_Files/Framework.h" "Header_Files/Game.h" "Header_Files/AdjacentSystems/GameTimer.h" "Header_Files/Render/RenderEngine.h" "Header_Files/Render/RenderThread.h" "Header_Files/Resource.h" "Header_Files/SceneObject.h" "Source_Files/Game.cpp" "Source_Files/AdjacentSystems/GameTimer.cpp" "Source_Files/Render/RenderEngine.cpp" "Source_Files/Render/RenderThread.cpp" "Source_Files/SceneObject.cpp" "Header_Files/Universe.h" "Header_Files/CelestialBody.h" "Source_Files/CelestialBody.cpp" "Header_Files/SceneObjectProducer.h" "Source_Files/SceneObjectProducer.cpp" "Header_Files/ResourceManager.h" "Source_Files/ResourceManager.cpp" "Header_Files/AdjacentSystems/FileSystem.h" "Source_Files/AdjacentSystems/FileSystem.cpp" "Header_Files/AdjacentSystems/InputHandler.h" "Source_Files/AdjacentSystems/InputHandler.cpp" "Header_Files/AdjacentSystems/Input.h" "Header_Files/Render/MTQueue.h" "Source_Files/Render/MTQueue.cpp" "Header_Files/IMoveable.h"  "Header_Files/GameObject.h" "Source_Files/GameObject.cpp" "Header_Files/Bullet.h" "Source_Files/Bullet.cpp" "Header_Files/ECS/flecs.h" "Source_Files/ECS/flecs.c" "Header_Files/ECS/MeshECS.h" "Header_Files/ECS/ControlECS.h" "Header_Files/ECS/PhysECS.h" "Source_Files/ECS/PhysECS.cpp" "Source_Files/ECS/MeshECS.cpp" "Source_Files/ECS/ControlECS.cpp" "Header_Files/ECS/GeneralECS.h" "Source_Files/ECS/GeneralECS.cpp" "Header_Files/AdjacentSystems/GEFile.h"   "Header_Files/ECS/KinematicsECS.h" "Source_Files/ECS/KinematicsECS.cpp" "Header_Files/GUI/GUIComponentEditor.h" "Source_Files/GUI/GUIComponentEditor.cpp" "Header_Files/GUI/GUISceneHierarchy.h" "Source_Files/GUI/GUISceneHierarchy.cpp" "Header_Files/GUI/GUIToolMenu.h" "Source_Files/GUI/GUIToolMenu.cpp" "Header_Files/GUI/GUIAssetExplorer.h" "Source_Files/GUI/GUIAssetExplorer.cpp")
#target_include_directories(GameEnginesPractice PRIVATE "Header_Files")
#target_link_libraries(GameEnginesPractice OgreBites)

#2
find_package(OGRE)
add_executable (GameEnginesPractice WIN32 "Source_Files/GameEnginesPractice.cpp" "Header_Files/GameEnginesPractice.h" "Header_Files/Framework.h" "Header_Files/Game.h" "Header_Files/AdjacentSystems/GameTimer.h" "Header_Files/Render/RenderEngine.h" "Header_Files/Render/RenderThread.h" "Header_Files/Resource.h" "Header_Files/SceneObject.h" "Source_Files/Game.cpp" "Source_Files/AdjacentSystems/GameTimer.cpp" "Source_Files/Render/RenderEngine.cpp" "Source_Files/Render/RenderThread.cpp" "Source_Files/SceneObject.cpp" "Header_Files/Universe.h" "Header_Files/CelestialBody.h" "Source_Files/CelestialBody.cpp" "Header_Files/SceneObjectProducer.h" "Source_Files/SceneObjectProducer.cpp" "Header_Files/ResourceManager.h" "Source_Files/ResourceManager.cpp" "Header_Files/AdjacentSystems/FileSystem.h" "Source_Files/AdjacentSystems/FileSystem.cpp" "Header_Files/AdjacentSystems/InputHandler.h" "Source_Files/AdjacentSystems/InputHandler.cpp" "Header_Files/AdjacentSystems/Input.h" "Header_Files/Render/MTQueue.h" "Source_Files/Render/MTQueue.cpp" "Header_Files/IMoveable.h"  "Header_Files/GameObject.h" "Source_Files/GameObject.cpp" "Header_Files/Bullet.h" "Source_Files/Bullet.cpp" "Header_Files/ECS/flecs.h" "Source_Files/ECS/flecs.c" "Header_Files/ECS/MeshECS.h" "Header_Files/ECS/ControlECS.h" "Header_Files/ECS/PhysECS.h" "Source_Files/ECS/PhysECS.cpp" "Source_Files/ECS/MeshECS.cpp" "Source_Files/ECS/ControlECS.cpp" "Header_Files/ECS/GeneralECS.h" "Source_Files/ECS/GeneralECS.cpp" "Header_Files/AdjacentSystems/GEFile.h"   "Header_Files/ECS/KinematicsECS.h" "Source_Files/ECS/KinematicsECS.cpp" "Header_Files/GUI/GUIComponentEditor.h" "Source_Files/GUI/GUIComponentEditor.cpp" "Header_Files/GUI/GUISceneHierarchy.h" "Source_Files/GUI/GUISceneHierarchy.cpp" "Header_Files/GUI/GUIToolMenu.h" "Source_Files/GUI/GUIToolMenu.cpp" "Header_Files/GUI/GUIAssetExplorer.h" "Source_Files/GUI/GUIAssetExplorer.cpp")
target_include_directories(GameEnginesPractice PRIVATE "Header_Files")
#target_link_libraries(GameEnginesPractice OgreBites)

CPMAddPackage(
    NAME gl3w
    GITHUB_REPOSITORY skaslev/gl3w
    GIT_TAG master
)

target_link_libraries(GameEnginesPractice gl3w)
find_package(OpenGL REQUIRED)


#target_include_directories(GameEnginesPractice PRIVATE ${OGRE_INCLUDE_DIRS} ${OGRE_RenderSystem_Direct3D11_INCLUDE_DIRS})
#target_link_libraries(GameEnginesPractice ${OGRE_LIBRARIES} ${OGRE_RenderSystem_Direct3D11_LIBRARIES} ${OGRE_HlmsUnlit_LIBRARIES} ${OGRE_HlmsPbs_LIBRARIES} "${SDKS_DIR}/Lua/lua54.lib")

#2
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/OgreMain/include")
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/Components/Hlms/Common/include")
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/Components/Hlms/Pbs/include")
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/Components/Hlms/Unlit/include")
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/build/include")
target_include_directories(GameEnginesPractice PRIVATE "${OGRE_PLACE}/build/sdk/include")
target_link_libraries(GameEnginesPractice ${OGRE_LIBRARIES} ${OGRE_RenderSystem_Direct3D11_LIBRARIES} ${OGRE_HlmsUnlit_LIBRARIES} ${OGRE_HlmsPbs_LIBRARIES} ${OGRE_Plugin_ParticleFX_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_RenderSystem_GL3Plus_LIBRARIES} ${OPENGL_LIBRARIES} $)



target_include_directories(GameEnginesPractice PRIVATE "${SDKS_DIR}/Lua/include")

CPMAddPackage(
    NAME LuaBridge
    GITHUB_REPOSITORY vinniefalco/LuaBridge
    GIT_TAG 2.6
)

target_link_libraries(GameEnginesPractice LuaBridge)

CPMAddPackage(
    NAME function2
    GITHUB_REPOSITORY Naios/function2
    GIT_TAG 4.2.0
)

target_link_libraries(GameEnginesPractice function2)

CPMAddPackage(
    NAME flecs
    GITHUB_REPOSITORY SanderMertens/flecs
    GIT_TAG v2.4.8
)

CPMAddPackage(
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.0.18 
)

target_link_libraries(GameEnginesPractice SDL2)

CPMAddPackage(
    NAME ImGui
    GITHUB_REPOSITORY ocornut/imgui
    VERSION 1.84.2
    DOWNLOAD_ONLY YES
)

if (ImGui_ADDED)
    add_library(DearImGui
        ${ImGui_SOURCE_DIR}/imgui.cpp ${ImGui_SOURCE_DIR}/imgui_draw.cpp
        ${ImGui_SOURCE_DIR}/imgui_tables.cpp ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
        ${ImGui_SOURCE_DIR}/imgui_demo.cpp # TODO: remove
        ${ImGui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp ${ImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

    target_include_directories(DearImGui PUBLIC ${ImGui_SOURCE_DIR})
    target_link_libraries(DearImGui SDL2)
endif()

target_link_libraries(GameEnginesPractice DearImGui)

CPMAddPackage(
    NAME gl3w
    GITHUB_REPOSITORY skaslev/gl3w
    GIT_TAG master
)

target_link_libraries(GameEnginesPractice gl3w)

#CPMAddPackage(
#    NAME zlib
#    GITHUB_REPOSITORY madler/zlib
#    GIT_TAG v1.2.11
#)
#
#CPMAddPackage(
#    NAME harfbuzz
#    GITHUB_REPOSITORY harfbuzz/harfbuzz
#    GIT_TAG 3.1.2
#)
#
#CPMAddPackage(
#    NAME freetype
#    GITHUB_REPOSITORY freetype/freetype
#    GIT_TAG VER-2-11-1
#)
#
#target_link_libraries(freetype PRIVATE harfbuzz)
#
#CPMAddPackage(
#    NAME pugixml
#    GITHUB_REPOSITORY zeux/pugixml
#    GIT_TAG v1.11.4
#)
#
#CPMAddPackage(
#    NAME assimp
#    GITHUB_REPOSITORY assimp/assimp
#    GIT_TAG v5.1.2
#)

#CPMAddPackage(
#    NAME ogre2
#    GITHUB_REPOSITORY OGRECave/ogre-next
#    GIT_TAG v2.2.5
#)
#target_link_libraries(ogre PRIVATE zlib freetype pugixml assimp)

#target_link_libraries(GameEnginesPractice ogre2)

#2
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(WIN_DLLS OgreMain_d.dll RenderSystem_Direct3D11_d.dll amd_ags_x64.dll OgreHlmsUnlit_d.dll OgreHlmsPbs_d.dll RenderSystem_GL3Plus_d.dll SDL2d.dll)
else ()
    set(WIN_DLLS OgreMain.dll RenderSystem_Direct3D11.dll amd_ags_x64.dll OgreHlmsUnlit.dll OgreHlmsPbs.dll OgreHlmsPbs.dll RenderSystem_GL3Plus.dll SDL2.dll)
endif ()

foreach(DLL ${WIN_DLLS})
    add_custom_command(TARGET GameEnginesPractice POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -E copy "${OGRE_PLACE}/build/sdk/bin/${CMAKE_BUILD_TYPE}/${DLL}" ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)
foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName} = ${${_variableName}}")
endforeach()
# TODO: Add tests and install targets if needed.
