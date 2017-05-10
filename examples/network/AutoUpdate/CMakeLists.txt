add_executable(Example.Network.LinuxAutoUpdate  StandAlone/LinuxAutoUpdate.cpp
                                StandAlone/md5.cpp
                                StandAlone/XMLParseClass.cpp
                                StandAlone/HTTPClient.cpp
                                StandAlone/FileSystem.cpp)

target_link_libraries(Example.Network.LinuxAutoUpdate bz2 expat curl)

add_executable(Example.Network.LinuxMakeUpdate  StandAlone/LinuxMakeUpdate.cpp
                                StandAlone/md5.cpp
                                StandAlone/FileSystem.cpp)

target_link_libraries(Example.Network.LinuxMakeUpdate bz2)
