
add_executable(csv2table Common.cpp
                        LoaderCSV.cpp
                        LoaderODS.cpp
                        LoaderXLSX.cpp
                        WriteTable.cpp
                        Main.cpp
                        ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(csv2table ${HGL_CONSOLE_LIB} zip)
