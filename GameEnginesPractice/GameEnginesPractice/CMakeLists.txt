# CMakeList.txt : CMake project for OgreEnginePractice, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


add_executable (GameEnginesPractice WIN32 "Source_Files/GameEnginesPractice.cpp" "Header_Files/GameEnginesPractice.h" "Header_Files/Framework.h" "Header_Files/Game.h" "Header_Files/GameTimer.h" "Header_Files/RenderEngine.h" "Header_Files/RenderThread.h" "Header_Files/Resource.h" "Header_Files/SceneObject.h" "Source_Files/Game.cpp" "Source_Files/GameTimer.cpp" "Source_Files/RenderEngine.cpp" "Source_Files/RenderThread.cpp" "Source_Files/SceneObject.cpp" "Header_Files/Universe.h" "Header_Files/CelestialBody.h" "Source_Files/CelestialBody.cpp")
target_link_libraries(GameEnginesPractice ${OGRE_LIBRARIES} ${OGRE_RenderSystem_Direct3D11_LIBRARIES} ${OGRE_HlmsUnlit_LIBRARIES} ${OGRE_HlmsPbs_LIBRARIES})
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

# TODO: Add tests and install targets if needed.
