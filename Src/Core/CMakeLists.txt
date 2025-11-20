set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
        "${CORE_DIR}/*.cpp"
        "${CORE_DIR}/*.h"
)

add_library(Core ${CORE_SRC})

target_include_directories(Core PUBLIC
        ${CORE_DIR}
        ${EXTERN_DIR}
)
