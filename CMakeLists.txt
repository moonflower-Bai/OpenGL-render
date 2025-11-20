cmake_minimum_required(VERSION 3.29)
#set(VCPKG_MANIFEST_MODE OFF)
project(LearnOpenGL)

add_compile_definitions(_HAS_STD_BYTE=0)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#set(CMAKE_PREFIX_PATH "C:/Users/Jingren Bai/vcpkg/installed/x64-mingw-static;${CMAKE_PREFIX_PATH}")
#set(glfw3_DIR ${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-mingw-static/share/glfw3)
#set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")

if(UNIX)
    find_package(OpenGL REQUIRED)  # Linux 需要显式链接
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
#find_package(GTest CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Extern)


add_executable(LearnOpenGL main.cpp
        utils/log.cpp
        Rendering/Primitives/old_primitive_pipeline/Triangle/TriangleRender.cpp
        Rendering/Primitives/old_primitive_pipeline/Triangle/TriangleRender.h
        utils/ReadShader.cpp
        utils/ReadShader.h
        Rendering/Scene/Camera.cpp
        Rendering/Scene/Camera.hpp
        Rendering/Primitives/old_primitive_pipeline/Base/ObjectRender.h
        Rendering/Pipeline/MainRenderThread_Primitive.cpp
        Rendering/Pipeline/MainRenderThread_Primitive.h
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
        Rendering/Assets/ModelLoader.cpp
        Rendering/Assets/ModelLoader.h
        Rendering/Scene/Model.cpp
        Rendering/Scene/Model.h
        Rendering/Scene/Quaternions.cpp
        Rendering/Scene/Quaternions.h
        Rendering/Primitives/old_primitive_pipeline/Point/PointRender.cpp
        Rendering/Primitives/old_primitive_pipeline/Point/PointRender.h
        Rendering/Assets/fluid/CPU_process/FluidSimulator.cpp
        Rendering/Assets/fluid/CPU_process/FluidSimulator.h
        Rendering/Assets/fluid/CPU_process/Particle.cpp
        Rendering/Assets/fluid/CPU_process/Particle.h
        Rendering/Primitives/old_primitive_pipeline/Ball/BallRender.cpp
        Rendering/Primitives/old_primitive_pipeline/Ball/BallRender.h
        Rendering/Primitives/old_primitive_pipeline/Base/RenderConfig.h
        Rendering/Primitives/Entity/Entity.cpp
        Rendering/Primitives/Entity/Entity.h
        Rendering/Primitives/Components/Component.cpp
        Rendering/Primitives/Components/Component.h
        Rendering/Assets/fluid/GPU_process/GPU_FluidSimulator.cpp
        Rendering/Assets/fluid/GPU_process/GPU_FluidSimulator.h
        Rendering/Pipeline/RenderThread_ECS.cpp
        Rendering/Pipeline/RenderThread_ECS.h
        Extern/include/glad/glad.h
        Extern/include/KHR/khrplatform.h
        Extern/src/glad.c
        Rendering/Assets/fluid/GPU_process/GPU_FluidRender.cpp
        Rendering/Assets/fluid/GPU_process/GPU_FluidRender.h
        Rendering/Assets/fluid/GPU_process/GPU_Particle.h
        Rendering/Primitives/Components/CameraComponent.cpp
        Rendering/Primitives/Components/CameraComponent.h
        Rendering/Primitives/Entity/CameraEntity.cpp
        Rendering/Primitives/Entity/CameraEntity.h
        Extern/json.hpp
        #        Input/InputSystem.cpp
#        Input/InputSystem.h
)

target_link_libraries(LearnOpenGL PRIVATE
        glfw
        opengl32  # Windows 下需要显式链接
        $<$<BOOL:${UNIX}>:OpenGL::GL>  # 仅 Linux 添加
)
target_compile_options(LearnOpenGL PRIVATE /arch:AVX) # MSVC

# 3. 智能处理 GTest 兼容性
#if(TARGET GTest::Main)
#    set(GTEST_MAIN_TARGET GTest::Main)
#else()
#    set(GTEST_MAIN_TARGET GTest::gtest_main)
#endif()

add_executable(camera_test
        Rendering/Scene/Camera.cpp
        Rendering/Scene/Camera.hpp
        unit_test/camera_test.cpp
        Rendering/Primitives/old_primitive_pipeline/Base/RenderBuilder.h)

#target_link_libraries(camera_test
#        GTest::gtest
##        GTest::main
#        ${GTEST_MAIN_TARGET}  # 自动选择兼容的 target
#)

enable_testing()
add_test(NAME camera_test COMMAND camera_test)