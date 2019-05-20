#ifndef HGL_VERTEX_BUFFER_BASE_INCLUDE
#define HGL_VERTEX_BUFFER_BASE_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    namespace graph
    {
        class VertexBufferControl;

        class VertexBufferBase
        {
            void *mem_data;																				///<内存中的数据

        protected:

            int dc_num;																					///<每个数据成员数(比如二维坐标为2、三维坐标为3)
            int count;																					///<数据个数

            int bytes;																					///<字节数

            void *mem_end;																				///<内存数据区访问结束地址

        protected:

            uint data_level;																			///<数据级别

            VertexBufferControl *vbc;                                                                   ///<顶点缓冲区控制器

        protected:

            void SetDataSize(int size);

        public:

            VertexBufferBase(uint level,uint size);
            virtual ~VertexBufferBase();

            virtual uint	GetDataType()const=0;														///<取得数据类型
            virtual int     GetDataBytes()const=0;														///<取得每数据字节数
                    int		GetComponent()const	{	return dc_num;	}									///<取数缓冲区元数据数量
                    int		GetCount()const		{	return count;	}									///<取得数据数量
                    int     GetStride()const    {   return dc_num*GetDataBytes();}                      ///<取得每一组数据字节数
                    void *	GetData()const		{	return mem_data;}									///<取得数据指针
                    int		GetBytes()const		{	return bytes;	}									///<取得数据字节数

        public:		//以下函数在各渲染器内部实现

                    bool	CreateVertexBuffer(uint type);
                    void	ChangeVertexBuffer(int,int,void *);
                    //void	BindVertexBuffer();
                    int     GetBufferIndex()const;                                                  ///<取得缓冲区索引
                    void	CloseVertexBuffer();
        };//class VertexBufferBase
    }//namespace graph
}//namespace hgl
#endif//HGL_VERTEX_BUFFER_BASE_INCLUDE
