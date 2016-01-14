#include<hgl/Graphics.h>

#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/graph/Shader.h>			//GLSL
#include<hgl/graph/Texture.h>			//Texture
#include<hgl/graph/Material.h>			//Material
#include<hgl/graph/VertexBuffer.h>		//VB3f/VB4f
#include<hgl/graph/Renderable.h>		//Renderable

#include<hgl/io/FileOutputStream.h>
#include<hgl/ut/CmdParse.h>
#include<hgl/graph/Image.h>
#include<hgl/Other.h>
#include<hgl/Str.h>
#include<hgl/Info.h>                    //GetString

#include<IL/il.h>
#include<IL/ilu.h>

#include<string.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::util;
using namespace hgl::graph;

//--------------------------------------------------------------------------------------------------
const	int							tf_count	=sizeof(tf_list)/sizeof(TextureFormat);
		TextureFormat *	glfmt[4]	={NULL,NULL,NULL,NULL};				//选中格式
		bool						gen_mipmaps	=false;								//是否产生mipmaps

		bool						only_view	=false;								//仅显示

const	float						texcoord[]	={0.0f,0.0f,	1.0f,0.0f,	1.0f,1.0f,	0.0f,0.0f,	1.0f,1.0f,	0.0f,1.0f};

		Renderable *				render_obj	=NULL;
		Material *					mtl			=NULL;
		Texture2D *					tex			=NULL;
		VB2f *						vertex		=NULL;
//--------------------------------------------------------------------------------------------------
//OpenIL数据

uint il_index=0;		//图片索引
int il_width,il_height,il_bit;
uint il_format=0;
uint il_type=0;
uint il_pal_type=0;
uint il_pal_color_num=0;
//--------------------------------------------------------------------------------------------------
//统计数据

int max_pixels=0;			//最大像素数
uint8 *image_data=NULL;		//图像数据

int max_bytes=0;			//最大字节数
uint8 *texture_data=NULL;	//贴图数据

int convert_count=0;		//转换总数
int bytes_count=0;			//总字节数
int cbytes_count=0;			//转换后总字节数
//--------------------------------------------------------------------------------------------------
TextureFormat *GetFormat(const char *str)
{
	TextureFormat *p=tf_list;

	for(int i=0;i<tf_count;i++)
		if(strcmp(str,p->name)==0)
			return p;
		else
			p++;

	return(NULL);
}
//--------------------------------------------------------------------------------------------------
TextureFormat *CheckOpenGLCoreFormat(const CmdParse &cmd,const os_char *flag)
{
	OSString fmtstr;

	if(!cmd.GetString(flag,fmtstr))return(NULL);

	for(int i=0;i<tf_count;i++)
		if(fmtstr.Comp(tf_list[i].name)==0)
			return(tf_list+i);

	return(NULL);
}

void CheckOpenGLCoreFormat(const CmdParse &cmd)
{
	//指定格式
	glfmt[0]=CheckOpenGLCoreFormat(cmd,OS_TEXT("/R:"));
	glfmt[1]=CheckOpenGLCoreFormat(cmd,OS_TEXT("/RG:"));
	glfmt[2]=CheckOpenGLCoreFormat(cmd,OS_TEXT("/RGB:"));
	glfmt[3]=CheckOpenGLCoreFormat(cmd,OS_TEXT("/RGBA:"));

	//默认格式
	if(!glfmt[0])glfmt[0]=GetFormat("R8");
	if(!glfmt[1])glfmt[1]=GetFormat("RG8");
	if(!glfmt[2])glfmt[2]=GetFormat("RGB8");
	if(!glfmt[3])glfmt[3]=GetFormat("RGBA8");
}

bool CheckSameAlpha(uint8 *data,uint count)
{
	uint8 *p=data;

	while(count--)
	{
		if(*p!=*data)return(false);

		p++;
	}

	return(true);
}

/**
* 检测RGB数据是否一致
*/
bool CheckSameRGB(uint8 *data,int color,uint count)
{
	uint8 *p=data;

	while(count--)
	{
		if(memcmp(data,p,color-1))
			return(false);

		p+=color;
	}

	return(true);
}

void MixLA(uint8 *lum,uint8 *alpha,int size)
{
	int i;

    for(i=0;i<size;i++)
    {
    	lum++;
        *lum++=*alpha++;
    }
}

void MixRGBA(uint8 *rgba,uint8 *alpha,int size)
{
	int i;

    for(i=0;i<size;i++)
    {
    	rgba+=3;
        *rgba++=*alpha++;
    }
}

void FlipData(uint8 *data,int line_bytes,int row_number)
{
	int i;
    uint8 *temp=new uint8[line_bytes];
    uint8 *end=data+line_bytes*(row_number-1);

    for(i=0;i<row_number/2;i++)
    {
    	memcpy(temp,data,line_bytes);
        memcpy(data,end,line_bytes);
        memcpy(end,temp,line_bytes);

        data+=line_bytes;
        end-=line_bytes;
    }

    delete[] temp;
}

void CheckFormat()
{
	uint fmt=il_format;

	if(il_format==IL_BGR)
		fmt=IL_RGB;
	else
	if(il_format==IL_BGRA)
		fmt=IL_RGBA;

	if(il_format==IL_RGBA||il_format==IL_BGRA||il_format==IL_LUMINANCE_ALPHA)
	{
		uint8 *alpha=ilGetAlpha(IL_UNSIGNED_BYTE);
		uint count=il_width*il_height;
		uint color=(il_format==IL_LUMINANCE_ALPHA?2:3);

		if(CheckSameAlpha(alpha,count))				//检测ALPHA全部一样的，将RGBA转为RGB,将LA转为L
		{
			if(il_format==IL_RGBA||il_format==IL_BGRA)
			{
				fmt=IL_RGB;
			}
			else
			if(il_format==IL_LUMINANCE_ALPHA)
			{
				fmt=IL_LUMINANCE;
			}
		}
		else									//如果ALPHA不同，检测颜色是否相同，如果相同只留ALPHA
		{
			uint8 *data=ilGetData();
			uint count=il_width*il_height;

			if(CheckSameRGB(data,color,count))
				fmt=IL_ALPHA;
		}

		if(alpha)free(alpha);
	}

	if(il_format==IL_RGB||il_format==IL_BGR)
	{
		if(CheckMono(ilGetData(),3,il_width,il_height))
			fmt=IL_LUMINANCE;
	}

	if(fmt!=IL_ALPHA								//只有ALPHA就不检测
	&&(il_format==IL_RGBA||il_format==IL_BGRA))
	{
		if(CheckMono(ilGetData(),4,il_width,il_height))
		{
			if(fmt==IL_RGB)				//前面一段代码已经将alpha砍掉了
				fmt=IL_LUMINANCE;
			else
				fmt=IL_LUMINANCE_ALPHA;
		}
	}

	if(fmt!=il_format)
	{
		ilConvertImage(fmt,il_type);

		il_format=fmt;
	}
}

void CheckPalette()
{
	uint pal=il_pal_type;

	if(il_pal_type==IL_PAL_BGR24||il_pal_type==IL_PAL_BGR32||il_pal_type==IL_PAL_RGB32)
		pal=IL_PAL_RGB24;
	else
	if(il_pal_type==IL_PAL_BGRA32)
		pal=IL_PAL_RGBA32;

	if(pal!=il_pal_type)
	{
		ilConvertPal(pal);

		il_pal_type=pal;
	}
}

void SaveTexture2DToFile(const os_char *filename,void *texture_data,uint width,uint height,char *format,uint bytes)
{
	os_char tex_fn[HGL_MAX_PATH];
	char fmt_str[17];

	replace_extname(filename,tex_fn,HGL_MAX_PATH,OS_TEXT("tex2d"));

	FileOutputStream fs;
    DataOutputStream *dos;

	if(!fs.Create(tex_fn))
	{
		LOG_INFO(OS_TEXT("\tCreateFile(")+OSString(tex_fn)+OS_TEXT(") error!"));
		return;
	}

	dos=new LEDataOutputStream(&fs);

	memset(fmt_str,0,16);
	strcpy(fmt_str,format);

	dos->Write("Tex2D\x1A",6);
	dos->WriteFloat(1.0);						//版本号

	if(gen_mipmaps)								//产生mipmaps
	{
		dos->WriteUint32(4*sizeof(uint32)+16);	//文件头长度

		dos->WriteUint32(width);					//宽
		dos->WriteUint32(height);					//高
		dos->Write(fmt_str,16);					//格式
		dos->WriteUint32(bytes);					//字节数
	}
	else										//不产生mipmaps
	{
		dos->WriteUint32(4*sizeof(uint32)+16);	//文件头长度

		dos->WriteUint32(width);					//宽
		dos->WriteUint32(height);					//高
		dos->Write(fmt_str,16);					//格式
		dos->WriteUint32(bytes);					//字节数

		dos->Write(texture_data,bytes);
	}

	delete dos;

	LOG_INFO(OS_TEXT("\tSave to ")+OSString(tex_fn));
}

int ConvertImage(const os_char *filename)
{
	uint pixels=0;
	TextureFormat *curfmt=NULL;
	TextureFormat *tarfmt=NULL;

	LOG_INFO(OS_TEXT("File: ")+OSString(filename));

	ilGenImages(1,&il_index);
	ilBindImage(il_index);

	if(ilLoadImage(filename))
	{
		il_width	=ilGetInteger(IL_IMAGE_WIDTH);
		il_height	=ilGetInteger(IL_IMAGE_HEIGHT);
		il_bit		=ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
		il_format	=ilGetInteger(IL_IMAGE_FORMAT);
		il_type		=ilGetInteger(IL_IMAGE_TYPE);

		if(ilGetInteger(IL_IMAGE_ORIGIN)==IL_ORIGIN_LOWER_LEFT)
			iluFlipImage();

		LOG_INFO(OS_TEXT("\twidth: ")+OSString(il_width));
		LOG_INFO(OS_TEXT("\theight: ")+OSString(il_height));

		pixels=il_width*il_height;

		if(pixels>max_pixels)
			SAFE_CLEAR_ARRAY(image_data);

		if(!image_data)
		{
			image_data=new uint8[pixels*4*4];
			max_pixels=pixels;
		}

		if(il_format!=IL_COLOR_INDEX)
		{
			if(il_format==IL_LUMINANCE		)
			{
				if(il_type==IL_UNSIGNED_BYTE)
					curfmt=GetFormat("R8");
				else
				if(il_type==IL_UNSIGNED_SHORT)
					curfmt=GetFormat("R16");
				else
				if(il_type==IL_FLOAT)
					curfmt=GetFormat("R32F");
				else
				{
					ilConvertImage(IL_LUMINANCE,IL_UNSIGNED_BYTE);
					il_type=IL_UNSIGNED_BYTE;

					curfmt=GetFormat("R8");
				}

				tarfmt=(glfmt[0]?glfmt[0]:curfmt);

				memcpy(image_data,ilGetData(),pixels*curfmt->source_bytes);
			}
			else
			{
				if(il_type!=IL_UNSIGNED_BYTE)
				{
					ilConvertImage(il_format,IL_UNSIGNED_BYTE);
					il_type=IL_UNSIGNED_BYTE;
				}

				CheckFormat();

				if(il_format==IL_ALPHA			)
				{
					curfmt=GetFormat("R8");
					tarfmt=(glfmt[0]?glfmt[0]:curfmt);

					memcpy(image_data,ilGetAlpha(GL_UNSIGNED_BYTE),pixels);
				}
				else
				if(il_format==IL_LUMINANCE_ALPHA)
				{
					curfmt=GetFormat("RG8");
					tarfmt=(glfmt[1]?glfmt[1]:curfmt);

					uint8 *alpha=ilGetAlpha(GL_UNSIGNED_BYTE);

					memcpy(image_data,ilGetData(),pixels*2);

					MixLA(image_data,alpha,pixels);

					free(alpha);
				}
				else
				if(il_format==IL_RGB			)
				{
					curfmt=GetFormat("RGB8");
					tarfmt=(glfmt[2]?glfmt[2]:curfmt);

					memcpy(image_data,ilGetData(),pixels*3);
				}
				else
				if(il_format==IL_RGBA			)
				{
					curfmt=GetFormat("RGBA8");
					tarfmt=(glfmt[3]?glfmt[3]:curfmt);

					uint8 *alpha=ilGetAlpha(GL_UNSIGNED_BYTE);

					memcpy(image_data,ilGetData(),pixels*4);

					MixRGBA(image_data,alpha,pixels);

					free(alpha);
				}
				else
				{
					curfmt=NULL;
					tarfmt=NULL;

					LOG_INFO("\tsource format error");

					ilDeleteImages(1,&il_index);
					return(0);
				}
			}//LUMINANCE

			tex->SetImage(il_width,il_height,image_data,pixels*curfmt->source_bytes,curfmt->tsf,tarfmt->internalFormat);

			LOG_INFO(OS_TEXT("\tcolor format = ")+OSString(curfmt->name)+OS_TEXT(" bytes = ")+OSString(curfmt->source_bytes*il_width*il_height));

			LOG_INFO(OS_TEXT("\ttarget format = ")+OSString(tarfmt->name));

			int bytes=tex->GetImage(NULL,tarfmt->tsf);

			if(bytes>0)
			{
				LOG_INFO(OS_TEXT("\toutput bytes = ")+OSString(bytes));

				if(bytes>max_bytes)
					SAFE_CLEAR_ARRAY(texture_data);

				if(!texture_data)
				{
					texture_data=new uint8[bytes];
					max_bytes=bytes;
				}

				tex->GetImage(texture_data,tarfmt->tsf);

				SaveTexture2DToFile(filename,texture_data,il_width,il_height,tarfmt->name,bytes);

				convert_count++;
				bytes_count+=il_width*il_height*curfmt->source_bytes;
				cbytes_count+=bytes;
			}
		}
		else	//索引色贴图
		{
			il_pal_type		=ilGetInteger(IL_PALETTE_TYPE);		//调色板类型
			il_pal_color_num=ilGetInteger(IL_PALETTE_NUM_COLS);	//颜色数量

			LOG_INFO(OS_TEXT("\tpal color number = ")+OSString(il_pal_color_num));

			if(il_pal_color_num==16||il_pal_color_num==256)
			{
				CheckPalette();

				if(il_pal_type==IL_PAL_RGB24 &&il_pal_color_num== 16)curfmt=GetFormat("16RGB");else

				if(il_pal_type==IL_PAL_RGBA32&&il_pal_color_num== 16)curfmt=GetFormat("16RGBA");else
				if(il_pal_type==IL_PAL_RGB24 &&il_pal_color_num==256)curfmt=GetFormat("256RGB");else
				if(il_pal_type==IL_PAL_RGBA32&&il_pal_color_num==256)curfmt=GetFormat("256RGBA");else
					curfmt=NULL;

				if(curfmt)
					LOG_INFO(OS_TEXT("\tcolor format = ")+OSString(curfmt->name));
			}
			else
				curfmt=NULL;
		}

		if(!curfmt)
			LOG_INFO("\tformat error!\n");
	}
	else
	{
		LOG_INFO("\tLoad File error!");
	}

	ilDeleteImages(1,&il_index);
	return(0);
}

void EnumConvertImage(void *app,FileInfo &fi)
{
	GraphicsApplication *gapp=(GraphicsApplication *)app;

	ConvertImage(fi.fullname);

	ClearScreen();

	const int l=(640-il_width)/2;
	const int t=(480-il_height)/2;

	vertex->Begin();
		vertex->WriteRect(l,t,il_width,il_height);
	vertex->End();

	DirectRender2D(render_obj);

	gapp->SwapBuffer();
	gapp->WaitActive();
}

HGL_GRAPHICS_MAIN(sii,app,args)
{
	CmdParse cmd(args);

	bool sub=false;

	sii.info.ProjectName=OS_TEXT("贴图转换");
	sii.info.ProjectCode=OS_TEXT("Texture Converter");
	sii.info.ProjectVersion=OS_TEXT("1.0");

	if(args.GetCount()<2)
	{
		char *hint=new char[32*tf_count];

		::strcpy(hint,"support format:\n\n");

		for(int i=0;i<tf_count;i++)
		{
			::strcat(hint,tf_list[i].name);
			::strcat(hint,"\n");
		}

		LOG_INFO(hint);

		delete[] hint;
		return(0);
	}

	if(!app.Init(&sii))return(-1);

	if(cmd.Find(OS_TEXT("/s"))!=-1)sub=true;						//检测是否处理子目录

	if(cmd.Find(OS_TEXT("/view"))!=-1)only_view=true;				//检测是否仅显示
	if(cmd.Find(OS_TEXT("/mip"))!=-1)gen_mipmaps=true;				//检测是否生成mipmaps

	CheckOpenGLCoreFormat(cmd);								//检测推荐格式

	ilInit();

	OSString cur_path=info::GetString(info::hfsStartPath);

	render_obj=CreateRenderable();
	render_obj->SetPrimitive(HGL_PRIM_TRIANGLES);

	render_obj->SetVertexBuffer(vbtVertex,vertex=new VB2f(6,0,HGL_DYNAMIC_DRAW));
	render_obj->SetVertexBuffer(vbtDiffuseTexCoord,new VB2f(6,texcoord));

	tex=CreateTexture2D();

	mtl=CreateMaterial();

	mtl->SetColorMaterial(false);
	mtl->SetTexture(mtcDiffuse,tex);

	render_obj->SetMaterial(mtl,true);
	render_obj->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord);

	render_obj->AutoCreateShader();

	double start_time=GetTime();
	double end_time;

	//EnumFile(cur_path,args[1],&app,false,true,sub,EnumConvertImage);

	end_time=GetTime();

	LOG_INFO(OS_TEXT("总计转换图片")+OSString(convert_count)
            +OS_TEXT("张，总计处理原始数据")+OSString(bytes_count)
            +OS_TEXT("字节，转换后")+OSString(cbytes_count)
            +OS_TEXT("字节，总计耗时")+OSString(end_time-start_time)+OS_TEXT("秒"));

	delete[] image_data;
	delete[] texture_data;
	delete tex;
	delete render_obj;

	ilShutDown();

	return(0);
}
