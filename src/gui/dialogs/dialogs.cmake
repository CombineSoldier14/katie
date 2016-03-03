set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractprintdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractprintdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractpagesetupdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractpagesetupdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qcolordialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qcolordialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfscompleter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qerrormessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfontdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfontdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qinputdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qmessagebox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprogressdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qsidebar_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfilesystemmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfilesystemmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfileinfogatherer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qwizard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintpreviewdialog.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractprintdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qabstractpagesetupdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qcolordialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qerrormessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfontdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qinputdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qmessagebox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprogressdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qsidebar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfilesystemmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfileinfogatherer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qwizard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintpreviewdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintdialog.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qmessagebox.qrc
)

if(${KATIE_PLATFORM} STREQUAL "wince")
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog_embedded.ui
    )
elseif(${KATIE_PLATFORM} STREQUAL "win32")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qwizard_win_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog_win_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintdialog_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qwizard_win.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        shell32     # the filedialog needs this library
    )
elseif(${KATIE_PLATFORM} STREQUAL "mac")
    # TODO: Compile qcolordialog_mac.mm with exception support, disregarding
    # the -no-exceptions configure option. (qcolordialog_mac needs to catch
    # exceptions thrown by cocoa)
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfiledialog_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qfontdialog_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qnspanelproxy_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintdialog_mac.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qcolordialog_mac.mm
    )
elseif(KATIE_PLATFORM MATCHES "(linux|freebsd|openbsd|qpa)")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog_unix_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintdialog_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qpagesetupdialog_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintsettingsoutput.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintwidget.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/dialogs/qprintpropertieswidget.ui
    )
endif()
