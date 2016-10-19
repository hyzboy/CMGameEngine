include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CDBTableViewer_SRCS CDBTableViewer.cpp main.cpp)

    set(CMAKE_AUTOMOC ON)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

    add_executable(CDBTableViewer ${HGL_GUI_TYPE} ${CDBTableViewer_SRCS} ${HGL_QT_MAIN_SOURCE})
	target_link_libraries(CDBTableViewer ${HGL_CONSOLE_LIB} ${HGL_QT_LIB})

    qt5_use_modules(CDBTableViewer Core Gui Widgets)

