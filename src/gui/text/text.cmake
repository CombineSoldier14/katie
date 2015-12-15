set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengineglyphcache_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontmetrics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontsubset_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol_p_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextengine_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextoption.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfragmentmap_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexthtmlparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstracttextdocumentlayout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentlayout_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextimagehandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qsyntaxhighlighter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentwriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qcssparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzipreader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzipwriter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextodfwriter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qglyphrun.h
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qglyphrun_p.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontsubset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontmetrics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcontrol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextformat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextoption.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qfragmentmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocument_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexthtmlparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstracttextdocumentlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentlayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentfragment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextimagehandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtexttable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextlist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextdocumentwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qsyntaxhighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qcssparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qzip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qtextodfwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qstatictext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/qglyphrun.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src)
add_definitions(-DQT_NO_OPENTYPE)

if(WITH_FREETYPE AND FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${FREETYPE_LIBRARIES}
    )
endif()

if(WITH_FONTCONFIG AND FONTCONFIG_FOUND)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${FONTCONFIG_LIBRARIES}
    )
    add_definitions(${FONTCONFIG_DEFINITIONS})
endif()

if(WITH_DIRECTWRITE)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontenginedirectwrite_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontenginedirectwrite.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        dwrite
    )
endif()

if(UNIX AND WITH_X11 AND X11_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_x11_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontdatabase_x11.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_x11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_ft.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "win32")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_win_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_win.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_mac_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_coretext_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_coretext.mm
    )
    if(WITH_HARFBUZZ)
        add_definitions(-DQT_ENABLE_HARFBUZZ_FOR_MAC)
    endif()
elseif(KATIE_PLATFORM STREQUAL "qpa")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qplatformfontdatabase_qpa.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qplatformfontdatabase_qpa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_qpa.cpp
    )
    add_definitions(-DQT_NO_FONTCONFIG -DQT_NO_FREETYPE)
elseif(KATIE_PLATFORM STREQUAL "qws")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_qpf_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstractfontengine_qws.h
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstractfontengine_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfont_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_ft.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qfontengine_qpf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qabstractfontengine_qws.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/text/qrawfont_ft.cpp
    )
    add_definitions(-DQT_NO_FONTCONFIG)
endif()
