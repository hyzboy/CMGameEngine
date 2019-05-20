SET(BASE_GRAPH_OBJECT_SOURCE    Object/FlowControl.cpp
                                Object/RootFlowControl.cpp)
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

SET(CM_SCENE_GRAPH_SOURCE   ColorType.cpp
                            SkyLight.cpp
                            Camera.cpp
                            WalkerCamera.cpp
							SceneGlobalState.cpp
							SceneNode.cpp
							SceneOrient.cpp
							Bitmap.cpp
#                                 AABox.cpp
#                                 Frustum.cpp
#                                 Plane.cpp
#                                 Ray.cpp
                                )

SET(CM_SCENE_GRAPH_NORENDER_SOURCE VertexBufferNoRender.cpp ${CM_SCENE_GRAPH_SOURCE})

IF(WIN32)
    add_library(CM.SceneGraph STATIC ${CM_SCENE_GRAPH_NORENDER_SOURCE})
ELSE()
    add_library(CM.SceneGraph SHARED ${CM_SCENE_GRAPH_NORENDER_SOURCE})
ENDIF()

IF(BUILD_OPENGL_LIB)
    SET(CM_SCENE_GRAPH_RENDER_SOURCE
        ${BASE_GRAPH_OBJECT_SOURCE}
        ${GRAPHICS_APPLICATION_SOURCE}
    #     HeightMapTerrain.cpp
        VertexBufferType.cpp
        VertexArray.cpp
		InlineTexture.cpp
        InlineRenderable.cpp
        RenderList.cpp
        Material.cpp
        Renderable.cpp
        TileData.cpp
        Transform.cpp
    )

    IF(WIN32)
        add_library(CM.SceneGraphRender STATIC ${CM_SCENE_GRAPH_SOURCE} ${CM_SCENE_GRAPH_RENDER_SOURCE})
    ELSE()
        add_library(CM.SceneGraphRender SHARED ${CM_SCENE_GRAPH_SOURCE} ${CM_SCENE_GRAPH_RENDER_SOURCE})
    ENDIF()
ENDIF(BUILD_OPENGL_LIB)
