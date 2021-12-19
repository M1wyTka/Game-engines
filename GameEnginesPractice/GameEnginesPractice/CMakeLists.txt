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
add_executable (GameEnginesPractice WIN32 "Source_Files/GameEnginesPractice.cpp" 
                                          "Source_Files/Game.cpp"
                                          "Source_Files/AdjacentSystems/GameTimer.cpp" 
                                          "Source_Files/Render/RenderEngine.cpp" 
                                          "Source_Files/Render/RenderThread.cpp"
                                          "Source_Files/SceneObject.cpp" 
                                          "Source_Files/CelestialBody.cpp" 
                                          "Source_Files/SceneObjectProducer.cpp" 
                                          "Source_Files/ResourceManager.cpp" 
                                          "Source_Files/AdjacentSystems/FileSystem.cpp" 
                                          "Source_Files/AdjacentSystems/InputHandler.cpp"
                                          "Source_Files/Render/MTQueue.cpp" 
                                          "Source_Files/GameObject.cpp" 
                                          "Source_Files/Bullet.cpp" 
                                           
                                          "Source_Files/ECS/PhysECS.cpp" 
                                          "Source_Files/ECS/MeshECS.cpp" 
                                          "Source_Files/ECS/ControlECS.cpp" 
                                          "Source_Files/ECS/GeneralECS.cpp" 
                                          "Source_Files/ECS/KinematicsECS.cpp" 
                                          "Source_Files/AdjacentSystems/ProjectLoader.cpp")
                                          #"Source_Files/AdjacentSystems/WindowManager.cpp"
                                          #"Source_Files/AdjacentSystems/LWindow.cpp"
                                          #"Source_Files/AdjacentSystems/EditorLWindow.cpp"
                                          #"Source_Files/AdjacentSystems/RenderLWindow.cpp")
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

target_link_libraries(GameEnginesPractice flecs::flecs_static)

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
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG v3.10.4
)

target_link_libraries(GameEnginesPractice nlohmann_json::nlohmann_json)

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

#get_cmake_property(_variableNames VARIABLES)
#list(SORT _variableNames)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName} = ${${_variableName}}")
#endforeach()
# TODO: Add tests and install targets if needed.
