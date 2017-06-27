#ifndef HGL_TOOL_MODEL_CONVERT_ASSIMP_LOADER_INCLUDE
#define HGL_TOOL_MODEL_CONVERT_ASSIMP_LOADER_INCLUDE

#include<hgl/algorithm/VectorMath.h>
#include<hgl/type/List.h>
#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/SceneNode.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>

using namespace hgl;
using namespace hgl::graph;

class AssimpLoader
{
	OSString main_filename;

	uint total_file_bytes;

	const aiScene *scene;
	
public:

	aiVector3D scene_min, scene_max, scene_center;

private:

	UTF8StringList tex_list;

private:
	
	void get_bounding_box_for_node(const aiNode *,aiVector3D *,aiVector3D *,aiMatrix4x4 *);
	void get_bounding_box(aiVector3D *,aiVector3D *);

	void LoadMaterial();
	void LoadMesh();
	void LoadScene(const OSString &,SceneNode *,const aiScene *,const aiNode *);

	void SaveFile(const void *,const uint &,const OSString &);
	void SaveFile(void **,const int64 *,const int &,const OSString &);

	void SaveTextures();

	template<typename T>
	void SaveFaces(const aiFace *,const T,const OSString &extname);

	void SaveTexCoord(float *,const aiVector3D *,const uint,const uint);

public:

	AssimpLoader();
	~AssimpLoader();

	bool LoadFile(const OSString &);
	bool SaveFile(const OSString &);
};//class AssimpLoader
#endif//HGL_TOOL_MODEL_CONVERT_ASSIMP_LOADER_INCLUDE