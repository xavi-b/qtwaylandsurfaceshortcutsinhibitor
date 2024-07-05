project(app VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 6.2 REQUIRED COMPONENTS Gui Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)

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
