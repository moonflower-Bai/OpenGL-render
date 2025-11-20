set(INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE INPUT_SRC CONFIGURE_DEPENDS
        "${INPUT_DIR}/*.cpp"
        "${INPUT_DIR}/*.h"
)

add_library(Input ${INPUT_SRC})

target_include_directories(Input PUBLIC
        ${INPUT_DIR}
        ${PROJECT_SOURCE_DIR}/Src
        ${PROJECT_SOURCE_DIR}/Extern
        ${PROJECT_SOURCE_DIR}/Extern/include
        ${PROJECT_BINARY_DIR}/vcpkg_installed/x64-windows/include

)
