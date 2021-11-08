# CMakeList.txt : CMake project for OgreEnginePractice, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


add_executable (GameEnginesPractice WIN32 "Source_Files/GameEnginesPractice.cpp" "Header_Files/GameEnginesPractice.h" "Header_Files/Framework.h" "Header_Files/Game.h" "Header_Files/AdjacentSystems/GameTimer.h" "Header_Files/Render/RenderEngine.h" "Header_Files/Render/RenderThread.h" "Header_Files/Resource.h" "Header_Files/SceneObject.h" "Source_Files/Game.cpp" "Source_Files/AdjacentSystems/GameTimer.cpp" "Source_Files/Render/RenderEngine.cpp" "Source_Files/Render/RenderThread.cpp" "Source_Files/SceneObject.cpp" "Header_Files/Universe.h" "Header_Files/CelestialBody.h" "Source_Files/CelestialBody.cpp" "Header_Files/SceneObjectProducer.h" "Source_Files/SceneObjectProducer.cpp" "Header_Files/ResourceManager.h" "Source_Files/ResourceManager.cpp" "Header_Files/AdjacentSystems/FileSystem.h" "Source_Files/AdjacentSystems/FileSystem.cpp" "Header_Files/AdjacentSystems/InputHandler.h" "Source_Files/AdjacentSystems/InputHandler.cpp" "Header_Files/AdjacentSystems/Input.h" "Header_Files/Render/MTQueue.h" "Source_Files/Render/MTQueue.cpp" "Header_Files/IMoveable.h"  "Header_Files/GameObject.h" "Source_Files/GameObject.cpp" "Header_Files/Bullet.h" "Source_Files/Bullet.cpp" "Header_Files/ECS/flecs.h" "Source_Files/ECS/flecs.c" "Header_Files/ECS/MeshECS.h" "Header_Files/ECS/ControlECS.h" "Header_Files/ECS/PhysECS.h" "Source_Files/ECS/PhysECS.cpp" "Source_Files/ECS/MeshECS.cpp" "Source_Files/ECS/ControlECS.cpp" "Header_Files/ECS/GeneralECS.h" "Source_Files/ECS/GeneralECS.cpp" "Header_Files/AdjacentSystems/GEFile.h"  )
target_link_libraries(GameEnginesPractice ${OGRE_LIBRARIES} ${OGRE_RenderSystem_Direct3D11_LIBRARIES} ${OGRE_HlmsUnlit_LIBRARIES} ${OGRE_HlmsPbs_LIBRARIES} "${SDKS_DIR}/Lua/lua54.lib")
target_include_directories(GameEnginesPractice PRIVATE "${SDKS_DIR}/Lua/include")
target_include_directories(GameEnginesPractice PRIVATE "${SDKS_DIR}/LuaBridge/include")
target_include_directories(GameEnginesPractice PRIVATE ${OGRE_INCLUDE_DIRS} ${OGRE_RenderSystem_Direct3D11_INCLUDE_DIRS})
target_include_directories(GameEnginesPractice PRIVATE "Header_Files")



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(WIN_DLLS OgreMain_d.dll RenderSystem_Direct3D11_d.dll amd_ags_x64.dll OgreHlmsUnlit_d.dll OgreHlmsPbs_d.dll)
else ()
    set(WIN_DLLS OgreMain.dll RenderSystem_Direct3D11.dll amd_ags_x64.dll OgreHlmsUnlit.dll OgreHlmsPbs.dll)
endif ()

foreach(DLL ${WIN_DLLS})
    add_custom_command(TARGET GameEnginesPractice POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -E copy ${OGRE_HOME_DIR}/bin/${CMAKE_BUILD_TYPE}/${DLL} ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach()

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)
foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName} = ${${_variableName}}")
endforeach()
# TODO: Add tests and install targets if needed.
