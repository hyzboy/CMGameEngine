#add_subdirectory(StreamCodeMaker)

add_subdirectory(csv2table)

# add_subdirectory(TextExtract)
# add_subdirectory(TextExchange)

IF(BUILD_GUI_TOOLS)
	add_subdirectory(CDBTableViewer)
ENDIF()

IF(BUILD_OPENGL_LIB)
    add_subdirectory(TexConv)
ENDIF()

IF(BUILD_OpenCart)
    add_subdirectory(OpenCart)
ENDIF()
