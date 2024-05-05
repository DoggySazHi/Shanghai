cmake_minimum_required(VERSION 3.14)
project(Shanghai)

set(CMAKE_CXX_STANDARD 23)

add_custom_target(
        WeTrickCMakeIntoGeneratingOurStuff ALL
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/include/reimu.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/marisa.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/sanae.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/youmu.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/yuyuko.h
)

# Create the header file for the layer shell protocol
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/reimu.h ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h
        COMMAND wayland-scanner client-header ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h && sed -i 's/namespace/wlrNamespace/g' ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h && sed -i 's/a wlrNamespace/a namespace/g' ${CMAKE_CURRENT_SOURCE_DIR}/include/wlr-layer-shell-unstable-v1-client-protocol.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml
)

# Create the C file for the layer shell protocol
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/marisa.h ${CMAKE_CURRENT_SOURCE_DIR}/protogen/wlr-layer-shell-unstable-v1.c
        COMMAND wayland-scanner private-code ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml ${CMAKE_CURRENT_SOURCE_DIR}/protogen/wlr-layer-shell-unstable-v1.c
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/protocol/wlr-layer-shell-unstable-v1.xml
)

# Create the header file for the xdg shell protocol
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/sanae.h ${CMAKE_CURRENT_SOURCE_DIR}/include/xdg-shell-client-protocol.h
        COMMAND wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ${CMAKE_CURRENT_SOURCE_DIR}/include/xdg-shell-client-protocol.h
        DEPENDS /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
)

# Create the C file for the xdg shell protocol
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/youmu.h ${CMAKE_CURRENT_SOURCE_DIR}/protogen/xdg-shell-client-protocol.c
        COMMAND wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ${CMAKE_CURRENT_SOURCE_DIR}/protogen/xdg-shell-client-protocol.c
        DEPENDS /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
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

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
