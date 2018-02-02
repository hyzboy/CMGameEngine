include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    set(CMAKE_AUTOMOC ON)

    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets)

    add_executable(TextEncodingConvertQt ${HGL_GUI_TYPE} ${HGL_QT_MAIN_SOURCE}
                                    MainUnit.cpp
									MainWindow.cpp)

	target_link_libraries(TextEncodingConvertQt ${HGL_CONSOLE_LIB} ${HGL_QT_LIB})

    qt5_use_modules(TextEncodingConvertQt Core Gui Widgets)

