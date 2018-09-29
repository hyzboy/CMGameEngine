#include"AssimpLoader.h"
#include<assimp/postprocess.h>
#include<assimp/cimport.h>
#include<hgl/io/FileSystem.h>
#include<hgl/type/List.h>
#include<hgl/io/MemoryOutputStream.h>

#define LOG_BR	LOG_INFO(OS_TEXT("------------------------------------------------------------"));

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void AssimpLoader::get_bounding_box_for_node (const aiNode* nd, 
												aiVector3D* min, 
												aiVector3D* max, 
												aiMatrix4x4* trafo)
{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) 
	{
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		for (t = 0; t < mesh->mNumVertices; ++t) 
		{
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n)
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);

	*trafo = prev;
}

void AssimpLoader::get_bounding_box (const aiNode *node,aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;

	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;

	get_bounding_box_for_node(node,min,max,&trafo);
}

AssimpLoader::AssimpLoader()
{
	scene=0;
	material_list=nullptr;
}

AssimpLoader::~AssimpLoader()
{
	delete[] material_list;

	if(scene)
		aiReleaseImport(scene);
}

void AssimpLoader::SaveFile(const void *data,const uint &size,const OSString &ext_name)
{
	if(!data||size<=0)return;

	OSString filename=main_filename+OS_TEXT(".")+ext_name;

	if(filesystem::SaveMemoryToFile(filename,data,size)==size)
	{
		LOG_INFO(OS_TEXT("SaveFile: ")+filename+OS_TEXT(" , size: ")+OSString(size));

		total_file_bytes+=size;
	}
}

void AssimpLoader::SaveFile(void **data,const int64 *size,const int &count,const OSString &ext_name)
{
	if(!data||size<=0)return;

	OSString filename=main_filename+OS_TEXT(".")+ext_name;

	int64 result=filesystem::SaveMemoryToFile(filename,data,size,count);

	if(result>0)
	{
		LOG_INFO(OS_TEXT("SaveFile: ")+filename+OS_TEXT(" , size: ")+OSString(result));

		total_file_bytes+=result;
	}
}

void OutFloat3(const OSString &front,const Color4f &c)
{
	LOG_INFO(front+OS_TEXT(": ")+OSString(c.r)
					+OS_TEXT(",")+OSString(c.g)
					+OS_TEXT(",")+OSString(c.b));
}

constexpr uint8 hgl_mtc[]={mtcNone,mtcDiffuse,mtcSpecular,mtcAmbient,mtcEmissive,mtcHeight,mtcNormal,mtcShininess,mtcAlpha,mtcDisplacement,mtcLight,mtcReflection};
constexpr uint16 gl_wrap[4]={HGL_WRAP_REPEAT,HGL_WRAP_CLAMP,HGL_WRAP_MIRRORED_REPEAT,0};

void OutMaterialTexture(const MaterialTextureStruct &mt,io::DataOutputStream *dos)
{
	dos->WriteUint8(hgl_mtc[mt.type]);
	dos->WriteInt32(mt.tex_id);
	dos->WriteUint8(mt.new_uvindex);
	dos->WriteFloat(mt.blend);
	dos->WriteUint8(mt.op);
	dos->WriteUint16(mt.wrap_mode,2);

	LOG_INFO(OS_TEXT("\tTexture Type: ")+OSString(mt.type));
	LOG_INFO(OS_TEXT("\tTexture ID: ")+OSString(mt.tex_id));

	LOG_INFO(OS_TEXT("\tuvindex: ")+OSString(mt.old_uvindex));
	LOG_INFO(OS_TEXT("\tblend: ")+OSString(mt.blend));
	LOG_INFO(OS_TEXT("\top: ")+OSString(mt.op));
	LOG_INFO(OS_TEXT("\twrap_mode: ")+OSString(mt.wrap_mode[0])+OS_TEXT(",")+OSString(mt.wrap_mode[1]));
}

void OutMaterial(const MaterialStruct *ms,const OSString &filename)
{
	io::FileOutputStream fos;
	io::LEDataOutputStream dos(&fos);

	fos.CreateTrunc(filename);

	dos.WriteFully("Material\x1A\x1",10);
	dos.WriteFloat(ms->diffuse,3);
	dos.WriteFloat(ms->specular,3);
	dos.WriteFloat(ms->ambient,3);
	dos.WriteFloat(ms->emission,3);
	dos.WriteFloat(ms->shininess);
	dos.WriteBool(ms->wireframe);
	dos.WriteBool(ms->two_sided);
	dos.WriteUint8(ms->tex_count);

	OutFloat3(OSString(OS_TEXT("diffuse")),ms->diffuse);
	OutFloat3(OSString(OS_TEXT("specular")),ms->specular);
	OutFloat3(OSString(OS_TEXT("ambient")),ms->ambient);
	OutFloat3(OSString(OS_TEXT("emission")),ms->emission);
	
	LOG_INFO(OS_TEXT("shininess: ")+OSString(ms->shininess));

	LOG_INFO(OS_TEXT("wireframe: ")+OSString(ms->wireframe?OS_TEXT("true"):OS_TEXT("false")));
	LOG_INFO(OS_TEXT("two_sided: ")+OSString(ms->two_sided?OS_TEXT("true"):OS_TEXT("false")));

	LOG_INFO(OS_TEXT("Material Texture Count ")+OSString(ms->tex_count));

	for(int i=0;i<ms->tex_count;i++)
		OutMaterialTexture(ms->tex_list[i],&dos);

	fos.Close();
}

void AssimpLoader::LoadMaterial()
{
	if(!scene->HasMaterials())return;

	int ret1, ret2;
	float c[4];
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess;
	float strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	LOG_BR;
	LOG_INFO(OS_TEXT("Material Count ")+OSString(scene->mNumMaterials));
	LOG_BR;
	
	aiString filename;
	aiTextureMapping tm;
	uint uvindex=0;
	float blend;
	aiTextureOp op;
	aiTextureMapMode wrap_mode[2];

	material_count=scene->mNumMaterials;

	material_list=new MaterialStruct[material_count];

	for(unsigned int m=0;m<scene->mNumMaterials;m++)
	{
		int tex_index=0;
		int tex_count=0;
		int tt_count=0;
		int32 tex_id;
		aiReturn found=AI_SUCCESS;

		aiMaterial *mtl=scene->mMaterials[m];

		for(uint tt=aiTextureType_DIFFUSE;tt<aiTextureType_UNKNOWN;tt++)
		{
			tex_count+=mtl->GetTextureCount((aiTextureType)tt);
		}

		MaterialStruct *ms=&(material_list[m]);
		
		ms->Init(tex_count);

		for(uint tt=aiTextureType_DIFFUSE;tt<aiTextureType_UNKNOWN;tt++)
		{
			tt_count=mtl->GetTextureCount((aiTextureType)tt);

			for(int t=0;t<tt_count;t++)
			{
				mtl->GetTexture((aiTextureType)tt,t,&filename,&tm,&uvindex,&blend,&op,wrap_mode);

				ms->tex_list[tex_index].type=tt;

				ms->uv_use.Add(uvindex);
		
				{
					char *fn=strrchr(filename.data,'\\');
					char *ext;

					if(!fn)
						fn=strrchr(filename.data,'/');
					else
						++fn;

					if(fn)
						++fn;
					else
						fn=filename.data;

					ext=strrchr(fn,'.');

					if(ext)*ext=0;

				#if HGL_OS == HGL_OS_Window
					tex_id=tex_list.CaseFind(fn);
				#else
					tex_id=tex_list.Find(fn);
				#endif//

					if(tex_id==-1)
						tex_id=tex_list.Add(fn);

					ms->tex_list[tex_index].tex_id=tex_id;
				}

				ms->tex_list[tex_index].old_uvindex=uvindex;
				ms->tex_list[tex_index].blend=blend;
				ms->tex_list[tex_index].op=op;
				
				ms->tex_list[tex_index].wrap_mode[0]=gl_wrap[wrap_mode[0]];
				ms->tex_list[tex_index].wrap_mode[1]=gl_wrap[wrap_mode[1]];

				++tex_index;
			}
		}

		ms->ProcUVIndex();

		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
		ms->diffuse.Set(c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
		ms->specular.Set(c);

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
		ms->ambient.Set(c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
		ms->emission.Set(c);

		max = 1;
		ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

		if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		{
			//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);

			ms->shininess=shininess*strength;
		}
		else 
		{
			//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
			ms->shininess=0;

			set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
			//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
			
			ms->specular.Zero();
		}

		max = 1;
		if(aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max)==AI_SUCCESS)
			ms->wireframe=wireframe;
		else
			ms->wireframe=false;

		max = 1;
		if(aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)==AI_SUCCESS)
			ms->two_sided=two_sided;
		else
			ms->two_sided=true;

		OutMaterial(ms,main_filename+OS_TEXT(".")+OSString(m)+OS_TEXT(".material"));
		LOG_BR;
	}
}

void AssimpLoader::SaveTextures()
{	
	const int tex_count=tex_list.GetCount();

	LOG_INFO(OS_TEXT("TOTAL Texture Count: ")+OSString(tex_count));

	for(int i=0;i<tex_count;i++)
		LOG_INFO(U8_TEXT("\t")+UTF8String(i)+U8_TEXT("\t")+tex_list[i]);
		
	LOG_BR;

	io::MemoryOutputStream mos;
	io::LEDataOutputStream dos(&mos);

	SaveUTF8StringList(&dos,tex_list);

	SaveFile(mos.GetData(),mos.Tell(),OS_TEXT("tex_list"));
}

template<typename T>
void AssimpLoader::SaveFaces(io::FileOutputStream *fos,const aiFace *face,const T count)
{
	T *data=new T[count*3];

	const aiFace *sp=face;
	T *tp=data;

	for(T i=0;i<count;i++)
	{
		*tp=sp->mIndices[0];++tp;
		*tp=sp->mIndices[1];++tp;
		*tp=sp->mIndices[2];++tp;

		++sp;
	}

	fos->WriteFully(data,sizeof(T)*3*count);

	delete[] data;
}

void AssimpLoader::SaveTexCoord(float *tp,const aiVector3D *texcoord,const uint comp,const uint count)
{
	if(comp<=0||comp>3)
		return;

	const uint size=1+sizeof(float)*comp*count;

	const aiVector3D *sp=texcoord;

	if(comp==1)
	{
		for(uint i=0;i<count;i++)
		{
			*tp=sp->x;++tp;
			++sp;
		}
	}
	else
	if(comp==2)
	{
		for(uint i=0;i<count;i++)
		{
			*tp=sp->x;++tp;
			*tp=sp->y;++tp;
			++sp;
		}
	}
	else
	if(comp==3)
	{
		for(uint i=0;i<count;i++)
		{
			*tp=sp->x;++tp;
			*tp=sp->y;++tp;
			*tp=sp->z;++tp;
			++sp;
		}
	}
}

void c4f_to_4b(uint8 *tp,const aiColor4D *c4,const int count)
{
	for(int i=0;i<count;i++)
	{
		*tp=c4->r*255;++tp;
		*tp=c4->g*255;++tp;
		*tp=c4->b*255;++tp;
		*tp=c4->a*255;++tp;
	}
}

void AxisY2Z(aiVector3D *v,const uint32 count)
{
	ai_real t;

	for(uint32 i=0;i<count;i++)
	{
		t=v->y;
		v->y=-(v->z);
		v->z=t;

		++v;
	}
}

void AssimpLoader::LoadMesh()
{
	if(!scene->HasMeshes())return;

	UTF8String mesh_name;

	total_file_bytes=0;

	LOG_BR;
	LOG_INFO(OS_TEXT("Mesh Count ")+OSString(scene->mNumMeshes));

	uint vertex_total=0;
	uint tri_total=0;
	uint bone_total=0;

	for(unsigned int i=0;i<scene->mNumMeshes;i++)
	{
		const aiMesh *mesh=scene->mMeshes[i];

		const uint v3fdata_size=mesh->mNumVertices*3*sizeof(float);
		const uint v4fdata_size=mesh->mNumVertices*4*sizeof(float);

		if(mesh->mName.length)
			mesh_name=U8_TEXT("Mesh [")+UTF8String(i)+U8_TEXT(":")+UTF8String(mesh->mName.C_Str())+U8_TEXT("]");
		else
			mesh_name=U8_TEXT("Mesh [")+UTF8String(i)+U8_TEXT("]");
	
		int pn=mesh->mFaces[0].mNumIndices;

		LOG_BR;

		LOG_INFO(mesh_name+U8_TEXT(" PrimitiveTypes is ")+UTF8String(mesh->mPrimitiveTypes));

		LOG_INFO(mesh_name+U8_TEXT(" vertex color count is ")+UTF8String(mesh->GetNumColorChannels()));
		LOG_INFO(mesh_name+U8_TEXT(" texcoord count is ")+UTF8String(mesh->GetNumUVChannels()));

		if(mesh->GetNumUVChannels()>0)
			LOG_INFO(mesh_name+U8_TEXT(" Material Index is ")+UTF8String(mesh->mMaterialIndex));

		if(mesh->HasPositions())LOG_INFO(mesh_name+U8_TEXT(" have Position, vertices count ")+UTF8String(mesh->mNumVertices));
		if(mesh->HasFaces())LOG_INFO(mesh_name+U8_TEXT(" have Faces,faces count ")+UTF8String(mesh->mNumFaces));
		if(mesh->HasNormals())LOG_INFO(mesh_name+U8_TEXT(" have Normals"));
		if(mesh->HasTangentsAndBitangents())LOG_INFO(mesh_name+U8_TEXT(" have Tangent & Bitangents"));
		if(mesh->HasBones())LOG_INFO(mesh_name+U8_TEXT(" have Bones, Bones Count ")+UTF8String(mesh->mNumBones));

		vertex_total+=mesh->mNumVertices;
		tri_total+=mesh->mNumFaces;
		bone_total+=mesh->mNumBones;
		
		LOG_INFO(mesh_name+U8_TEXT(" PN is ")+UTF8String(pn));

		if(pn!=3)
			continue;
		
		MaterialStruct *mtl=&(material_list[mesh->mMaterialIndex]);

		const uint uv_channels=mtl->uv_use.GetCount();

		LOG_INFO(mesh_name+U8_TEXT(" use UV Channels is ")+UTF8String(uv_channels));

		io::FileOutputStream fos;

		OSString mesh_filename=main_filename+OS_TEXT(".")+OSString(i)+OS_TEXT(".mesh");

		if(!fos.CreateTrunc(mesh_filename))
		{
			LOG_INFO(OS_TEXT("Create Mesh file Failed,filename: ")+mesh_filename);
			continue;
		}

		{
			MeshStruct ms;

			ms.primitive_type	=HGL_PRIM_TRIANGLES;
			ms.vertices_number	=mesh->mNumVertices;
			ms.faces_number		=mesh->mNumFaces;
			ms.color_channels	=mesh->GetNumColorChannels();
			ms.texcoord_channels=uv_channels;
			ms.material_index	=mesh->mMaterialIndex;

			if(mesh->HasNormals())
			{
				if(mesh->HasTangentsAndBitangents())
					ms.ntb=NTB_BIT_ALL;
				else
					ms.ntb=NTB_BIT_NORMAL;
			}

			ms.bones_number		=mesh->mNumBones;

			fos.WriteFully(&ms,sizeof(MeshStruct));
		}

		if(mesh->HasPositions())
		{
			AxisY2Z(mesh->mVertices,mesh->mNumVertices);
			fos.WriteFully(mesh->mVertices,v3fdata_size);
		}

		if(mesh->HasNormals())
		{
			AxisY2Z(mesh->mNormals,mesh->mNumVertices);
			fos.WriteFully(mesh->mNormals,v3fdata_size);

			if(mesh->HasTangentsAndBitangents())
			{
				AxisY2Z(mesh->mTangents,mesh->mNumVertices);
				AxisY2Z(mesh->mBitangents,mesh->mNumVertices);

				fos.WriteFully(mesh->mTangents,v3fdata_size);
				fos.WriteFully(mesh->mBitangents,v3fdata_size);
			}
		}

		if(mesh->GetNumColorChannels()>0)
		{
			const int64 vertex_color_size=mesh->mNumVertices*4*mesh->GetNumColorChannels();

			uint8 *vertex_color=new uint8[vertex_color_size];
			uint8 *tp=vertex_color;
			
			for(int c=0;c<mesh->GetNumColorChannels();c++)
			{
				c4f_to_4b(tp,mesh->mColors[c],mesh->mNumVertices);
				tp+=mesh->mNumVertices*4;
			}

			fos.WriteFully(vertex_color,vertex_color_size);

			delete[] vertex_color;
		}

		if(mesh->GetNumUVChannels())
		{
			int tc=0;
			int comp_total=0;
			int *uv_use=mtl->uv_use.GetData();

			//这里要重新审视数据来源，并不是每一个纹理通道都有数据，并且和材质对应。
			、、材质中的uv index具体对应啥 还不是很清楚
			
			for(int c=0;c<AI_MAX_NUMBER_OF_TEXTURECOORDS;c++)
			{
				if(uv_use[c]>=mesh->GetNumUVChannels())
					uv_use[c]=0;

				comp_total+=mesh->mNumUVComponents[uv_use[c]];
			}

			comp_total*=mesh->mNumVertices;

			uint64 tc_size=mesh->GetNumUVChannels()+comp_total*sizeof(float);

			uint8 *tc_data=new uint8[tc_size];
			float *tp=(float *)(tc_data+mesh->GetNumUVChannels());
			for(int c=0;c<uv_channels;c++)
			{
				tc_data[c]=mesh->mNumUVComponents[uv_use[c]];

				SaveTexCoord(tp,mesh->mTextureCoords[c],mesh->mNumUVComponents[c],mesh->mNumVertices);

				tp+=mesh->mNumUVComponents[c]*mesh->mNumVertices;
			}
			
			fos.WriteFully(tc_data,tc_size);

			delete[] tc_data;
		}

		if(mesh->HasFaces())
		{
			if(mesh->mNumVertices>0xFFFF)
				SaveFaces<uint32>(&fos,mesh->mFaces,mesh->mNumFaces);
			else
				SaveFaces<uint16>(&fos,mesh->mFaces,mesh->mNumFaces);
		}

		fos.Close();
	}

	LOG_BR;
	LOG_INFO(OS_TEXT("TOTAL Vertices Count ")+OSString(vertex_total));
	LOG_INFO(OS_TEXT("TOTAL Faces Count ")+OSString(tri_total));
	LOG_INFO(OS_TEXT("TOTAL Bones Count ")+OSString(bone_total));

	LOG_INFO(OS_TEXT("TOTAL File Size ")+OSString(total_file_bytes));
	LOG_BR;
}

void AssimpLoader::LoadScene(const UTF8String &front,io::DataOutputStream *dos,const aiNode *nd)
{
	aiVector3D bb_min,bb_max;
	float box[6];
	get_bounding_box(nd,&bb_min,&bb_max);

	box[0]=bb_min.x;box[1]=bb_min.y;box[2]=bb_min.z;
	box[3]=bb_max.x;box[4]=bb_max.y;box[5]=bb_max.z;
	
	dos->WriteUTF8String(nd->mName.C_Str());	
	dos->WriteFloat(box,6);

	dos->WriteFloat((float *)&(nd->mTransformation),16);

	dos->WriteUint32(nd->mNumMeshes);

	LOG_INFO(front+U8_TEXT("[")+UTF8String(nd->mName.C_Str())+U8_TEXT("][Mesh:")+UTF8String(nd->mNumMeshes)+U8_TEXT("][SubNode:")+UTF8String(nd->mNumChildren)+U8_TEXT("]"))

	if(nd->mNumMeshes>0)
		dos->WriteUint32(nd->mMeshes,nd->mNumMeshes);

	const UTF8String new_front=U8_TEXT("  ")+front;

	dos->WriteUint32(nd->mNumChildren);

	for(unsigned int n=0;n<nd->mNumChildren;++n)
		LoadScene(new_front,dos,nd->mChildren[n]);
}

bool AssimpLoader::LoadFile(const OSString &filename)
{
	uint filesize;
	char *filedata;
	
	filesize=filesystem::LoadFileToMemory(filename,(void **)&filedata);

	if(!filedata)
		return(false);

	scene=aiImportFileFromMemory(filedata,filesize,aiProcessPreset_TargetRealtime_Quality|aiProcess_FlipUVs,nullptr);
	
	delete[] filedata;

	if(!scene)
		return(false);

	filename.SubString(main_filename,0,filename.FindRightChar(OS_TEXT('.')));

	get_bounding_box(scene->mRootNode,&scene_min,&scene_max);

	scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
	scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
	scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

	//这个顺序是假设了所有的材质和模型都有被使用
	//aiProcessPreset_TargetRealtime_Quality已经指定了会删除多余材质，所以这里不用处理。
	//但多余模型并不确定是否存在

	io::FileOutputStream fos;
	io::LEDataOutputStream dos(&fos);

	OSString scene_filename=main_filename+OS_TEXT(".scene");

	if(!fos.CreateTrunc(scene_filename))
	{
		LOG_INFO(OS_TEXT("Create Scene file Failed,filename: ")+scene_filename);
		return(false);
	}

	dos.WriteFully("SCENE\x1a\x01",7);

	dos.WriteUint32(scene->mNumMaterials);
	dos.WriteUint32(scene->mNumMeshes);

	LoadMaterial();										//载入所有材质
	LoadMesh();											//载入所有mesh
	LoadScene(UTF8String(""),&dos,scene->mRootNode);	//载入场景节点

	fos.Close();

	SaveTextures();
	
	return(true);
}
