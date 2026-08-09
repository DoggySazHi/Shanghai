cmake_minimum_required(VERSION 3.14)
project(Shanghai)

set(CMAKE_CXX_STANDARD 23)

option(SHANGHAI_ENABLE_NOTIFICATIONS "Register Shanghai as the org.freedesktop.Notifications DBus service (requires libsystemd/sd-bus)" ON)

set(SHANGHAI_NOTIFICATIONS_AVAILABLE FALSE)
if (SHANGHAI_ENABLE_NOTIFICATIONS AND UNIX AND NOT APPLE)
    find_library(SHANGHAI_SDBUS_LIBRARY NAMES systemd)
    find_path(SHANGHAI_SDBUS_INCLUDE_DIR NAMES systemd/sd-bus.h)

    if (SHANGHAI_SDBUS_LIBRARY AND SHANGHAI_SDBUS_INCLUDE_DIR)
        set(SHANGHAI_NOTIFICATIONS_AVAILABLE TRUE)
        message(STATUS "libsystemd/sd-bus found; notification server support enabled")
    else ()
        message(WARNING "SHANGHAI_ENABLE_NOTIFICATIONS is ON but libsystemd/sd-bus was not found; building without notification server support")
    endif ()
endif ()

set(SHANGHAI_BACKEND "" CACHE STRING "Linux display backend to build: x11 or wayland")

# Check if Linux
if (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")

    if (SHANGHAI_BACKEND STREQUAL "")
        if (DEFINED ENV{WAYLAND_DISPLAY})
            set(SHANGHAI_BACKEND "wayland")
        else ()
            set(SHANGHAI_BACKEND "x11")
        endif ()
        message(STATUS "SHANGHAI_BACKEND not set; guessed '${SHANGHAI_BACKEND}' from the environment")
    endif ()

    if (SHANGHAI_BACKEND STREQUAL "wayland")
        message(STATUS "Configuring for Wayland")

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
        )

        if (SHANGHAI_NOTIFICATIONS_AVAILABLE)
            list(APPEND SYSTEM_SPECIFIC_SOURCE
                    src/notifications/NotificationServer.cpp
                    src/notifications/NotificationServer.h
            )
        endif ()

    elseif (SHANGHAI_BACKEND STREQUAL "x11")
        message(STATUS "Configuring for X11")

        SET(SYSTEM_SPECIFIC_SOURCE
                libs/glad/glad.c
                src/platform/GlfwMain.cpp
                src/platform/stb_image_impl.cpp
                src/x11/Platform.cpp
        )

        if (SHANGHAI_NOTIFICATIONS_AVAILABLE)
            list(APPEND SYSTEM_SPECIFIC_SOURCE
                    src/notifications/NotificationServer.cpp
                    src/notifications/NotificationServer.h
            )
        endif ()
    else ()
        message(FATAL_ERROR "Unknown SHANGHAI_BACKEND '${SHANGHAI_BACKEND}'; expected 'x11' or 'wayland'")
    endif ()
elseif (WIN32)
    message(STATUS "Configuring for Windows")

    SET(SYSTEM_SPECIFIC_SOURCE
            libs/glad/glad.c
            src/platform/GlfwMain.cpp
            src/platform/stb_image_impl.cpp
            src/windows/Platform.cpp
    )
elseif (APPLE)
    message(STATUS "Configuring for macOS")

    # Platform.mm pokes at NSWindow/NSEvent for the things GLFW does not expose
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_STANDARD 23)

    SET(SYSTEM_SPECIFIC_SOURCE
            libs/glad/glad.c
            src/platform/GlfwMain.cpp
            src/platform/stb_image_impl.cpp
            src/macos/Platform.mm
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
        src/states/Cheering.cpp
        src/states/Cheering.h
        src/states/Throwing.cpp
        src/states/Throwing.h
        src/fonts/BDF.cpp
        src/fonts/BDF.h
        src/fonts/FontRenderer.cpp
        src/fonts/FontRenderer.h
        src/9patch/NinePatch.cpp
        src/9patch/NinePatch.h
        src/gl/Quad.cpp
        src/gl/Quad.h
        src/platform/Platform.h
        src/speechbubble/SpeechBubble.cpp
        src/speechbubble/SpeechBubble.h
)

add_executable(Shanghai
        ${SYSTEM_SPECIFIC_SOURCE}
        ${SHANGHAI_SOURCE}
)

if (UNIX AND NOT APPLE)
    if (SHANGHAI_BACKEND STREQUAL "wayland")
        target_compile_definitions(Shanghai
                PRIVATE
                SHANGHAI_PLATFORM_WAYLAND
                SHANGHAI_GL_ES
        )

        target_link_libraries(Shanghai
                wayland-client
                wayland-server
                wayland-cursor
                wayland-egl
                EGL
                GLESv2
                jsoncpp
        )
    else ()
        find_package(X11 REQUIRED)
        find_package(OpenGL REQUIRED)
        find_package(glfw3 REQUIRED)

        target_compile_definitions(Shanghai
                PRIVATE
                SHANGHAI_PLATFORM_X11
                SHANGHAI_GL_CORE
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

    if (SHANGHAI_NOTIFICATIONS_AVAILABLE)
        target_compile_definitions(Shanghai PRIVATE SHANGHAI_ENABLE_NOTIFICATIONS)
        target_link_libraries(Shanghai ${SHANGHAI_SDBUS_LIBRARY})
        target_include_directories(Shanghai PRIVATE ${SHANGHAI_SDBUS_INCLUDE_DIR})
    endif ()
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)

    target_compile_definitions(Shanghai
            PRIVATE
            SHANGHAI_PLATFORM_WINDOWS
            SHANGHAI_GL_CORE
    )

    target_link_libraries(Shanghai
            ${CMAKE_SOURCE_DIR}/libs/glfw-win/glfw3_mt.lib
            JsonCpp::JsonCpp
            OpenGL::GL)

    target_include_directories(Shanghai
            PRIVATE
            ${CMAKE_SOURCE_DIR}/libs/glfw-win/include)
elseif (APPLE)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)

    target_compile_definitions(Shanghai
            PRIVATE
            SHANGHAI_PLATFORM_MACOS
            SHANGHAI_GL_CORE
    )

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(CORE_GRAPHICS_LIBRARY CoreGraphics REQUIRED)

    target_link_libraries(Shanghai
            glfw
            JsonCpp::JsonCpp
            OpenGL::GL
            ${COCOA_LIBRARY}
            ${CORE_GRAPHICS_LIBRARY})
endif ()

target_compile_definitions(Shanghai
        PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:DEBUG>
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TamzenForPowerline10x20r.bdf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
