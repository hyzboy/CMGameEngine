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

#pragma pack(push,1)

struct MaterialTextureStruct
{
	uint8 type=0;

	int32 tex_id=-1;

	uint8 tm=0;
	uint32 old_uvindex=0;
	uint32 new_uvindex=0;
	float blend=0;
	uint32 op=0;
	uint32 wrap_mode[2]={0,0};
};//

struct MaterialStruct
{
	uint32 tex_count;

	MaterialTextureStruct *tex_list;

	Set<int> uv_use;

	Color4f diffuse;
	Color4f specular;
	Color4f ambient;
	Color4f emission;

	float shininess=0;

	bool wireframe=false;
	bool two_sided=false;

public:

	MaterialStruct()
	{
		tex_count=0;
		tex_list=nullptr;
	}
	
	void Init(const uint32 tc)
	{
		tex_count=tc;

		tex_list=new MaterialTextureStruct[tc];
	}

	~MaterialStruct()
	{
		delete[] tex_list;
	}

	void ProcUVIndex()
	{
		for(int i=0;i<tex_count;i++)
			tex_list[i].new_uvindex=i;
	}
};
#pragma pack(pop)

class AssimpLoader
{
	OSString main_filename;

	uint total_file_bytes;

	const aiScene *scene;
	
public:

	aiVector3D scene_min, scene_max, scene_center;

private:

	UTF8StringList tex_list;

	int material_count;
	MaterialStruct *material_list;

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