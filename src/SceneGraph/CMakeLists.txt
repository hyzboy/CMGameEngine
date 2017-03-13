SET(BASE_GRAPH_OBJECT_SOURCE     Object/FlowControl.cpp)
#                                 Object/MultiFlowControl.cpp
#                                 Object/RadioObject.cpp
#                                 Object/VisualObject.cpp
#                                 Object/ObjectControl.cpp
#                                 Object/ObjectControl.KeyFocus.cpp
#                                 Object/ObjectControl.Makeup.cpp
#                                 Object/ObjectControl.Proc.cpp
#                                 Object/GUIObject.cpp)

SOURCE_GROUP("Object" FILES ${BASE_GRAPH_OBJECT_SOURCE})

SET(GRAPHICS_APPLICATION_SOURCE
        Application/GraphicsApplication.cpp
        Application/GraphicsSystemInitInfo.cpp)

SOURCE_GROUP("Application" FILES ${GRAPHICS_APPLICATION_SOURCE})

SET(CM_SCENE_GRAPH_SOURCE    Camera.cpp
                                WalkerCamera.cpp
#                                 AABox.cpp
#                                 Frustum.cpp
#                                 Plane.cpp
#                                 Ray.cpp
                                )

IF(WIN32)
    add_library(CM.SceneGraph STATIC ${CM_SCENE_GRAPH_SOURCE})
ELSE()
    add_library(CM.SceneGraph SHARED ${CM_SCENE_GRAPH_SOURCE})
ENDIF()

IF(BUILD_OPENGL_LIB)
    SET(CM_SCENE_GRAPH_RENDER_SOURCE
        ${BASE_GRAPH_OBJECT_SOURCE}
        ${GRAPHICS_APPLICATION_SOURCE}
    #     HeightMapTerrain.cpp
        VertexBufferType.cpp
        VertexArray.cpp
        Bitmap.cpp
        InlineRenderable.cpp
        Material.cpp
        Renderable.cpp
        RenderList.cpp
        SceneGlobalState.cpp
        SceneNode.cpp
        SceneOrient.cpp
        SkyLight.cpp
        TileData.cpp
        Transform.cpp
    )

    IF(WIN32)
        add_library(CM.SceneGraphRender STATIC ${CM_SCENE_GRAPH_SOURCE} ${CM_SCENE_GRAPH_RENDER_SOURCE})
    ELSE()
        add_library(CM.SceneGraphRender SHARED ${CM_SCENE_GRAPH_SOURCE} ${CM_SCENE_GRAPH_RENDER_SOURCE})
    ENDIF()
ENDIF(BUILD_OPENGL_LIB)
