#include"AssimpLoader.h"
#include<assimp/postprocess.h>
#include<assimp/cimport.h>
#include<hgl/FileSystem.h>
#include<hgl/type/Smart.h>

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

inline Matrix4f matrix_convert(const aiMatrix4x4 &m)
{
	Matrix4f result;

	memcpy(&result,&m,16*sizeof(float));

	return result;
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

void AssimpLoader::get_bounding_box (aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;

	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;

	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}

AssimpLoader::AssimpLoader()
{
	scene=0;
}

AssimpLoader::~AssimpLoader()
{
	if(scene)
		aiReleaseImport(scene);
}

void AssimpLoader::SaveFile(void *data,uint size,const OSString &ext_name)
{
	if(!data||size<=0)return;

	OSString filename=main_filename+OS_TEXT(".")+ext_name;

	if(filesystem::SaveMemoryToFile(filename,data,size)==size)
	{
		LOG_INFO(OS_TEXT("SaveFile: ")+filename+OS_TEXT(" , size: ")+OSString(size));

		total_file_bytes+=size;
	}
}

//void AssimpLoader::LoadMaterial()
//{
//	if(!scene->HasMaterials())return;
//
//	int ret1, ret2;
//	float c[4];
//	struct aiColor4D diffuse;
//	struct aiColor4D specular;
//	struct aiColor4D ambient;
//	struct aiColor4D emission;
//	float shininess, strength;
//	int two_sided;
//	int wireframe;
//	unsigned int max;	// changed: to unsigned
//
//	for(unsigned int m=0;m<scene->mNumMaterials;m++)
//	{
//		int tex_index=0;
//		aiReturn found=AI_SUCCESS;
//		aiString filename;
//
//		aiMaterial *mtl=scene->mMaterials[m];			
////		Material *mat=CreateMaterial();
//
//		found=mtl->GetTexture(aiTextureType_DIFFUSE,tex_index,&filename);
//
//		if(filename.length>6			//贴图扩展名.tex2d
//		 &&filename.data[0]				//起始不为0
//		 &&filename.data[0]!=L'$')		//起始不为$，不知为何，3dsmax有可能产生这种情况
//		{
//			Texture2D *tex=CreateTexture2D();
//			WideString fn=filename.data;
//
//			try
//			{
//				if(tex->LoadFile(fn,0,Texture2D::ltGenMipmaps))
//					tex_list.Add(fn,tex);
//			}
//			catch(...)
//			{
//				delete tex;
//				tex=NULL;
//			}
//
//			if(tex)
//			{
//				mat->SetTexture(mtcDiffuse,tex);
//
//				mat->SetAlphaTest(0.999f);				//透明度小于0.9的不显示，使用alpha test
//			}
//		}
//
//		mat_list.Add(mat);
//
//		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
//		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
//			color4_to_float4(&diffuse, c);
//		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
//		mat->SetDiffuse(c[0],c[1],c[2],c[3]);
//		mat->SetColor(c[0],c[1],c[2],c[3]);
//
//		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
//			color4_to_float4(&specular, c);
//		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//		mat->SetSpecular(c[0],c[1],c[2],c[3]);
//
//		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
//		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
//			color4_to_float4(&ambient, c);
//		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
//		mat->SetAmbient(c[0],c[1],c[2],c[3]);
//
//		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
//			color4_to_float4(&emission, c);
//		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
//		mat->SetEmission(c[0],c[1],c[2],c[3]);
//
//		max = 1;
//		ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
//		max = 1;
//		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
//
//		if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
//		{
//			//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
//			mat->SetShininess(shininess * strength);
//		}
//		else 
//		{
//			//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
//			mat->SetShininess(0);
//
//			set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
//			//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//			mat->SetSpecular(c[0],c[1],c[2],c[3]);
//		}
//
//		max = 1;
//		if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
//			if(wireframe)
//				mat->SetFillMode(HGL_DRAW_LINE);
//
//		max = 1;
//		if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
//			mat->SetCullFace(HGL_FACE_BACK);
//		else
//			mat->SetCullFace(0);		//双面材质
//	}
//}

template<typename T>
void AssimpLoader::SaveFaces(const aiFace *face,const T count,const OSString &extname)
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

	SaveFile(data,sizeof(T)*3*count,extname);

	delete[] data;
}

void AssimpLoader::LoadMesh()
{
	if(!scene->HasMeshes())return;

	UTF8String mesh_name;

	total_file_bytes=0;

	LOG_INFO(OS_TEXT("Mesh Count ")+OSString(scene->mNumMeshes));

	uint vertex_total=0;
	uint tri_total=0;
	uint bone_total=0;

	for(unsigned int i=0;i<scene->mNumMeshes;i++)
	{
		const aiMesh *mesh=scene->mMeshes[i];
//		Material *mtl=mat_list[mesh->mMaterialIndex];

//		Renderable *obj=CreateRenderable();

//		mesh_list.Add(obj);

//		obj->SetMaterial(mtl,false);		//设置模型材质，不独占

		if(mesh->mName.length)
			mesh_name=U8_TEXT("Mesh [")+UTF8String(i)+U8_TEXT(":")+UTF8String(mesh->mName.C_Str())+U8_TEXT("]");
		else
			mesh_name=U8_TEXT("Mesh [")+UTF8String(i)+U8_TEXT("]");
	
		bool vertex_color=false;
		bool diffuse_coord=false;
		int pn=mesh->mFaces[0].mNumIndices;

		LOG_INFO(OS_TEXT("------------------------------------------------------------"));
		LOG_INFO(mesh_name+U8_TEXT(" PrimitiveTypes is ")+UTF8String(mesh->mPrimitiveTypes));

		LOG_INFO(mesh_name+U8_TEXT(" vertex color count is ")+UTF8String(mesh->GetNumColorChannels()));
		LOG_INFO(mesh_name+U8_TEXT(" texcoord count is ")+UTF8String(mesh->GetNumUVChannels()));

		if(mesh->HasPositions())LOG_INFO(mesh_name+U8_TEXT(" have Position, vertices count ")+UTF8String(mesh->mNumVertices));
		if(mesh->HasFaces())LOG_INFO(mesh_name+U8_TEXT(" have Faces,faces count ")+UTF8String(mesh->mNumFaces));
		if(mesh->HasNormals())LOG_INFO(mesh_name+U8_TEXT(" have Normals"));
		if(mesh->HasTangentsAndBitangents())LOG_INFO(mesh_name+U8_TEXT(" have Tangent & Bitangents"));
		if(mesh->HasBones())LOG_INFO(mesh_name+U8_TEXT(" have Bones, Bones Count ")+UTF8String(mesh->mNumBones));

		vertex_total+=mesh->mNumVertices;
		tri_total+=mesh->mNumFaces;
		bone_total+=mesh->mNumBones;
		
		if(mesh->mColors[0]!=NULL)
		{
			vertex_color=true;
		}

		//if(mesh->HasTextureCoords(0)		//有贴图坐标数据
		// &&mtl->GetTexture(mtcDiffuse))		//也确实载入了贴图
		//	diffuse_coord=true;

		LOG_INFO(mesh_name+U8_TEXT(" PN is ")+UTF8String(pn));

		if(pn!=3)
			continue;

		const uint v3fdata_size=mesh->mNumVertices*3*sizeof(float);
		const uint v4fdata_size=mesh->mNumVertices*4*sizeof(float);

		//保存顶点
		if(mesh->HasPositions())
			SaveFile(mesh->mVertices,v3fdata_size,OSString(i)+OS_TEXT(".vertex"));

		if(mesh->HasNormals())
			SaveFile(mesh->mNormals,v3fdata_size,OSString(i)+OS_TEXT(".normal"));

		if(mesh->HasTangentsAndBitangents())
		{
			SaveFile(mesh->mTangents,v3fdata_size,OSString(i)+OS_TEXT(".tangent"));
			SaveFile(mesh->mBitangents,v3fdata_size,OSString(i)+OS_TEXT(".bitangent"));
		}

		for(int c=0;c<mesh->GetNumColorChannels();c++)
			SaveFile(mesh->mColors[c],v4fdata_size,OSString(i)+OS_TEXT(".")+OSString(c)+OS_TEXT(".color"));

		for(int c=0;c<mesh->GetNumUVChannels();c++)
			SaveFile(mesh->mTextureCoords[c],mesh->mNumVertices*mesh->mNumUVComponents[c]*sizeof(float),OSString(i)+OS_TEXT(".")+OSString(c)+OS_TEXT(".texcoord"));

		if(mesh->HasFaces())
		{
			if(mesh->mNumVertices>0xFFFF)
				SaveFaces<uint32>(mesh->mFaces,mesh->mNumFaces,OSString(i)+OS_TEXT(".face"));
			else
				SaveFaces<uint16>(mesh->mFaces,mesh->mNumFaces,OSString(i)+OS_TEXT(".face"));
		}
	}

	LOG_INFO(OS_TEXT("------------------------------------------------------------"));
	LOG_INFO(OS_TEXT("TOTAL Vertices Count ")+OSString(vertex_total));
	LOG_INFO(OS_TEXT("TOTAL Faces Count ")+OSString(tri_total));
	LOG_INFO(OS_TEXT("TOTAL Bones Count ")+OSString(bone_total));

	LOG_INFO(OS_TEXT("TOTAL File Size ")+OSString(total_file_bytes));
}

//void AssimpLoader::LoadScene(SceneNode *node,const aiScene *sc,const aiNode *nd)
//{
//	aiMatrix4x4 m=nd->mTransformation;
//
//	aiTransposeMatrix4(&m);
//
//	node->SetLocalMatrix(matrix_convert(m));		//强制COPY，可能有问题
//
//	for(unsigned int n=0;n<nd->mNumMeshes;++n)
//		node->SubData.Add(mesh_list[nd->mMeshes[n]]);
//
//	for(unsigned int n=0;n<nd->mNumChildren;++n)
//	{
//		SceneNode* sub_node=new SceneNode;
//
//		LoadScene(sub_node,sc,nd->mChildren[n]);
//
//		node->AddSubNode(sub_node);
//	}
//}

bool AssimpLoader::LoadFile(const OSString &filename)
{
	uint filesize;
	char *filedata;
	
	filesize=filesystem::LoadFileToMemory(filename,(void **)&filedata);

	if(!filedata)
		return(false);

	scene=aiImportFileFromMemory(filedata,filesize,aiProcessPreset_TargetRealtime_MaxQuality,nullptr);
	
	delete[] filedata;

	if(!scene)
		return(false);

	filename.SubString(main_filename,0,filename.FindRightChar(OS_TEXT('.')));

	get_bounding_box(&scene_min,&scene_max);

	scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
	scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
	scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

	//这个顺序是假设了所有的材质和模型都有被使用
	//aiProcessPreset_TargetRealtime_Quality已经指定了会删除多余材质，所以这里不用处理。
	//但多余模型并不确定是否存在

//	LoadMaterial();								//载入所有材质
	LoadMesh();									//载入所有mesh
//	LoadScene(&root,scene,scene->mRootNode);	//载入场景节点

	//root.RefreshMatrix();						//刷新矩阵

	//root.ExpendToList(&render_list);			//展开到渲染列表

//	root.SaveToStream(&FileStream(L"Root.scene"));
	
	return(true);
}

