INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/hiredis)

FILE(GLOB_RECURSE HIREDIS_SOURCES "hiredis/*.c")
FILE(GLOB_RECURSE HIREDIS_HEADERS "hiredis/*.h")

add_library(RedisClient STATIC
			${HIREDIS_SOURCES}
			${HIREDIS_HEADERS}
			RedisDB.cpp
			RedisDBStrings.cpp
			RedisDBStringBinary.cpp
			RedisDBStringNumber.cpp
			RedisDBSets.cpp
			RedisDBSortedSets.cpp
			RedisDBHashes.cpp)
