set(UTIL_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE UTILS_SRC CONFIGURE_DEPENDS
        "${UTIL_DIR}/*.cpp"
        "${UTIL_DIR}/*.h"
)

add_library(Utils ${UTILS_SRC})

target_include_directories(Utils PUBLIC
        ${UTIL_DIR}
        ${PROJECT_SOURCE_DIR}/Src
        ${PROJECT_SOURCE_DIR}/Extern
        ${PROJECT_SOURCE_DIR}/Extern/include
        ${PROJECT_BINARY_DIR}/vcpkg_installed/x64-windows/include

)
