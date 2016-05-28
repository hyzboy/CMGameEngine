IF(BUILD_QT4_SUPPORT_LIB)
    add_library(QT4Support	QT4GuiApplication.cpp)
ENDIF(BUILD_QT4_SUPPORT_LIB)

IF(BUILD_QT5_SUPPORT_LIB)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

    add_library(QT5Support	QT5GuiApplication.cpp)

    qt5_use_modules(QT5Support Core Gui Widgets)
ENDIF(BUILD_QT5_SUPPORT_LIB)
