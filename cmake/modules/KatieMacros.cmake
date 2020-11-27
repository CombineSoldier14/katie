# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
# Redistribution and use is allowed according to the terms of the BSD license.

macro(KATIE_RESOURCES RESOURCES)
    foreach(tmpres ${RESOURCES} ${ARGN})
        get_filename_component(resource "${tmpres}" ABSOLUTE)
        get_source_file_property(skip "${resource}" SKIP_RESOURCE)
        if(NOT skip)
            get_filename_component(rscext "${resource}" EXT)
            get_filename_component(rscname "${resource}" NAME_WE)
            get_filename_component(rscpath "${resource}" PATH)
            string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" rscpath "${rscpath}")
            if("${rscext}" STREQUAL ".ui")
                set(rscout "${rscpath}/ui_${rscname}.h")
                make_directory("${rscpath}")
                include_directories("${rscpath}")
                add_custom_command(
                    COMMAND "${KATIE_UIC}" "${resource}" -o "${rscout}"
                    OUTPUT "${rscout}"
                    MAIN_DEPENDENCY "${resource}"
                )
            elseif("${rscext}" STREQUAL ".qrc")
                set(rscout "${rscpath}/qrc_${rscname}.cpp")
                make_directory("${rscpath}")
                include_directories("${rscpath}")
                add_custom_command(
                    COMMAND "${KATIE_RCC}" "${resource}" -o "${rscout}" -name "${rscname}"
                    OUTPUT "${rscout}"
                    MAIN_DEPENDENCY ${resource}
                )
                set_property(SOURCE "${resource}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
            elseif("${rscext}" MATCHES "(.h|.hpp|.cc|.cpp)")
                file(READ "${resource}" rsccontent)
                # this can be simpler if continue() was supported by old CMake versions
                if("${rsccontent}" MATCHES "(Q_OBJECT|Q_OBJECT_FAKE|Q_GADGET)")
                    set(rscout "${rscpath}/moc_${rscname}${rscext}")
                    get_directory_property(dirdefs COMPILE_DEFINITIONS)
                    get_directory_property(dirincs INCLUDE_DIRECTORIES)
                    set(mocargs)
                    # COMPILE_DEFINITIONS does not include undefine definitions
                    foreach(ddef ${dirdefs})
                        set(mocargs ${mocargs} -D${ddef})
                    endforeach()
                    foreach(incdir ${dirincs})
                        set(mocargs ${mocargs} -I${incdir})
                    endforeach()
                    make_directory("${rscpath}")
                    include_directories("${rscpath}")
                    add_custom_command(
                        COMMAND "${KATIE_MOC}" -nw "${resource}" -o "${rscout}" ${mocargs}
                        OUTPUT "${rscout}"
                    )
                    set_property(SOURCE "${resource}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
                endif()
            endif()
        endif()
    endforeach()
endmacro()

macro(KATIE_DBUS_ADAPTOR SRCDEP SRCIN OUTNAME)
    get_filename_component(resource "${SRCIN}" ABSOLUTE)
    set(rscout "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}.h")
    set(mocout "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}.moc")
    add_custom_command(
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${resource}" -a "${rscout}" -p "${OUTNAME}" ${ARGN}
        COMMAND "${KATIE_MOC}" -nw "${rscout}" -o "${mocout}" -i
        OUTPUT "${rscout}"
    )
    set_property(SOURCE ${SRCDEP} APPEND PROPERTY OBJECT_DEPENDS ${rscout})
endmacro()

macro(KATIE_DBUS_INTERFACE SRCIN)
    string(REGEX MATCH ".*\\.(.*)\\.xml" ${SRCIN} OUTNAME)
    string(TOLOWER "${SRCIN}" SRCIN)
    set(rscout "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}interface.h")
    add_custom_command(
        COMMAND "${KATIE_QDBUSXML2CPP}" -m "${SRCIN}" -a "${rscout}" -p "${OUTNAME}interface" ${ARGN}
        OUTPUT "${rscout}"
    )
    set_property(SOURCE "${SRCIN}" APPEND PROPERTY OBJECT_DEPENDS "${rscout}")
endmacro()

macro(KATIE_DEFINITION DEF)
    set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} ${DEF} ${ARGN})
    add_definitions(${DEF} ${ARGN})
endmacro()
