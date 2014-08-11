INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/hiredis)

set(HIREDIS	hiredis/async.c
			hiredis/dict.c
			hiredis/hiredis.c
			hiredis/net.c
			hiredis/sds.c)

add_library(RedisClient STATIC
			${HIREDIS}
			RedisDB.cpp
			RedisDBStrings.cpp
			RedisDBStringBinary.cpp
			RedisDBStringNumber.cpp
			RedisDBSets.cpp
			RedisDBSortedSets.cpp
			RedisDBHashes.cpp)
