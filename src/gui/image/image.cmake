set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qbitmap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qicon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qicon_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconengineplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimageiohandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagereader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagewriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qmovie.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qnativeimage_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpaintengine_pic_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpicture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpicture_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_raster_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapcache_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapdata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapdatafactory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapfilter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagepixmapcleanuphooks_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qvolatileimage_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qvolatileimagedata_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qnativeimagehandleprovider_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qbmphandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qppmhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qxbmhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qxpmhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qgifhandler_p.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qbitmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qicon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimageiohandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagereader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagewriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpaintengine_pic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpicture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapdatafactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmapfilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qiconengineplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qmovie.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_raster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qnativeimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimagepixmapcleanuphooks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qvolatileimage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qvolatileimagedata.cpp
    # Built-in image format support
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qbmphandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qppmhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qxbmhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qxpmhandler.cpp
    # Optional but with no external dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qgifhandler.cpp
)

katie_setup_sources(
    GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage_neon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage_sse2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image/qimage_ssse3.cpp
)

if(WITH_PNG AND PNG_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpnghandler_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpnghandler.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${PNG_LIBRARIES}
    )
    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})
endif()

if(WITH_JPEG AND JPEG_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qjpeghandler_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qjpeghandler.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${JPEG_LIBRARIES}
    )
    include_directories(${JPEG_INCLUDE_DIR})
endif()

if(WITH_MNG AND MNG_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qmnghandler_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qmnghandler.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${MNG_LIBRARIES}
    )
    include_directories(${MNG_INCLUDES})
endif()

if(WITH_TIFF AND TIFF_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qtiffhandler_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qtiffhandler.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${TIFF_LIBRARIES}
    )
    include_directories(${TIFF_INCLUDE_DIR})
endif()

if(KATIE_PLATFORM STREQUAL "win32")
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_win.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_mac_p.cpp
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_mac.cpp
    )
elseif(WITH_X11 AND X11_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_x11_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/image/qpixmap_x11.cpp
    )
endif()
