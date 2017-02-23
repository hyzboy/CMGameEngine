SET(CM_DFS_SOURCE        dfsClientConnect.cpp

                        dfsClientID.cpp

                        dfsClientFile.cpp
                        dfsClientLoad.cpp
                        dfsClientSave.cpp
                        dfsClientSaveThread.cpp

                        dfsClientDelete.cpp

                        dfsClientRank.cpp
                        dfsClientMail.cpp)


IF(WIN32)
    ADD_LIBRARY(CM.DFS STATIC ${CM_DFS_SOURCE})
else()
    ADD_LIBRARY(CM.DFS SHARED ${CM_DFS_SOURCE})
endif()