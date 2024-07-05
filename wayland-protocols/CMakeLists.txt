project(wayland-protocols VERSION 0.1 LANGUAGES C)

list(APPEND PROTOCOLS "keyboard-shortcuts-inhibit-unstable-v1")
set(PROTOCOL_SUFFIX "client-protocol")

set(SOURCES "")
foreach(PROTOCOL_NAME IN LISTS PROTOCOLS)
    set(PROTOCOL_XML ${CMAKE_CURRENT_SOURCE_DIR}/${PROTOCOL_NAME}.xml)
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
    list(APPEND SOURCES ${PROTOCOL_C} ${PROTOCOL_H})
endforeach()

add_library(wayland-protocols
    STATIC
    ${SOURCES})

include(GNUInstallDirs)
install(TARGETS wayland-protocols
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
