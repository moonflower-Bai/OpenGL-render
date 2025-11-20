set(RENDER_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE RENDER_SRC CONFIGURE_DEPENDS
        "${RENDER_DIR}/*.cpp"
        "${RENDER_DIR}/*.h"
)

list(APPEND RENDER_SRC
        ${PROJECT_SOURCE_DIR}/Extern/src/glad.c
)

add_library(Rendering ${RENDER_SRC})

target_include_directories(Rendering PUBLIC
        ${RENDER_DIR}
        ${PROJECT_SOURCE_DIR}/Src
        ${PROJECT_SOURCE_DIR}/Extern
        ${PROJECT_SOURCE_DIR}/Extern/include
        ${PROJECT_BINARY_DIR}/vcpkg_installed/x64-windows/include
)

target_link_libraries(Rendering PUBLIC
        Core
        Utils
        glfw
        glm::glm
        opengl32
)
