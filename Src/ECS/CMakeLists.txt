set(ECS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ECS_SRC CONFIGURE_DEPENDS
        "${ECS_DIR}/*.cpp"
        "${ECS_DIR}/*.h"
)

add_library(ECS ${ECS_SRC})

target_include_directories(ECS PUBLIC
        ${ECS_DIR}
)
