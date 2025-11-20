cmake_minimum_required(VERSION 3.29)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(_HAS_STD_BYTE=0)

# 依赖
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
if (UNIX)
    find_package(OpenGL REQUIRED)
endif()

# 外部依赖 include
set(EXTERN_DIR ${PROJECT_SOURCE_DIR}/Extern)

# 模块
add_subdirectory(Src/Core)
add_subdirectory(Src/Utils)
add_subdirectory(Src/ECS)
add_subdirectory(Src/Rendering)
add_subdirectory(Src/Input)

# main程序
add_executable(LearnOpenGL
        Src/main.cpp
)

target_include_directories(LearnOpenGL PRIVATE
        Src
        ${EXTERN_DIR}
        ${EXTERN_DIR}/include
)

target_link_libraries(LearnOpenGL PRIVATE
        Core
        Utils
        Rendering
        ECS
        Input
        glm::glm
        glfw
        opengl32
        $<$<BOOL:${UNIX}>:OpenGL::GL>
)

# Copy shaders directory after build
add_custom_command(TARGET LearnOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Src/Rendering/Assets/fluid/GPU_process/shaders
        $<TARGET_FILE_DIR:LearnOpenGL>/shaders
)
