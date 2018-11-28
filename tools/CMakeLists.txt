#add_subdirectory(StreamCodeMaker)

if(DATABASE_INCLUDE_CDB)
    add_subdirectory(csv2table)
endif()

# add_subdirectory(TextExtract)
# add_subdirectory(TextExchange)

add_subdirectory(TextEncodingConvert)

IF(BUILD_GUI_TOOLS)
    add_subdirectory(TextEncodingConvertQt)

    IF(DATABASE_INCLUDE_CDB)
        add_subdirectory(CDBTableViewer)
	endif()
ENDIF()

IF(BUILD_OPENGL_LIB)
    add_subdirectory(TexConv)
    add_subdirectory(ModelConvert)
ENDIF()

IF(BUILD_OpenCart)
    add_subdirectory(OpenCart)
ENDIF()

add_subdirectory(NewsSpider)
