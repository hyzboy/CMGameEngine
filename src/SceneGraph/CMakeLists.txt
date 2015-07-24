SET(BASE_GRAPH_OBJECT_SOURCE 	Object/EventObject.cpp
								Object/FlowObject.cpp
								Object/FlowControl.cpp
								Object/MultiFlowControl.cpp
								Object/RadioObject.cpp
								Object/VisualObject.cpp
								Object/ObjectControl.cpp
								Object/ObjectControl.KeyFocus.cpp
								Object/ObjectControl.Makeup.cpp
								Object/ObjectControl.Proc.cpp
								Object/GUIObject.cpp)

SOURCE_GROUP("Object" FILES ${BASE_GRAPH_OBJECT_SOURCE})

SET(GRAPHICS_APPLICATION_SOURCE
		GraphicsApplication.cpp
		GraphicsApplicationEvent.cpp
		GraphicsSystemInitInfo.cpp)

SOURCE_GROUP("Application" FILES ${GRAPHICS_APPLICATION_SOURCE})

SET(CM_SCENE_GRAPH_ALL_SOURCE   AABox.cpp
                                Frustum.cpp
                                Plane.cpp
                                Ray.cpp
                                Camera.cpp
                                WalkerCamera.cpp)

IF(WIN32)
	add_library(CM.SceneGraph STATIC ${CM_SCENE_GRAPH_ALL_SOURCE})
ELSE()
	add_library(CM.SceneGraph SHARED ${CM_SCENE_GRAPH_ALL_SOURCE})
ENDIF()

IF(BUILD_OPENGL_LIB)
	SET(CM_SCENE_GRAPH_RENDER_ALL_SOURCE
		${BASE_GRAPH_OBJECT_SOURCE}
		${GRAPHICS_APPLICATION_SOURCE}
	# 	HeightMapTerrain.cpp
		Image.cpp
		InlineRenderable.cpp
		Material.cpp
		Renderable.cpp
		RenderList.cpp
		SceneGlobalState.cpp
		SceneNode.cpp
		SceneOrient.cpp
		Shader.cpp
		ShaderDataBlock.cpp
		SkyLight.cpp
		Texture.cpp
		TileData.cpp
		Transform.cpp
	)

IF(WIN32)
	add_library(CM.SceneGraphRender STATIC ${CM_SCENE_GRAPH_RENDER_ALL_SOURCE})
ELSE()
	add_library(CM.SceneGraphRender SHARED ${CM_SCENE_GRAPH_RENDER_ALL_SOURCE})
ENDIF()
ENDIF(BUILD_OPENGL_LIB)
