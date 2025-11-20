set(INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE INPUT_SRC CONFIGURE_DEPENDS
        "${INPUT_DIR}/*.cpp"
        "${INPUT_DIR}/*.h"
)

add_library(Input ${INPUT_SRC})

target_include_directories(Input PUBLIC
        ${INPUT_DIR}
)
