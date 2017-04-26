add_executable(LinuxAutoUpdate  StandAlone/LinuxAutoUpdate.cpp
                                StandAlone/md5.cpp
                                StandAlone/XMLParseClass.cpp
                                StandAlone/HTTPClient.cpp
                                StandAlone/FileSystem.cpp)

target_link_libraries(LinuxAutoUpdate bz2 expat curl)
