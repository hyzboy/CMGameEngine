#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/graph/InlineRenderable.h>	//CreateRenderableWireCube
#include<hgl/graph/SceneNode.h>			//SceneNode
#include<hgl/object/FlowObject.h>       //FlowObject

#include"CameraControl.h"

using namespace hgl;
using namespace hgl::graph;

const Vector3f  eye(100,100,80),
                center(0,0,0),
                up_vector(0,0,1),
                forward_vector(0,1,0);

#define NTB_NONE		0
#define NTB_NORMAL		1
#define NTB_FULL		2
#define NTB_NT_COMPRESS	3

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

	uint8 ntb;							///<0:无 1:只有normal 2:有完整NTB 3:有压缩NT

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
	Material *mtl;
	Renderable *r;

public:

	MeshData()
	{
		va=nullptr;
		mtl=nullptr;
		r=nullptr;
	}

	~MeshData()
	{
		SAFE_CLEAR(r);
		SAFE_CLEAR(mtl);
		SAFE_CLEAR(va);
	}
};

bool LoadMeshFile(MeshData &md,const OSString &filename)
{	
	MeshFileHeader mfh;

	io::FileInputStream fis;
	io::LEDataInputStream dis(&fis);

	if(!fis.Open(filename))
		RETURN_FALSE;

	if(dis.ReadFully(&mfh,sizeof(mfh))!=sizeof(mfh))
		RETURN_FALSE;

	md.va=new VertexArray(mfh.primitive_type);

	{
		VB3f *vertex=new VB3f(mfh.vertices_number);	
		dis.ReadFully(vertex->Begin(),vertex->GetBytes());vertex->End();
		md.va->SetVertex(vertex);
	}

	if(mfh.ntb!=NTB_NONE)
	{
		VB3f *normal=new VB3f(mfh.vertices_number);

		dis.ReadFully(normal->Begin(),normal->GetBytes());normal->End();

		md.va->SetNormal(normal);

		if(mfh.ntb==NTB_FULL)
			dis.Seek(normal->GetBytes()*2,io::soCurrent);		//跳过tangent和bitangent
	}

	dis.Seek(mfh.vertices_number*4*mfh.color_channels,io::soCurrent);		//跳过vertex color

	if(mfh.texcoord_channels)
	{
		uint8 *uv_comp=new uint8[mfh.texcoord_channels];

		dis.ReadUint8(uv_comp,mfh.texcoord_channels);

		uint32 uv_total=0;
		for(int i=0;i<mfh.texcoord_channels;i++)
			uv_total+=uv_comp[i];

		dis.Seek(uv_total*mfh.vertices_number*sizeof(float),io::soCurrent);						//跳过texcoord
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

	md.mtl=new Material;
	md.mtl->SetColorMaterial(true);
	md.mtl->SetColor(1,1,1,1);	
    md.mtl->SetLightMode(HGL_VERTEX_LIGHT);                          ///<使用顶点级别的光照

	md.mtl->SetUptAxis(HGL_AXIS_Y);

	md.r=new Renderable(md.va,md.mtl);
	md.r->AutoCreateShader(true,nullptr,filename);

	return(true);
}

class SceneLoader
{
	OSString main_filename;

	VertexArray *box_va;
	Material *box_mtl;
	Renderable *box_r;

	SceneNode *root;

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
		mesh_count=0;
		mesh_list=nullptr;

		root=new SceneNode;

		box_va=CreateRenderableWireCube();

		box_mtl=new Material;
		box_mtl->SetColorMaterial(true);
		box_mtl->SetColor(1,1,1,1);

		box_r=new Renderable(box_va,box_mtl);
		box_r->AutoCreateShader();
	}

	~SceneLoader()
	{
		delete[] mesh_list;

		delete box_r;
		delete box_mtl;
		delete box_va;
	}

private:
	
	void LoadMesh()
	{
		mesh_list=new MeshData[mesh_count];

		for(int i=0;i<mesh_count;i++)
			LoadMeshFile(mesh_list[i],main_filename+OS_TEXT(".")+OSString(i)+OS_TEXT(".mesh"));
	}

	void CreateBoundingBoxRenderable(SceneNode *node)
	{
		const AABB &bb=node->GetBoundingBox();

		Vector3f center((bb.MaxX()+bb.MinX())/2.0f,
						(bb.MaxY()+bb.MinY())/2.0f,
						(bb.MaxZ()+bb.MinZ())/2.0f);

		Vector3f size(	(bb.MaxX()-bb.MinX())/2.0f,
						(bb.MaxY()-bb.MinY())/2.0f,
						(bb.MaxZ()-bb.MinZ())/2.0f);

		node->AddSubNode(box_r,translate(center)*scale(size));
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

		//CreateBoundingBoxRenderable(node);

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

		if(!dis->ReadUint32(mesh_count))
			RETURN_FALSE;

		LoadMesh();

		bool result=LoadScene(root,dis);

		delete dis;
		dis=nullptr;
		
		root->RefreshMatrix();                                                    ///<刷新当前节点下所有子节点的矩阵
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

		sl.Load(OS_TEXT("T4112"));

		rl=sl.GetRenderList();

		const AABB &bb=sl.GetRootBouningBox();

		this->cam.eye.Set(bb.MaxX(),bb.MaxY(),bb.MaxZ());
		this->cam.center.Set((bb.MinX()+bb.MaxX())/2.0f,
							 (bb.MinY()+bb.MaxY())/2.0f,
							 (bb.MinZ()+bb.MaxZ())/2.0f);

		this->cam.znear=bb.MaxZ()*2;
		this->cam.zfar=bb.MinZ()*2;
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
