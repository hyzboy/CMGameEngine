#ifndef HGL_IO_SEEK_ACCESS_INCLUDE
#define HGL_IO_SEEK_ACCESS_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
	namespace io
	{
		enum_int(SeekOrigin)		/// 资源偏移方向枚举
		{
			soBegin=0,				///<从资源最开始处开始，offset必须大于0。移到资源的offset位置
			soCurrent,				///<从资源当前位置开始，移到资源的Position+offset位置
			soEnd					///<从资源的结束位置开始，offset必须小于0，表示结束前的字符数
		};//enum SeekOrigin

		/**
		* 定位访问功能基类
		*/
		class SeekAccess
		{
		public:

			virtual ~SeekAccess()=default;

			virtual bool	CanRestart()const=0;													///<是否可以复位
			virtual bool	CanSeek()const=0;														///<是否可以定位
			virtual bool	CanSize()const=0;														///<是否可以取得尺寸

			virtual bool	Restart()=0;															///<复位访问指针
			virtual int64	Seek(int64,SeekOrigin=soBegin)=0;										///<移动访问指针
			virtual int64	Tell()const=0;															///<返回当前访问位置
			virtual int64	GetSize()const=0;														///<取得文件长度
		};//class SeekAccess
	}//namespace io
}//namespace hgl
#endif//HGL_IO_SEEK_ACCESS_INCLUDE
