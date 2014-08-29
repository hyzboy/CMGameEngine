find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(CDBTableViewer_SRCS CDBTableViewer.cpp main.cpp)
qt4_automoc(${CDBTableViewer_SRCS})
add_executable(CDBTableViewer ${CDBTableViewer_SRCS} ${HGL_QT_MAIN_SOURCE})
target_link_libraries(CDBTableViewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${HGL_CONSOLE_LIB} ${HGL_QT_LIB})
