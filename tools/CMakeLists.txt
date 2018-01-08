#add_subdirectory(StreamCodeMaker)

add_subdirectory(csv2table)

# add_subdirectory(TextExtract)
# add_subdirectory(TextExchange)

add_subdirectory(TextEncodingConvert)
add_subdirectory(TextEncodingConvertQt)

IF(BUILD_GUI_TOOLS)
	add_subdirectory(CDBTableViewer)
ENDIF()

IF(BUILD_OPENGL_LIB)
    add_subdirectory(TexConv)
    add_subdirectory(ModelConvert)
ENDIF()

IF(BUILD_OpenCart)
    add_subdirectory(OpenCart)
ENDIF()
