#include<GL/glew.h>
#include<hgl/graph/Render.h>
#include<hgl/stream/FileStream.h>
#include<hgl/type/DateTime.h>
#include<hgl/File.h>

namespace hgl
{
	namespace graph
	{
		void FlipBitmap(uint8 *tar,uint8 *src,int width,int height,int byte)
		{
			int size=width*byte;

			tar+=size*(height-1);

			while(height--)
			{
				memcpy(tar,src,size);

				src+=size;
				tar-=size;
			}
		}

		uint8 *GetScreenData(int left,int top,int width,int height,uint color,bool flip)
		{
			uint8 *ori_data,*data;

			ori_data=new uint8[width*height*3];

			glReadPixels(left,top,width,height,color,GL_UNSIGNED_BYTE,ori_data);

			if(!flip)
				return ori_data;

			data=new uint8[width*height*3];

			FlipBitmap(data,ori_data,width,height,color);

			delete[] ori_data;

			return data;
		}

		uint8 *GetScreenData(int fin_width,int fin_height,int left,int top,int width,int height,uint color,bool flip)
		{
			if(fin_width==width
			 &&fin_height==height)
				return(GetScreenData(left,top,width,height,color,flip));

			uint8 *ori_data=GetScreenData(left,top,width,height,color,flip);

			uint8 *data=new uint8[fin_width*fin_height*3];

			gluScaleImage(	color,
							width,		height,		GL_UNSIGNED_BYTE,ori_data,
							fin_width,	fin_height,	GL_UNSIGNED_BYTE,data);

			delete[] ori_data;

			return(data);
		}

		bool CreateScreenShotFilename(UTF16String &fn,const u16char *ext_name)
		{
			const u16char *path=GetString(hfsStartPath);

			fn=path;
			fn+=u"\\ScreenShot";

			if(!MakePath(fn))
				return(false);

			Date d;
			Time t;

			d.Sync();
			t.Sync();

			fn.CatSprintf(	u"\\%4d-%02d-%02d %02d.%02d.%02d.%d.%s",
							d.Year.operator int(),
							d.Month.operator int(),
							d.Day.operator int(),
							t.Hour.operator int(),
							t.Minute.operator int(),
							t.Second.operator int(),
							t.MilliSecond.operator int(),
							ext_name);

			return(true);
		}

		/**
		* 截取当前屏幕图像保存成BMP文件
		* @param filename 文件名,如不输入表示自动产生文件名(于 启动目录\ScreenShot 目录下)
		* @return 保存是否成功
		*/
		bool SaveScreenToBMP(const u16char *filename,int width,int height)
		{
			FileStream fp;
			uint32 line_size;
			uint8 *data,*p;
			int row;
			UTF16String fn;

			if(filename)fn=filename;
			else
			{
				if(!CreateScreenShotFilename(fn,u"BMP"))
            		return(false);
			}

			if(fp.Create(fn)==false)return(false);

			if(width==-1||height==-1)
				graph::GetScreenSize(width,height);

			data=GetScreenData(width,height,0,0,width,height,GL_BGR,false);

			if(data==nullptr)return(false);

			line_size=(width*3+3)>>2;
			line_size<<=2;

			fp.Write("BM",2);
			fp.WriteUint32(54+line_size*height);
			fp.WriteUint32(0);
			fp.WriteUint32(54);
			fp.WriteUint32(40);
			fp.WriteUint32(width);
			fp.WriteUint32(height);
			fp.WriteUint16(1);
			fp.WriteUint16(24);
			fp.WriteUint32(0);
			fp.WriteUint32(line_size*height);
			fp.WriteUint32(10000);
			fp.WriteUint32(10000);
			fp.WriteUint32(0xFFFFFF+1);	//使用所有的颜色
			fp.WriteUint32(0xFFFFFF+1);	//使用所有的颜色

			row=height;
			p=data;
			while(row--)
			{
				fp.Write(p,line_size);
				p+=width*3;
			}

			fp.Close();
			delete[] data;

			return(true);
		}

		/**
		* 截取当前屏幕图像保存成TGA文件
		* @param filename 文件名,如不输入表示自动产生文件名(于 启动目录\ScreenShot 目录下)
		* @return 保存是否成功
		*/
		bool SaveScreenToTGA(const u16char *filename,int width,int height)
		{
			const uint8 TGAHeader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

			FileStream fp;
			uint8 *data;
			int size;
			UTF16String fn;

			if(filename)fn=filename;
			else
			{
				if(!CreateScreenShotFilename(fn,u"TGA"))
					return(false);
			}

			if(fp.Create(fn)==false)return(false);

			if(width==-1||height==-1)
				graph::GetScreenSize(width,height);

			data=GetScreenData(width,height,0,0,width,height,GL_BGR,false);

			size=width*height*3;

			fp.Write(TGAHeader,12);
			fp.WriteUint16(width);
			fp.WriteUint16(height);
			fp.WriteUint16(24);
			fp.Write(data,size);

			fp.Close();

			delete[] data;

			return(true);
		}
	}//namespace graph
}//namespace hgl
