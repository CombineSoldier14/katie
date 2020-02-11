# - Try to find ODBC
# Once done this will define
#
#  ODBC_FOUND - system has ODBC
#  ODBC_INCLUDES - the ODBC include directory
#  ODBC_LIBRARIES - the libraries needed to use ODBC
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(ODBC_INCLUDES AND ODBC_LIBRARIES)
    set(ODBC_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_ODBC QUIET odbc)
    if(NOT PC_ODBC_FOUND)
        pkg_check_modules(PC_ODBC QUIET libiodbc)
    endif()

    set(ODBC_INCLUDES ${PC_ODBC_INCLUDE_DIRS})
    set(ODBC_LIBRARIES ${PC_ODBC_LIBRARIES})
endif()

if(NOT ODBC_INCLUDES OR NOT ODBC_LIBRARIES)
    find_path(ODBC_INCLUDES
        NAMES sql.h
        PATH_SUFFIXES iodbc libiodbc
        HINTS $ENV{ODBCDIR}/include
    )

    find_library(ODBC_LIBRARIES
        NAMES odbc iodbc
        HINTS $ENV{ODBCDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODBC
    VERSION_VAR PC_ODBC_VERSION
    REQUIRED_VARS ODBC_LIBRARIES ODBC_INCLUDES
)

mark_as_advanced(ODBC_INCLUDES ODBC_LIBRARIES)
