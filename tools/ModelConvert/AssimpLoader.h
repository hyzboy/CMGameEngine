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

#define NTB_NONE		0
#define NTB_NORMAL		1
#define NTB_FULL		2
#define NTB_NT_COMPRESS	3

struct MeshStruct
{
	uint8 flag[4];						///<'MESH'
	uint8 sperator;						///<0x1a
	uint8 version;						///<1

	uint8 primitive_type;				///<图元类型

	uint32 vertices_number;				///<顶点数量
	uint32 faces_number;				///<面数量

	uint8 color_channels;				///<顶点色数量
	uint8 texcoord_channels;			///<纹理坐标数量

	uint32 material_index;				///<材质索引

	uint8 ntb;							///<0:无 1:只有normal 2:有完整NTB 3:有压缩NT

	uint32 bones_number;				///<骨骼数量

public:

	MeshStruct()
	{
		memset(this,0,sizeof(MeshStruct));
		flag[0]='M';
		flag[1]='E';
		flag[2]='S';
		flag[3]='H';
		sperator=0x1A;
		version=1;
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
	void get_bounding_box(const aiNode *,aiVector3D *,aiVector3D *);

	void LoadMaterial();
	void LoadMesh();
	void LoadScene(const UTF8String &,io::DataOutputStream *,const aiNode *);

	void SaveFile(const void *,const uint &,const OSString &);
	void SaveFile(void **,const int64 *,const int &,const OSString &);

	void SaveTextures();

	template<typename T>
	void SaveFaces(io::FileOutputStream *,const aiFace *,const T);

	void SaveTexCoord(float *,const aiVector3D *,const uint,const uint);

public:

	AssimpLoader();
	~AssimpLoader();

	bool LoadFile(const OSString &);
	bool SaveFile(const OSString &);
};//class AssimpLoader
#endif//HGL_TOOL_MODEL_CONVERT_ASSIMP_LOADER_INCLUDE
