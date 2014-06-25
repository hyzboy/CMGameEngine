add_library(SceneGraph SHARED

# 	AABox.cpp
# 	Frustum.cpp
# 	Plane.cpp
# 	Ray.cpp
	Camera.cpp
	WalkerCamera.cpp
)

IF(BUILD_OPENGL_LIB)
	add_library(SceneGraphRender SHARED
		GUIObject.cpp
		ObjectControl.cpp
		ObjectControl.KeyFocus.cpp
		ObjectControl.Makeup.cpp
		ObjectControl.Proc.cpp
		GraphicsApplication.cpp
		GraphicsApplicationEvent.cpp
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
		VertexBuffer.cpp
	)
ENDIF(BUILD_OPENGL_LIB)
