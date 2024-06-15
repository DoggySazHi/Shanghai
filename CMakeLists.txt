cmake_minimum_required(VERSION 3.14)
project(Shanghai)

set(CMAKE_CXX_STANDARD 23)

# Check if Linux
if (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")

    if (DEFINED ENV{WAYLAND_DISPLAY})
        message(STATUS "Wayland display detected; configuring for Wayland")

        add_custom_target(
                WeTrickCMakeIntoGeneratingOurStuff ALL
                DEPENDS
                ${CMAKE_CURRENT_SOURCE_DIR}/include/reimu.h
                ${CMAKE_CURRENT_SOURCE_DIR}/include/marisa.h
                ${CMAKE_CURRENT_SOURCE_DIR}/include/sanae.h
                ${CMAKE_CURRENT_SOURCE_DIR}/include/youmu.h
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

        SET(SYSTEM_SPECIFIC_SOURCE
                protogen/wlr-layer-shell-unstable-v1.c
                protogen/xdg-shell-client-protocol.c
                src/wayland/main.cpp
                src/wayland/EGLWaylandContext.cpp
                src/wayland/EGLWaylandContext.h
                src/wayland/keyboard.h
                src/wayland/pointer.h
                src/wayland/layer.h
                src/wayland/seat.h
                src/wayland/surface.h
                libs/glad/glad.c
        )

    else ()
        message(STATUS "Wayland display not detected; configuring for X11")

        SET(SYSTEM_SPECIFIC_SOURCE
                libs/glad/glad.c
                src/x11/main.cpp
        )
    endif ()
elseif (WIN32)
    message(STATUS "Configuring for Windows")

    SET(SYSTEM_SPECIFIC_SOURCE
            libs/glad/glad.c
            src/windows/main.cpp
    )
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(SHANGHAI_SOURCE
        src/Shader.cpp
        src/Shader.h
        src/Shanghai.cpp
        src/Shanghai.h
        src/ShanghaiStateMachine.cpp
        src/ShanghaiStateMachine.h
        src/states/ShanghaiStateMachineState.h
        src/states/Standing.cpp
        src/states/Standing.h
        src/state.h
        src/states/Walking.cpp
        src/states/Walking.h
        src/states/Sitting.cpp
        src/states/Sitting.h
        src/states/SittingAndLooking.cpp
        src/states/SittingAndLooking.h
        src/states/LieDown.cpp
        src/states/LieDown.h
        src/states/Crawling.cpp
        src/states/Crawling.h
        src/config/ShanghaiConfiguration.cpp
        src/config/ShanghaiConfiguration.h
        src/states/Ceiling.cpp
        src/states/Ceiling.h
        src/states/CeilingCrawl.cpp
        src/states/CeilingCrawl.h
        src/Background.cpp
        src/Background.h
        src/gl.h
        src/states/Resist.cpp
        src/states/Resist.h
        src/animation/ShanghaiAnimationMachine.cpp
        src/animation/ShanghaiAnimationMachine.h
        src/states/Dragged.cpp
        src/states/Dragged.h
        src/states/Falling.cpp
        src/states/Falling.h
        src/states/Landed.cpp
        src/states/Landed.h
        src/wayland/registry.h
        src/Random.h
        src/Random.cpp
        src/states/Jump.cpp
        src/states/Jump.h
        src/states/WallHold.cpp
        src/states/WallHold.h
        src/states/WallClimb.cpp
        src/states/WallClimb.h
)

add_executable(Shanghai
        ${SYSTEM_SPECIFIC_SOURCE}
        ${SHANGHAI_SOURCE}
)

if (UNIX AND NOT APPLE)
    if (DEFINED ENV{WAYLAND_DISPLAY})
        target_compile_definitions(Shanghai
                PRIVATE
                __WAYLAND__
        )

        target_link_libraries(Shanghai
                wayland-client
                wayland-server
                wayland-cursor
                wayland-egl
                EGL
                GLESv2
                wlroots
                jsoncpp
        )
    else ()
        find_package(X11 REQUIRED)
        find_package(OpenGL REQUIRED)
        find_package(glfw3 REQUIRED)

        target_compile_definitions(Shanghai
                PRIVATE
                __X11__
        )

        target_link_libraries(Shanghai
                X11
                Xfixes
                Xext
                jsoncpp
                OpenGL::GL
                glfw
        )
    endif ()
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)

    target_compile_definitions(Shanghai
            PRIVATE
            __WIN32__
    )

    target_link_libraries(Shanghai
            ${CMAKE_SOURCE_DIR}/libs/glfw-win/glfw3_mt.lib
            JsonCpp::JsonCpp
            OpenGL::GL)

    target_include_directories(Shanghai
            PRIVATE
            ${CMAKE_SOURCE_DIR}/libs/glfw-win/include)
endif ()

target_compile_definitions(Shanghai
        PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:DEBUG>
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
