set(RENDER_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE RENDER_SRC CONFIGURE_DEPENDS
        "${RENDER_DIR}/*.cpp"
        "${RENDER_DIR}/*.h"
)

add_library(Rendering ${RENDER_SRC})

target_include_directories(Rendering PUBLIC
        ${RENDER_DIR}
        ${EXTERN_DIR}
        ${EXTERN_DIR}/include
)

target_link_libraries(Rendering PUBLIC
        Core
        Utils
        glm::glm
        glfw
)
