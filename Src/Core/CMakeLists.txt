set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
        "${CORE_DIR}/*.cpp"
        "${CORE_DIR}/*.h"
)

add_library(Core ${CORE_SRC})

target_include_directories(Core PUBLIC
        ${CORE_DIR}
        ${PROJECT_SOURCE_DIR}/Src          # 允许 include "Rendering/xxx.h"
        ${PROJECT_SOURCE_DIR}/Extern
        ${PROJECT_SOURCE_DIR}/Extern/include
        ${PROJECT_BINARY_DIR}/vcpkg_installed/x64-windows/include  # glm / eigen
)
