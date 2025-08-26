cmake_minimum_required(VERSION 3.29)
#set(VCPKG_MANIFEST_MODE OFF)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_PREFIX_PATH "C:/Users/Jingren Bai/vcpkg/installed/x64-mingw-static;${CMAKE_PREFIX_PATH}")
#set(glfw3_DIR ${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-mingw-static/share/glfw3)
#set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")

if(UNIX)
    find_package(OpenGL REQUIRED)  # Linux 需要显式链接
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Extern)


add_executable(LearnOpenGL main.cpp
        Extern/glad.cpp
        utils/log.cpp
        Rendering/Primitives/TriangleRendering.cpp
        Rendering/Primitives/TriangleRendering.h
        utils/ReadShader.cpp
        utils/ReadShader.h
        Rendering/Scene/Camera.cpp
        Rendering/Scene/Camera.hpp
        Rendering/Primitives/RectangleRender.cpp
        Rendering/Primitives/RectangleRender.h
        Rendering/Primitives/ObjectRender.cpp
        Rendering/Primitives/ObjectRender.h
        Rendering/Pipeline/MainRenderThread.cpp
        Rendering/Pipeline/MainRenderThread.h
        Shader/Shader.cpp
        Shader/Shader.h
        Rendering/Core/Texture.cpp
        Rendering/Core/Texture.h
        Rendering/Core/Vertex.h
        Extern/stb_image.h
        utils/getProgramPath.h
        Extern/stb_image.cpp
        utils/getProgramPath.cpp
        Rendering/Core/ResourceManager.cpp
        Rendering/Core/ResourceManager.h
        Rendering/Asserts/ModelLoader.cpp
        Rendering/Asserts/ModelLoader.h
        Rendering/Primitives/RenderBuilder.cpp
        Rendering/Primitives/RenderBuilder.h
)


target_link_libraries(LearnOpenGL PRIVATE
        glfw
        glm::glm
#        stb::stb_image
        $<$<BOOL:${UNIX}>:OpenGL::GL>  # 仅 Linux 添加
)

# 3. 智能处理 GTest 兼容性
if(TARGET GTest::Main)
    set(GTEST_MAIN_TARGET GTest::Main)
else()
    set(GTEST_MAIN_TARGET GTest::gtest_main)
endif()

add_executable(camera_test
        Rendering/Scene/Camera.cpp
        Rendering/Scene/Camera.hpp
        unit_test/camera_test.cpp)

target_link_libraries(camera_test
        GTest::gtest
#        GTest::main
        ${GTEST_MAIN_TARGET}  # 自动选择兼容的 target
)

enable_testing()
add_test(NAME camera_test COMMAND camera_test)