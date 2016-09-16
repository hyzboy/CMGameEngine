# IF(WIN32)
#     INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/include)
#     INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opusfile/include)
#
#     SET(OPUS_SOURCE	libogg/src/bitwise.c
#                 	libogg/src/framing.c)
#
#     SET(OPUSFILE_SOURCE	libvorbis/lib/analysis.c
# 		                libvorbis/lib/bitrate.c
# 		                libvorbis/lib/block.c
# 		                libvorbis/lib/codebook.c
# 		                libvorbis/lib/envelope.c
# 		                libvorbis/lib/floor0.c
# 		                libvorbis/lib/floor1.c
# 		                libvorbis/lib/info.c
# 		                libvorbis/lib/lookup.c
# 		                libvorbis/lib/lpc.c
# 		                libvorbis/lib/lsp.c
# 		                libvorbis/lib/mapping0.c
# 		                libvorbis/lib/mdct.c
# 		                libvorbis/lib/psy.c
# 		                libvorbis/lib/registry.c
# 		                libvorbis/lib/res0.c
# 		                libvorbis/lib/sharedbook.c
# 		                libvorbis/lib/smallft.c
# 		                libvorbis/lib/synthesis.c
# 		                libvorbis/lib/vorbisenc.c
# 		                libvorbis/lib/vorbisfile.c
# 		                libvorbis/lib/window.c)
#
#     add_library(CMP.Audio.VorbisOGG SHARED OggRead.cpp ${OGG_SOURCE} ${VORBIS_SOURCE})
#
#     SOURCE_GROUP("LibOGG"		FILES ${OGG_SOURCE})
#     SOURCE_GROUP("LibVorbis"	FILES ${VORBIS_SOURCE})
# ELSE()
    FIND_PATH(OPUS_INCLUDE_DIR
        NAMES opus.h
        HINTS
        /usr/include/opus
        /usr/local/include/opus)
    INCLUDE_DIRECTORIES(${OPUS_INCLUDE_DIR})

    FIND_PATH(OPUS_LIBRARY_DIR
        NAMES libopus.so
        HINTS
        ${LIB_3RD_FIND_HINT})
    LINK_DIRECTORIES(${OPUS_LIBRARY_DIR})

    add_library(CMP.Audio.Opus SHARED OpusRead.cpp)
    target_link_libraries(CMP.Audio.Opus opus opusfile)

    add_executable(OpusTest OpusTest.cpp)
    target_link_libraries(OpusTest opus opusfile)
# ENDIF()
