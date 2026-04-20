cmake_minimum_required(VERSION 3.29)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(_HAS_STD_BYTE=0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 依赖
# find_package(glfw3 CONFIG REQUIRED)
# find_package(glm CONFIG REQUIRED)
if (UNIX)
    find_package(OpenGL REQUIRED)
#     find_package(glad REQUIRED)
endif()

# 外部依赖 include
set(EXTERN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Extern/include)
set(EXTERN_LIB_DIR ${PROJECT_SOURCE_DIR}/Extern/lib)

link_directories(${EXTERN_LIB_DIR})

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
        ${EXTERN_INCLUDE_DIR}
)

target_link_libraries(LearnOpenGL PRIVATE
        Core
        Utils
        Rendering
        ECS
        Input
        # glm::glm
        glfw3
)

if (WIN32)
    target_link_libraries(LearnOpenGL PRIVATE opengl32)
elseif(LINUX)
    target_link_libraries(LearnOpenGL PRIVATE OpenGL::GL)
endif()

# Copy shaders directory after build
add_custom_command(TARGET LearnOpenGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Src/Rendering/Assets/fluid/GPU_process/shaders
        $<TARGET_FILE_DIR:LearnOpenGL>/shaders
)
