add_subdirectory(Noop)

IF(BUILD_RENDER_DEVICE_OPENGL_CORE_LIB)
    add_subdirectory(GLCore)
ENDIF()

IF(BUILD_RENDER_DEVICE_OPENGL_ES_LIB)
    add_subdirectory(GLCore)
ENDIF()

IF(BUILD_RENDER_DEVICE_VULKAN_LIB)
    add_subdirectory(Vulkan)
ENDIF()

IF(BUILD_RENDER_DEVICE_SOFTWARE_LIB)
    add_subdirectory(Software)
ENDIF()

