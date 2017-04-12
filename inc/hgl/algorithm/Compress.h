#ifndef HGL_ALGORITHM_COMPRESS_INCLUDE
#define HGL_ALGORITHM_COMPRESS_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
	/**
	* 压缩算法枚举<br>
	* 注意：请在任何情况下都使用算法名称，而不是数值来保存压缩算法的类型。
	*/
	enum_uint(CompressAlgorithm)		///压缩算法
	{
        caNone  =0,                     ///<不压缩

		caBzip2,                       	///<Bzip2算法
		caLzo,							///<Lzo算法
		caLzma,							///<Lzma算法
		caZip,							///<Zip算法
		caRLE,							///<RLE算法

		caRYC,							///<RYC算法(图像专用)
        caRSC,                          ///<RSC算法(图像专用)
		caJpeg2000,						///<Jpeg2000算法(图像专用)
		caVSRLE,						///<VSRLE算法(图像专用)

        caEnd                           ///<结束定义，无意义
	};

    typedef CompressAlgorithm CA;		///<压缩算法枚举定义

	const UTF16String CAToName(CA);																		///<压缩法算枚举到名称转换
	CA NameToCA(const UTF16String &);                       											///<名称到压缩算法枚举转换

	uint32 Compress(const UTF16String &,void *,uint32,void *,uint32,float=1);   						///<压缩数据
	uint32 Decompress(const UTF16String &,void *,uint32,void *,uint32); 								///<解压缩数据

	uint32 ImageCompress(const UTF16String &,void *,uint32,void *,int,int,int,float=1);					///<压缩图像数据
	uint32 ImageDecompress(const UTF16String &,void *,void *,uint32,int,int,int); 						///<解压缩图像数据

	/**
	* 压缩一个内存块的数据并存放到另一个内存块
	* @param ca 压缩算法
	* @param data 原始数据指针
	* @param size 原始数据的长度
	* @param compressdata 压缩后数据的存放位置
	* @param out_max 压缩后输出数据存放区大小
	* @param level 压缩级别(0:不压缩,1:最大压缩比)
	* @return 压缩后数据的长度
	* @see CompressAlgorithm
	* @see Decompress
	*/
	uint32 __inline Compress(CA ca,void *data,uint32 size,void *compressdata,uint32 out_max,float level=1)
	{
		return Compress(CAToName(ca),data,size,compressdata,out_max,level);
	}

	/**
	* 解压缩一个内存块的数据并存放到另一个内存块
	* @param ca 压缩算法
	* @param compressdata 压缩数据的存放位置
	* @param size 压缩数据的长度
	* @param out_data 解压缩后数据的存放位置
	* @param out_max 解压缩数据存放区的大小
	* @return 解压缩后的数据长度
	* @see CompressAlgorithm
	* @see Compress
	*/
	uint32 __inline Decompress(CA ca,void *compressdata,uint32 size,void *out_data,uint32 out_max)
	{
    	return Decompress(CAToName(ca),compressdata,size,out_data,out_max);
	}

	/**
	* 解压缩一个图像数据
	* @param ca 压缩算法
	* @param buffer 压缩后的数据存放缓冲区
	* @param buffer_size 缓冲区大小
	* @param data 原始的图像数据
	* @param width 图像宽
	* @param height 图像高
	* @param channels 图像通道数
	* @param level 压缩级别(0:不压缩,1:最大压缩比)
	* @return 压缩后的数据大小
	*/
	uint32 __inline ImageCompress(CA ca,void *buffer,uint32 buffer_size,void *data,int width,int height,int channels,float level=0)
	{
        return ImageCompress(CAToName(ca),buffer,buffer_size,data,width,height,channels,level);
	}

	/**
	* 解压缩一个图像数据
	* @param ca 压缩算法
	* @param data 解压后的数据存放处
	* @param buffer 压缩的数据指针
	* @param size 压缩数据的长度
	* @param width 图像宽
	* @param height 图像高
	* @param channels 图像通道数
	* @return 解压缩后的数据大小
	*/
	uint32 __inline ImageDecompress(CA ca,void *data,void *buffer,uint32 size,int width,int height,int channels)
	{
    	return ImageDecompress(CAToName(ca),data,buffer,size,width,height,channels);
	}

	/**
	* 流式数据压缩类
	*/
	class CompressStream                                                                            ///流式数据压缩类
	{
		void *stream;

	public:

		CompressStream(const UTF16String &,float=1);
		CompressStream(CA);
		~CompressStream();

		uint Compress(void *,uint &,bool,void *,uint &);
	};

	/**
	* 流式数据解压类
	*/
	class DecompressStream                                                                          ///流式数据解压类
	{
		void *stream;

	public:

		DecompressStream(const UTF16String &);
		DecompressStream(CA);
		~DecompressStream();

		uint Decompress(void *,uint &,void *,uint &);
	};
}//namespace hgl
#endif//HGL_ALGORITHM_COMPRESS_INCLUDE
