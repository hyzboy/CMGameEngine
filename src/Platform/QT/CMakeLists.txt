IF(BUILD_QT4_SUPPORT_LIB)
    add_library(CM.QT4Support	SHARED QT4GuiApplication.cpp)
ENDIF(BUILD_QT4_SUPPORT_LIB)

IF(BUILD_QT5_SUPPORT_LIB)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

IF(WIN32)
    add_library(CM.QT5Support	STATIC QT5GuiApplication.cpp)
ELSE()
    add_library(CM.QT5Support	SHARED QT5GuiApplication.cpp)
ENDIF()

    qt5_use_modules(CM.QT5Support Core Gui Widgets)
ENDIF(BUILD_QT5_SUPPORT_LIB)
