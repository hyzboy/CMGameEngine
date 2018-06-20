IF(BUILD_QT5_SUPPORT_LIB)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

IF(WIN32)
    add_library(CM.QT5Support	STATIC QT5GuiApplication.cpp)
ELSE()
    add_library(CM.QT5Support	SHARED QT5GuiApplication.cpp)
ENDIF()

    target_link_libraries(CM.QT5Support Qt5::Core Qt5::Gui Qt5::Widgets)
ENDIF(BUILD_QT5_SUPPORT_LIB)
