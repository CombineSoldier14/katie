include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/generic
    ${CMAKE_CURRENT_BINARY_DIR}/bearer
)

set(QGENERICBEARERPLUGIN_HEADERS
    ${QGENERICBEARERPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/platformdefs_win.h
)

set(QGENERICBEARERPLUGIN_SOURCES
    ${QGENERICBEARERPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

add_library(qgenericbearerplugin MODULE ${QGENERICBEARERPLUGIN_SOURCES} ${QGENERICBEARERPLUGIN_HEADERS})
target_link_libraries(qgenericbearerplugin KtCore KtNetwork)
set_target_properties(qgenericbearerplugin PROPERTIES OUTPUT_NAME qgenericbearer)

katie_setup_target(qgenericbearerplugin)

install(
    TARGETS qgenericbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
