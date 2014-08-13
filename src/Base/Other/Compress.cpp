#include<hgl/LogInfo.h>
#include<hgl/Compress.h>
#include<hgl/PlugIn.h>
#include<hgl/Other.h>
#include<hgl/type/Map.h>
namespace hgl
{
	u16char CAName[caEnd][32]=
	{
		U16_TEXT(""),

		U16_TEXT("Bzip2"),
		U16_TEXT("Lzo"),
		U16_TEXT("Lzma"),
		U16_TEXT("Zip"),
		U16_TEXT("RLE"),
		U16_TEXT("RYC"),
		U16_TEXT("RSC"),
		U16_TEXT("Jpeg2000"),
		U16_TEXT("VSRLE")
	};

	CA NameToCA(const UTF16String &name)
	{
		if(name.Length()<=0)return(caNone);

		uint ca=caNone;

		while(++ca<caEnd)
			if(name==CAName[ca])return(CA(ca));

		LOG_PROBLEM(U16_TEXT("未知的压缩算法：") + name);

		return(caNone);
	}

	const UTF16String CAToName(CA ca)
	{
		UTF16String name;

		if (ca <= caNone || ca >= caEnd)name = U16_TEXT("未知算法");
			else name=CAName[ca];

		return name;
	}
//--------------------------------------------------------------------------------------------------
	struct CompressPlugInInterface
	{
		uint32	(*Compress				)(void *,uint32,void *,uint32,float);
		uint32	(*Decompress			)(void *,uint32,void *,uint32);

		void *	(*CreateCompressStream	)(float);
		uint32	(*CompressStream		)(void *,void *,uint32 &,bool,void *,uint32 &);
		void	(*CloseCompressStream	)(void *);

		void *	(*CreateDecompressStream)();
		uint32	(*DecompressStream		)(void *,void *,uint32 &,void *,uint32 &);
		void	(*CloseDecompressStream	)(void *);

		uint32 	(*ImageCompress			)(void *,uint32,void *,int,int,int,float);
		uint32 	(*ImageDecompress		)(void *,void *,uint32,int,int,int);
	};

	/*PlugInManage(	CompressInterface,			//管理器名称，仅此一次，所以随便命名
					CompressPlugInInterface,	//接口结构名称,来自外部
					CompressInterfaceCheck,		//检查接口函数名称,要对外用
					u"Compress",				//插件前缀
					3);							//当前所用版本
					*/
	PlugInManage(Compress,"Compress",3);
//--------------------------------------------------------------------------------------------------
	/**
	* 压缩一个内存块的数据并存放到另一个内存块
	* @param ca_name 压缩算法
	* @param data 原始数据指针
	* @param size 原始数据的长度
	* @param compressdata 压缩后数据的存放位置
	* @param out_max 压缩后输出数据存放区大小
	* @param level 压缩级别(0:不压缩,1:最大压缩比)
	* @return 压缩后数据的长度
	* @see CompressAlgorithm
	* @see Decompress
	*/
	uint32 Compress(const OSString &ca_name,void *data,uint32 size,void *compressdata,uint32 out_max,float level)
	{
		if(!data||!size||!compressdata||!out_max)return(0);

		CompressPlugInInterface *func=CompressInterfaceCheck(ca_name);

		if(func&&func->Compress)
			return func->Compress(data,size,compressdata,out_max,level);

		return(0);
	}

	/**
	* 解压缩一个内存块的数据并存放到另一个内存块
	* @param ca_name 压缩算法
	* @param compressdata 压缩数据的存放位置
	* @param size 压缩数据的长度
	* @param out_data 解压缩后数据的存放位置
	* @param out_max 解压缩数据存放区的大小
	* @return 解压缩后的数据长度
	* @see CompressAlgorithm
	* @see Compress
	*/
	uint32 Decompress(const OSString &ca_name,void *compressdata,uint32 size,void *out_data,uint32 out_max)
	{
		if(!out_data||!size||!compressdata||!out_max)return(0);

		CompressPlugInInterface *func=CompressInterfaceCheck(ca_name);

		if(func&&func->Decompress)
			return func->Decompress(compressdata,size,out_data,out_max);

		return(0);
	}

	/**
	* 解压缩一个图像数据
	* @param ca_name 压缩算法
	* @param buffer 压缩后的数据存放缓冲区
	* @param buffer_size 缓冲区大小
	* @param data 原始的图像数据
	* @param width 图像宽
	* @param height 图像高
	* @param channels 图像通道数
	* @param level 压缩级别(0:不压缩,1:最大压缩比)
	* @return 压缩后的数据大小
	*/
	uint32 ImageCompress(const OSString &ca_name,void *buffer,uint32 buffer_size,void *data,int width,int height,int channels,float level)
	{
		if(!buffer||!buffer_size||!data)return(0);

		CompressPlugInInterface *func=CompressInterfaceCheck(ca_name);

		if(func)
		{
			if(func->ImageCompress)
				return func->ImageCompress(buffer,buffer_size,data,width,height,channels,level);
			else
			if(func->Compress)
				return func->Compress(data,width*height*channels,buffer,buffer_size,level);
		}

		return(0);
	}

	/**
	* 解压缩一个图像数据
	* @param ca_name 压缩算法
	* @param data 解压后的数据存放处
	* @param buffer 压缩的数据指针
	* @param size 压缩数据的长度
	* @param width 图像宽
	* @param height 图像高
	* @param channels 图像通道数
	* @return 解压缩后的数据大小
	*/
	uint32 ImageDecompress(const OSString &ca_name,void *data,void *buffer,uint32 size,int width,int height,int channels)
	{
		if(!buffer||!size||!data)return(0);

		CompressPlugInInterface *func=CompressInterfaceCheck(ca_name);

		if(func)
		{
			#ifdef _DEBUG
				uint32 result;

				double start,end;

				start=GetDoubleTime();

				if(func->ImageDecompress)
					result=func->ImageDecompress(data,buffer,size,width,height,channels);
				else
				if(func->Compress)
					result=func->Decompress(buffer,size,data,width*height*channels);

				end=GetDoubleTime();

//				PutInfo(u"图像解压<%dx%dx%d>：%s算法，原始%u字节,解压后%u字节，%lf秒",width,height,channels,ca_name.wc_str(),size,result,double(end-start));

				return(result);
			#else

			if(func->ImageDecompress)
				return func->ImageDecompress(data,buffer,size,width,height,channels);
			else
			if(func->Compress)
				return func->Decompress(buffer,size,data,width*height*channels);

			#endif//
		}

		return(0);
	}
//--------------------------------------------------------------------------------------------------
/*	CompressStream::CompressStream(const UTF16String &ca_name,float level)
	{
		CompressPlugInInterface *func=CompressInterfaceCheck(ca_name);
		{
			if(func&&func->CreateCompressStream==nullptr)
			{
				PutInfo(u"压缩算法<%d>不支持流式压缩",CAName[ca_name]);
			}
			else
				func->CreateCompressStream(level);
		}
		else
			stream=nullptr;
	}*/
}
