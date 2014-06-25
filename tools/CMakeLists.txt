

add_subdirectory(StreamCodeMaker)

add_subdirectory(csv2table)

IF(BUILD_GUI_TOOLS)
	add_subdirectory(CDBTableViewer)
ENDIF()
