cmake_minimum_required(VERSION 3.14)
project(Shanghai)

set(CMAKE_CXX_STANDARD 23)

add_custom_target(
        WeTrickCMakeIntoGeneratingOurStuff ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/reimu.h
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/reimu.h ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h
        COMMAND wayland-scanner client-header ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(WL_GENERATED_PRIVATE_CODE
        protogen/wlr-layer-shell-unstable-v1.c
        protogen/xdg-shell-client-protocol.c
)

SET(SHANGHAI_SOURCE
        src/main.cpp
        src/EGLWaylandContext.cpp
        src/EGLWaylandContext.h
)

add_executable(Shanghai
        ${WL_GENERATED_PRIVATE_CODE}
        ${SHANGHAI_SOURCE}
)

target_link_libraries(Shanghai
        wayland-client
        wayland-server
        wayland-cursor
        wayland-egl
        EGL
        GLESv2
        wlroots
)
