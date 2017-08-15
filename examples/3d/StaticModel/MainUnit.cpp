#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/InlineRenderable.h>	//CreateRenderableWireCube
#include<hgl/graph/SceneNode.h>			//SceneNode
#include<hgl/object/FlowObject.h>       //FlowObject

#include"CameraControl.h"

using namespace hgl;
using namespace hgl::graph;

struct MaterialTextureData
{
	uint8 type=0;

	int32 tex_id=-1;

	uint8 uvindex=0;
	float blend=0;
	uint8 op=0;
	uint16 wrap_mode[2]={0,0};
};//

struct MaterialData
{
	uint8 tex_count;

	MaterialTextureData *tex_list;

	Set<int> uv_use;

	Color4f diffuse;
	Color4f specular;
	Color4f ambient;
	Color4f emission;

	float shininess=0;

	bool wireframe=false;
	bool two_sided=false;

public:

	MaterialData()
	{
		tex_count=0;
		tex_list=nullptr;
	}
	
	void Init(const uint32 tc)
	{
		tex_count=tc;

		tex_list=new MaterialTextureData[tc];
	}

	~MaterialData()
	{
		delete[] tex_list;
	}
};

void LoadMaterialFile(Material &mtl,MaterialData &md,const OSString &filename,Texture **tex_list)
{
	io::FileInputStream fis;
	io::LEDataInputStream dis(&fis);

	if(!fis.Open(filename))
		return;

	uint8 flag[9];

	if(dis.ReadFully(flag,9)!=9)
		return;

	if(memcmp(flag,"Material\x1A",9))
		return;

	uint8 ver;

	if(!dis.ReadUint8(ver))return;

	if(ver!=1)return;

	if(dis.ReadFloat(md.diffuse,3)!=3)return;
	if(dis.ReadFloat(md.specular,3)!=3)return;
	if(dis.ReadFloat(md.ambient,3)!=3)return;
	if(dis.ReadFloat(md.emission,3)!=3)return;

	if(!dis.ReadFloat(md.shininess))return;
	if(!dis.ReadBool(md.wireframe))return;
	if(!dis.ReadBool(md.two_sided))return;

	mtl.SetTwoSide(md.two_sided);

    mtl.SetLightMode(HGL_PIXEL_LIGHT);

	if(!dis.ReadUint8(md.tex_count))return;

	md.Init(md.tex_count);

	for(uint8 i=0;i<md.tex_count;i++)
	{
		MaterialTextureData *mtd=&(md.tex_list[i]);

		if(!dis.ReadUint8(mtd->type))return;
		if(!dis.ReadInt32(mtd->tex_id))return;
		if(!dis.ReadUint8(mtd->uvindex))return;
		if(!dis.ReadFloat(mtd->blend))return;
		if(!dis.ReadUint8(mtd->op))return;
		if(!dis.ReadUint16(mtd->wrap_mode,2))return;

		mtl.SetTexture(mtd->type,tex_list[mtd->tex_id]);
	}	
}

#define NTB_BIT_NORMAL					(1<<0)
#define NTB_BIT_TANGENT					(1<<1)
#define NTB_BIT_BINORMAL				(1<<2)

#define NTB_BIT_ALL						(NTB_BIT_NORMAL|NTB_BIT_TANGENT|NTB_BIT_BINORMAL)

#define NTB_BIT_COMPRESS_NORMAL			(1<<3)
#define NTB_BIT_COMPRESS_TANGENT		(1<<4)
#define NTB_BIT_COMPRESS_BINORMAL		(1<<5)

#define NTB_BIT_COMPRESS_ALL			(NTB_BIT_COMPRESS_NORMAL|NTB_BIT_COMPRESS_TANGENT|NTB_BIT_COMPRESS_BINORMAL)

#define NTB_BIT_COMPRESS_NORMAL_TANGENT	(1<<6)

#pragma pack(push,1)
struct MeshFileHeader
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

	uint8 ntb;							///<NTB信息位合集

	uint32 bones_number;				///<骨骼数量

public:

	MeshFileHeader()
	{
		memset(this,0,sizeof(MeshFileHeader));
	}
};//struct MeshFileHeader

#pragma pack(pop)
struct MeshData
{
	VertexArray *va;
	Renderable *r;

public:

	MeshData()
	{
		va=nullptr;
		r=nullptr;
	}

	~MeshData()
	{
		SAFE_CLEAR(r);
		SAFE_CLEAR(va);
	}
};

bool LoadMeshFile(MeshData &md,const OSString &filename,Material *mtl_list,MaterialData *md_list)
{	
	MeshFileHeader mfh;

	io::FileInputStream fis;
	io::LEDataInputStream dis(&fis);

	if(!fis.Open(filename))
		RETURN_FALSE;

	if(dis.ReadFully(&mfh,sizeof(mfh))!=sizeof(mfh))
		RETURN_FALSE;

	Material *mtl=&(mtl_list[mfh.material_index]);
	MaterialData *mtd=&(md_list[mfh.material_index]);

	uint8 *uv_comp;
	float **uv_data;
	VertexBufferBase **uv_vb;

	md.va=new VertexArray(mfh.primitive_type);

	{
		VB3f *vertex=new VB3f(mfh.vertices_number);	
		dis.ReadFully(vertex->Begin(),vertex->GetBytes());vertex->End();
		md.va->SetVertex(vertex);
	}

	if(mfh.ntb!=NTB_BIT_NORMAL)
	{
		VB3f *normal=new VB3f(mfh.vertices_number);

		dis.ReadFully(normal->Begin(),normal->GetBytes());normal->End();

		md.va->SetNormal(normal);

		if(mfh.ntb==NTB_BIT_ALL)
			dis.Seek(normal->GetBytes()*2,io::soCurrent);					//跳过tangent和bitangent
	}

	dis.Seek(mfh.vertices_number*4*mfh.color_channels,io::soCurrent);		//跳过vertex color

	if(mfh.texcoord_channels)
	{
		uv_comp=new uint8[mfh.texcoord_channels];
		uv_data=new float *[mfh.texcoord_channels];
		uv_vb=new VertexBufferBase *[mfh.texcoord_channels];

		dis.ReadUint8(uv_comp,mfh.texcoord_channels);

		uint32 uv_total=0;
		for(int i=0;i<mfh.texcoord_channels;i++)
		{
			uv_total+=uv_comp[i];
			uv_data[i]=new float[uv_comp[i]*mfh.vertices_number*sizeof(float)];
		}

		for(int i=0;i<mfh.texcoord_channels;i++)
		{
			dis.ReadFloat(uv_data[i],uv_comp[i]*mfh.vertices_number);

			if(uv_comp[i]==1)
				uv_vb[i]=new VB1f(mfh.vertices_number,uv_data[i]);
			else
			if(uv_comp[i]==2)
				uv_vb[i]=new VB2f(mfh.vertices_number,uv_data[i]);
			else
				uv_vb[i]=nullptr;
		}
	}

	if(mfh.vertices_number>0xFFFF)
	{
		VB4u32 *face=new VB4u32(mfh.faces_number*3);

		dis.ReadFully(face->Begin(),face->GetBytes());face->End();

		md.va->SetIndex(face);
	}
	else
	{
		VB4u16 *face=new VB4u16(mfh.faces_number*3);
		
		dis.ReadFully(face->Begin(),face->GetBytes());face->End();

		md.va->SetIndex(face);
	}

	fis.Close();

	for(int i=0;i<mtd->tex_count;i++)
	{
		MaterialTextureData *mt=&(mtd->tex_list[i]);

		md.va->SetVertexBuffer(VertexBufferType(mt->type-mtcDiffuse+vbtDiffuseTexCoord),uv_vb[mt->uvindex]);
	}

	md.r=new Renderable(md.va,mtl);

	for(int i=0;i<mtd->tex_count;i++)
	{
		MaterialTextureData *mt=&(mtd->tex_list[i]);

		md.r->SetTexCoord(mt->type,VertexBufferType(mt->type-mtcDiffuse+vbtDiffuseTexCoord));
	}

	SAFE_CLEAR_OBJECT_ARRAY(uv_data,mfh.texcoord_channels);
	//SAFE_CLEAR_OBJECT_ARRAY(uv_vb,mfh.texcoord_channels);
	SAFE_CLEAR_ARRAY(uv_comp);

	md.r->AutoCreateShader(true,nullptr,filename);

	return(true);
}

class SceneLoader
{
	OSString main_filename;

	SceneNode *root;

	uint32 tex_count;
	Texture **tex_list;

	uint32 mtl_count;
	MaterialData *md_list;
	Material *mtl_list;

	uint32 mesh_count;
	MeshData *mesh_list;

	RenderList rl;

public:

	const AABB &GetRootBouningBox()
	{
		return root->GetBoundingBox();
	}

	RenderList *GetRenderList(){return &rl;}

public:

	SceneLoader()
	{
		tex_count=0;
		tex_list=nullptr;

		mtl_count=0;
		md_list=nullptr;
		mtl_list=nullptr;

		mesh_count=0;
		mesh_list=nullptr;

		root=new SceneNode;
	}

	~SceneLoader()
	{
		delete root;
		delete[] mesh_list;

		SAFE_CLEAR_ARRAY(md_list);
		SAFE_CLEAR_ARRAY(mtl_list);
		SAFE_CLEAR_OBJECT_ARRAY(tex_list,tex_count);
	}

private:

	void LoadTexture()
	{
		UTF8StringList fn_list;

		{
			io::FileInputStream fis;

			if(!fis.Open(main_filename+OS_TEXT(".tex_list")))
				return;

			io::LEDataInputStream dis(&fis);

			LoadUTF8StringList(fn_list,&dis);

			fis.Close();
		}

		tex_count=fn_list.GetCount();

		tex_list=new Texture *[tex_count];

		OSString fn;

		for(int i=0;i<tex_count;i++)
		{
		#if HGL_OS == HGL_OS_Windows
			fn=to_u16(fn_list.GetString(i));
		#else
			fn=fn_list.GetString(i);
		#endif//

			tex_list[i]=CreateTexture2D(fn+OS_TEXT(".tex2d"));
		}
	}

	void LoadMaterial()
	{
		mtl_list=new Material[mtl_count];
		md_list=new MaterialData[mtl_count];

		for(int i=0;i<mtl_count;i++)
			LoadMaterialFile(mtl_list[i],md_list[i],main_filename+OS_TEXT(".")+OSString(i)+OS_TEXT(".material"),tex_list);
	}
	
	void LoadMesh()
	{
		mesh_list=new MeshData[mesh_count];

		for(int i=0;i<mesh_count;i++)
			LoadMeshFile(mesh_list[i],main_filename+OS_TEXT(".")+OSString(i)+OS_TEXT(".mesh"),mtl_list,md_list);
	}

	void AddMesh(SceneNode *node,const uint32 *mesh_index,const uint32 count)
	{
		for(int i=0;i<count;i++)
			node->Add(mesh_list[mesh_index[i]].r);
	}

	bool LoadScene(SceneNode *node,io::DataInputStream *dis)
	{
		UTF8String node_name;

		if(!dis->ReadUTF8String(node_name))
			RETURN_FALSE;

		//bounding box
		{
			float box[6];

			if(dis->ReadFloat(box,6)!=6)
				RETURN_FALSE;

			vec bb_min(box[0],box[1],box[2],1);
			vec bb_max(box[3],box[4],box[5],1);

			AABB bb(bb_min,bb_max);

			node->SetBoundingBox(bb);
		}

		//matrix
		{
			Matrix4f mat;

			if(dis->ReadFloat((float *)&mat,16)!=16)
				RETURN_FALSE;

			node->SetLocalMatrix(mat);
		}

		//mesh
		{
			uint32 count;

			if(!dis->ReadUint32(count))
				RETURN_FALSE;

			uint32 *index=new uint32[count];

			if(dis->ReadUint32(index,count)!=count)
			{
				delete[] index;
				RETURN_FALSE;
			}
			
			AddMesh(node,index,count);

			delete[] index;
		}//mesh

		//children
		{
			uint32 children_count;

			if(!dis->ReadUint32(children_count))
				RETURN_FALSE;

			for(uint32 i=0;i<children_count;i++)
			{
				SceneNode *sub_node=new SceneNode;

				if(!LoadScene(sub_node,dis))
				{
					delete sub_node;
					RETURN_FALSE;
				}

				node->AddSubNode(sub_node);
			}
		}//children

		return(true);
	}

public:

	bool Load(const OSString &filename)
	{
		io::FileInputStream fis;

		main_filename=filename;

		LoadTexture();

		if(!fis.Open(filename+OS_TEXT(".scene")))
			RETURN_FALSE;

		io::LEDataInputStream *dis=new io::LEDataInputStream(&fis);

		uint8 flag[6],ver;

		if(dis->ReadFully(flag,6)!=6)
			RETURN_FALSE;

		if(memcmp(flag,"SCENE\x1A",6))
			RETURN_FALSE;

		if(!dis->ReadUint8(ver))
			RETURN_FALSE;

		if(ver!=1)
			RETURN_FALSE;

		if(!dis->ReadUint32(mtl_count))
			RETURN_FALSE;

		LoadMaterial();

		if(!dis->ReadUint32(mesh_count))
			RETURN_FALSE;

		LoadMesh();

		bool result=LoadScene(root,dis);

		delete dis;
		dis=nullptr;
		
		root->RefreshMatrix();                                              ///<刷新当前节点下所有子节点的矩阵
		root->ExpendToList(&rl);                                            ///<展开当前节点成一个渲染列表
		return(result);
	}
};

class TestObject:public CameraControlFlowObject
{
	SceneLoader sl;
	RenderList *rl;

public:

	TestObject()
	{
        SetClearColor(ceMozillaCharcoal);		//设置清屏颜色

		sl.Load(OS_TEXT("jill_mesh"));

		rl=sl.GetRenderList();

		const AABB &bb=sl.GetRootBouningBox();

		this->cam.eye.Set(bb.MaxX(),bb.MaxY(),bb.MaxZ());
		this->cam.center.Set((bb.MinX()+bb.MaxX())/2.0f,
							 (bb.MinY()+bb.MaxY())/2.0f,
							 (bb.MinZ()+bb.MaxZ())/2.0f);

		this->cam.znear=1;
		this->cam.zfar=-1;
	}

	~TestObject()
	{
	}

	void Draw(const Matrix4f *)
	{
        ClearColorDepthBuffer();

		Render(rl,&proj,&mv);
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("静态模型显示","StaticModel",new TestObject());
