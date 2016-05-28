include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CDBTableViewer_SRCS CDBTableViewer.cpp main.cpp)

IF(BUILD_QT4_SUPPORT_LIB)
    find_package(Qt4 REQUIRED)
    include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
    qt4_automoc(${CDBTableViewer_SRCS})
    add_executable(CDBTableViewer ${CDBTableViewer_SRCS} ${HGL_QT_MAIN_SOURCE})
    target_link_libraries(CDBTableViewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${HGL_CONSOLE_LIB} ${HGL_QT_LIB})

ENDIF()

IF(BUILD_QT5_SUPPORT_LIB)

    set(CMAKE_AUTOMOC ON)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

	add_executable(CDBTableViewer ${CDBTableViewer_SRCS} ${HGL_QT_MAIN_SOURCE})
	target_link_libraries(CDBTableViewer ${HGL_CONSOLE_LIB} ${HGL_QT_LIB})

    qt5_use_modules(CDBTableViewer Core Gui Widgets)

ENDIF()

