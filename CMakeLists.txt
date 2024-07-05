cmake_minimum_required(VERSION 3.16)

project(qtwaylandsurfaceshortcutsinhibitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Gui Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
set(PROTOCOL_NAME "keyboard-shortcuts-inhibit-unstable-v1")
set(PROTOCOL_SUFFIX "client-protocol")
set(PROTOCOL_XML ${CMAKE_SOURCE_DIR}/${PROTOCOL_NAME}.xml)
set(PROTOCOL_C ${CMAKE_BINARY_DIR}/${PROTOCOL_NAME}-${PROTOCOL_SUFFIX}.c)
set(PROTOCOL_H ${CMAKE_BINARY_DIR}/${PROTOCOL_NAME}-${PROTOCOL_SUFFIX}.h)
add_custom_command(
    OUTPUT ${PROTOCOL_C}
    COMMAND wayland-scanner private-code ${PROTOCOL_XML} ${PROTOCOL_C}
    DEPENDS ${PROTOCOL_XML}
    COMMENT "Generating ${PROTOCOL_C} from ${PROTOCOL_XML}"
)
add_custom_command(
    OUTPUT ${PROTOCOL_H}
    COMMAND wayland-scanner client-header ${PROTOCOL_XML} ${PROTOCOL_H}
    DEPENDS ${PROTOCOL_XML}
    COMMENT "Generating ${PROTOCOL_H} from ${PROTOCOL_XML}"
)
add_library(wayland-protocols
    STATIC
    ${PROTOCOL_C}
    ${PROTOCOL_H})
set_target_properties(wayland-protocols PROPERTIES LINKER_LANGUAGE CXX)

qt_standard_project_setup()

qt_add_executable(appqtwaylandsurfaceshortcutsinhibitor
    main.cpp
)

target_link_libraries(appqtwaylandsurfaceshortcutsinhibitor
    PRIVATE
    Qt6::Gui
    Qt6::Widgets
    ${WAYLAND_CLIENT_LIBRARIES}
    wayland-protocols
)

target_include_directories(appqtwaylandsurfaceshortcutsinhibitor
    PRIVATE
    ${Qt6Gui_PRIVATE_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}
)

include(GNUInstallDirs)
install(TARGETS appqtwaylandsurfaceshortcutsinhibitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
