#ifndef HGL_VERTEX_BUFFER_OBJECT_INCLUDE
#define HGL_VERTEX_BUFFER_OBJECT_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/Color3f.h>
#include<hgl/type/Color4f.h>
#include<hgl/Other.h>
#include<hgl/VectorMath.h>
#include<glew/include/GL/glew.h>
namespace hgl
{
	namespace graph
	{
		//顶点缓冲区认可的几种格式
		#define HGL_BYTE					GL_BYTE
		#define HGL_UNSIGNED_BYTE			GL_UNSIGNED_BYTE
		#define HGL_SHORT					GL_SHORT
		#define HGL_UNSIGNED_SHORT			GL_UNSIGNED_SHORT
		#define HGL_INT						GL_INT
		#define HGL_UNSIGNED_INT			GL_UNSIGNED_INT
		#define HGL_HALF_FLOAT				GL_HALF_FLOAT
		#define HGL_FLOAT					GL_FLOAT
		#define HGL_DOUBLE					GL_DOUBLE

		#define HGL_STREAM_DRAW				GL_STREAM_DRAW	//流式绘制，数据很少变化
		#define HGL_STATIC_DRAW				GL_STATIC_DRAW	//静态绘制，数据基本不会变化
		#define HGL_DYNAMIC_DRAW			GL_DYNAMIC_DRAW	//动态绘制，数据频繁变化

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

			uint video_buffer_index;																	///<对应显存访问区
			uint video_buffer_type;																		///<对应显存访问区类型

		protected:

			void SetDataSize(int size)
			{
				if(bytes==size)return;

				bytes=size;

				if(mem_data)
					hgl_realloc(mem_data,size);
				else
					hgl_malloc(size);

				mem_end=((char *)mem_data)+size;
			}

		public:

			VertexBufferBase(uint level,uint size)
			{
				dc_num=0;

				bytes=size;

				mem_data=hgl_malloc(size);			//在很多情况下，hgl_malloc分配的内存是对齐的，这样有效率上的提升
				mem_end=((char *)mem_data)+size;

				data_level=level;

				video_buffer_index=0;
				video_buffer_type=0;
			}

			virtual ~VertexBufferBase()
			{
				CloseVertexBuffer();

				hgl_free(mem_data);
			}

			virtual uint	GetDataType()const=0;														///<取得数据类型
			virtual uint	GetDataBytes()const=0;														///<取得每数据字节数
					int		GetComponent()const	{	return dc_num;	}									///<取数缓冲区元数据数量
					int		GetCount()const		{	return count;	}									///<取得数据数量
					void *	GetData()const		{	return mem_data;}									///<取得数据指针
					int		GetBytes()const		{	return bytes;	}									///<取得数据字节数

		public:		//以下函数在各渲染器内部实现

					bool	CreateVertexBuffer(uint type);
					void	ChangeVertexBuffer(int,int,void *);
					void	BindVertexBuffer();
					void	CloseVertexBuffer();
		};//class VertexBufferBase

		/**
		* 顶点属性数据实际模板
		*/
		template<typename T,int C> class VertexBuffer:public VertexBufferBase
		{
		protected:

			T *mem_type;																				///<符合当前类型的地址
			T *access;																					///<当前访问地址

			T *start;																					///<访问起始地址

		public:

			VertexBuffer(int _size,const void *_data=nullptr,uint level=HGL_STATIC_DRAW):VertexBufferBase(level,C*_size*sizeof(T))
			{
				dc_num=C;

				count=_size;

				mem_type=(T *)GetData();
				access=0;
				start=0;

				if(_data)
					memcpy(mem_type,_data,bytes);
			}

			virtual ~VertexBuffer()HGL_DEFAULT_MEMFUNC;

			uint GetDataBytes()const
			{
				return sizeof(T);
			}

			void SetCount(int _count)
			{
				count=_count;

				SetDataSize(_count*C*sizeof(T));

				mem_type=(T *)GetData();
				access=0;
				start=0;
			}

			/**
			* 取得数据区地址
			* @param offset 从第几个数据开始访问
			* @return 访问地址
			*/
			T *Get(int offset=0)
			{
				if(!mem_type||offset>=count)
				{
					LOG_HINT(OS_TEXT("VertexBuffer::Get() out,offset:")+OSString(offset));
					return(nullptr);
				}

				return mem_type+offset*C;
			}

			/**
			* 开始访问数据区
			* @param offset 从第几个数据开始访问
			* @return 访问地址
			*/
			void *Begin(int offset=0)
			{
				if(access)
				{
					LOG_HINT(OS_TEXT("VertexBuffer::Begin() access!=0,offset:")+OSString(offset));
					return(nullptr);
				}

				access=Get(offset);

				if(access)
					start=access;

				return access;
			}

			/**
			* 结束访问
			*/
			void End()
			{
				ChangeVertexBuffer(	((char *)start )-((char *)mem_type),
									((char *)access)-((char *)start),
									start);

				access=nullptr;
				start=nullptr;
			}

			/**
			* 写入指定数量的数据
			* @param vp 数据指针
			* @param number 数据数量
			*/
			bool WriteData(const T *vp,const int number)
			{
				if(!this->access||this->access+C*number>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer::Write(const T *,number) out,number:")+OSString(number));
					return(false);
				}

				memcpy(access,vp,C*number*sizeof(T));

				access+=C*number;

				return(true);
			}
		};//class VertexBuffer

		/**
		* 一元数据缓冲区
		*/
		template<typename T> class VertexBuffer1:public VertexBuffer<T,1>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using VertexBuffer<T,1>::VertexBuffer;
#else
			VertexBuffer1(int _size,const void *_data=nullptr,uint level=HGL_STATIC_DRAW):VertexBuffer<T,1>(_size,_data,level){}
#endif//HGL_CONSTRUCTION_REUSE
			virtual ~VertexBuffer1()HGL_DEFAULT_MEMFUNC;

			uint	GetDataType()const;

			bool Write(const T v1)
			{
				if(!this->access||this->access+1>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer1::Write(const T) out"));
					return(false);
				}

				*this->access++=v1;
				return(true);
			}

			/**
			 * 将一个值重复多次写入缓冲区
			 * @param v 值
			 * @param count 写入数量
			 */
			bool Write(const T v,const int count)
			{
				if(!this->access||this->access+count>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer1::Write(const T,")+OSString(count)+OS_TEXT(") out"));
					return(false);
				}

				hgl_set(this->access,v,count);
				this->access+=count;
				return(true);
			}
		};//class VertexBuffer1

		/**
		* 二元数据缓冲区
		*/
		template<typename T> class VertexBuffer2:public VertexBuffer<T,2>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using VertexBuffer<T,2>::VertexBuffer;
#else
			VertexBuffer2(int _size,const void *_data=nullptr,uint level=HGL_STATIC_DRAW):VertexBuffer<T,2>(_size,_data,level){}
#endif//HGL_CONSTRUCTION_REUSE
			virtual ~VertexBuffer2()HGL_DEFAULT_MEMFUNC;

			uint	GetDataType()const;

			bool Write(const T v1,const T v2)
			{
				if(!this->access||this->access+2>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::Write(const T ,const T) out"));
					return(false);
				}

				*this->access++=v1;
				*this->access++=v2;

				return(true);
			}

			bool Write(const T *v)
			{
				if(!this->access||this->access+2>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::Write(T *) out"));
					return(false);
				}

				*this->access++=*v++;
				*this->access++=*v;

				return(true);
			}

			bool Write(const Vector2f &v)
			{
				if(!this->access||this->access+2>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::Write(vec2 &) out"));
					return(false);
				}

				*this->access++=v.x;
				*this->access++=v.y;

				return(true);
			}

			/**
			 * 将一个值重复多次写入缓冲区
			 * @param v 值
			 * @param count 写入数量
			 */
			bool Write(const Vector2f &v,const int count)
			{
				if(!this->access||this->access+(count<<1)>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer1::Write(const Vector2f &,")+OSString(count)+OS_TEXT(") out"));
					return(false);
				}

				for(int i=0;i<count;i++)
				{
					*this->access++=v.x;
					*this->access++=v.y;
				}

				return(true);
			}

			bool WriteLine(const T start_x,const T start_y,const T end_x,const T end_y)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::WriteLine(T,T,T,T) out"));
					return(false);
				}

				*this->access++=start_x;
				*this->access++=start_y;
				*this->access++=end_x;
				*this->access++=end_y;

				return(true);
			}

			bool WriteLine(const Vector2f &start,const Vector2f &end)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::WriteLine(vec2,vec2) out"));
					return(false);
				}

				*this->access++=start.x;
				*this->access++=start.y;
				*this->access++=end.x;
				*this->access++=end.y;

				return(true);
			}

			/**
			* 写入2D三角形
			*/
			bool WriteTriangle(const Vector2f &v1,const Vector2f &v2,const Vector2f &v3)
			{
				if(!this->access||this->access+6>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::WriteTriangle(vec2,vec2,vec2) out"));
					return(false);
				}

				*this->access++=v1.x;
				*this->access++=v1.y;

				*this->access++=v2.x;
				*this->access++=v2.y;

				*this->access++=v3.x;
				*this->access++=v3.y;

				return(true);
			}

			/**
			* 写入2D三角形
			*/
			bool WriteTriangle(const Vector2f *v)
			{
				if(!this->access||this->access+6>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer2::WriteTriangle(vec2 *) out"));
					return(false);
				}

				*this->access++=v->x;
				*this->access++=v->y;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;

				return(true);
			}

			/**
			* 写入2D四边形坐标数据
			*/
			bool WriteQuad(const Vector2f &lt,const Vector2f &rt,const Vector2f &rb,const Vector2f &lb)
			{
				if(WriteTriangle(lt,lb,rb))
				if(WriteTriangle(lt,rb,rt))
					return(true);

				LOG_HINT(OS_TEXT("VertexBuffer2::WriteQuad(vec2 &,vec2 &,vec2 &,vec2 &) error"));
				return(false);
			}

			/**
			* 写入2D矩形（两个三角形）坐标数据
			*/
			bool WriteRect(const T left,const T top,const T width,const T height)
			{
				const Vector2f lt(left		,top);
				const Vector2f rt(left+width,top);
				const Vector2f rb(left+width,top+height);
				const Vector2f lb(left		,top+height);

				return WriteQuad(lt,rt,rb,lb);
			}
		};//class VertexBuffer2

		/**
		* 三元数据缓冲区
		*/
		template<typename T> class VertexBuffer3:public VertexBuffer<T,3>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using VertexBuffer<T,3>::VertexBuffer;
#else
			VertexBuffer3(int _size,const void *_data=nullptr,uint level=HGL_STATIC_DRAW):VertexBuffer<T,3>(_size,_data,level){}
#endif//HGL_CONSTRUCTION_REUSE

			virtual ~VertexBuffer3()HGL_DEFAULT_MEMFUNC;

			uint	GetDataType()const;

			/**
			* 计算绑定盒
			* @param corner 一个顶角的坐标
			* @param lanegth 每条边的长
			*/
			void GetBoundingBox(Vector3f &min_vertex,Vector3f &max_vertex)
			{
				T *p=this->mem_type;

				//先以corner为最小值,length为最大值，求取最小最大值
				min_vertex.x=*p++;
				min_vertex.y=*p++;
				min_vertex.z=*p++;

				max_vertex=min_vertex;

				for(int i=1;i<this->count;i++)
				{
					if(*p<min_vertex.x)min_vertex.x=*p;
					if(*p>max_vertex.x)max_vertex.x=*p;
					p++;

					if(*p<min_vertex.y)min_vertex.y=*p;
					if(*p>max_vertex.y)max_vertex.y=*p;
					p++;

					if(*p<min_vertex.z)min_vertex.z=*p;
					if(*p>max_vertex.z)max_vertex.z=*p;
					p++;
				}
			}

			bool Write(const T v1,const T v2,const T v3)
			{
				if(!this->access||this->access+3>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::Write(T,T,T) out"));
					return(false);
				}

				*this->access++=v1;
				*this->access++=v2;
				*this->access++=v3;

				return(true);
			}

			bool Write(const T *v)
			{
				if(!this->access||this->access+3>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::Write(T *) out"));
					return(false);
				}

				*this->access++=*v++;
				*this->access++=*v++;
				*this->access++=*v;

				return(true);
			}

			bool Write(const Vector3f &v)
			{
				if(!this->access||this->access+3>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::Write(vec3 &) out"));
					return(false);
				}

				*this->access++=v.x;
				*this->access++=v.y;
				*this->access++=v.z;

				return(true);
			}

			/**
			 * 将一个值重复多次写入缓冲区
			 * @param v 值
			 * @param count 写入数量
			 */
			bool Write(const Vector3f &v,const int count)
			{
				if(!this->access||this->access+(count*3)>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::Write(const Vector3f,")+OSString(count)+OS_TEXT(") out"));
					return(false);
				}

				for(int i=0;i<count;i++)
				{
					*this->access++=v.x;
					*this->access++=v.y;
					*this->access++=v.z;
				}

				return(true);
			}

			bool Write(const Color3f &v)
			{
				if(!this->access||this->access+3>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::Write(color3f &) out"));
					return(false);
				}

				*this->access++=v.r;
				*this->access++=v.g;
				*this->access++=v.b;

				return(true);
			}

			bool WriteLine(const T start_x,const T start_y,const T start_z,const T end_x,const T end_y,const T end_z)
			{
				if(!this->access||this->access+6>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::WriteLine(T,T,T,T,T,T) out"));
					return(false);
				}

				*this->access++=start_x;
				*this->access++=start_y;
				*this->access++=start_z;
				*this->access++=end_x;
				*this->access++=end_y;
				*this->access++=end_z;

				return(true);
			}

			bool WriteLine(const Vector3f &start,const Vector3f &end)
			{
				if(!this->access||this->access+6>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::WriteLine(vec3,vec3) out"));
					return(false);
				}

				*this->access++=start.x;
				*this->access++=start.y;
				*this->access++=start.z;
				*this->access++=end.x;
				*this->access++=end.y;
				*this->access++=end.z;

				return(true);
			}

			/**
			* 写入3D三角形
			*/
			bool WriteTriangle(const Vector3f &v1,const Vector3f &v2,const Vector3f &v3)
			{
				if(!this->access||this->access+9>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::WriteTriangle(vec3,vec3,vec3) out"));
					return(false);
				}

				*this->access++=v1.x;
				*this->access++=v1.y;
				*this->access++=v1.z;

				*this->access++=v2.x;
				*this->access++=v2.y;
				*this->access++=v2.z;

				*this->access++=v3.x;
				*this->access++=v3.y;
				*this->access++=v3.z;

				return(true);
			}

			/**
			* 写入3D三角形
			*/
			bool WriteTriangle(const Vector3f *v)
			{
				if(!this->access||this->access+9>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer3::WriteTriangle(vec3 *) out"));
					return(false);
				}

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;

				return(true);
			}

			/**
			* 写入3D四边形坐标数据
			*/
			bool WriteQuad(const Vector3f &lt,const Vector3f &rt,const Vector3f &rb,const Vector3f &lb)
			{
				if(WriteTriangle(lt,lb,rb))
				if(WriteTriangle(lt,rb,rt))
					return(true);

				LOG_HINT(OS_TEXT("VertexBuffer3::WriteQuad(vec3 &,vec3 &,vec3 &,vec3 &) error"));
				return(false);
			}
		};//class VertexBuffer3

		/**
		* 四元数据缓冲区
		*/
		template<typename T> class VertexBuffer4:public VertexBuffer<T,4>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using VertexBuffer<T,4>::VertexBuffer;
#else
			VertexBuffer4(int _size,const void *_data=nullptr,uint level=HGL_STATIC_DRAW):VertexBuffer<T,4>(_size,_data,level){}
#endif//HGL_CONSTRUCTION_REUSE
			virtual ~VertexBuffer4()HGL_DEFAULT_MEMFUNC;

			uint	GetDataType()const;

			bool Write(const T v1,const T v2,const T v3,const T v4)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::Write(T,T,T,T) out"));
					return(false);
				}

				*this->access++=v1;
				*this->access++=v2;
				*this->access++=v3;
				*this->access++=v4;

				return(true);
			}

			bool Write(const T *v)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::Write(T *) out"));
					return(false);
				}

				*this->access++=*v++;
				*this->access++=*v++;
				*this->access++=*v++;
				*this->access++=*v;

				return(true);
			}

			bool Write(const Vector4f &v)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::Write(color4 &) out"));
					return(false);
				}

				*this->access++=v.x;
				*this->access++=v.y;
				*this->access++=v.z;
				*this->access++=v.w;

				return(true);
			}

			bool Write(const Color4f &v)
			{
				if(!this->access||this->access+4>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::Write(color4 &) out"));
					return(false);
				}

				*this->access++=v.r;
				*this->access++=v.g;
				*this->access++=v.b;
				*this->access++=v.a;

				return(true);
			}

			/**
			 * 将一个值重复多次写入缓冲区
			 * @param v 值
			 * @param count 写入数量
			 */
			bool Write(const Vector4f &v,const int count)
			{
				if(!this->access||this->access+(count<<2)>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::Write(const Vector4f,")+OSString(count)+OS_TEXT(") out"));
					return(false);
				}

				for(int i=0;i<count;i++)
				{
					*this->access++=v.x;
					*this->access++=v.y;
					*this->access++=v.z;
					*this->access++=v.w;
				}

				return(true);
			}

			bool WriteLine(const T start_x,const T start_y,const T start_z,const T end_x,const T end_y,const T end_z)
			{
				if(!this->access||this->access+8>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::WriteLine(T,T,T,T,T,T) out"));
					return(false);
				}

				*this->access++=start_x;
				*this->access++=start_y;
				*this->access++=start_z;
				*this->access++=1.0f;
				*this->access++=end_x;
				*this->access++=end_y;
				*this->access++=end_z;
				*this->access++=1.0f;

				return(true);
			}

			bool WriteLine(const Vector3f &start,const Vector3f &end)
			{
				if(!this->access||this->access+8>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::WriteLine(vec3,vec3) out"));
					return(false);
				}

				*this->access++=start.x;
				*this->access++=start.y;
				*this->access++=start.z;
				*this->access++=1.0f;
				*this->access++=end.x;
				*this->access++=end.y;
				*this->access++=end.z;
				*this->access++=1.0f;

				return(true);
			}

			/**
			* 写入3D三角形
			*/
			bool WriteTriangle(const Vector3f &v1,const Vector3f &v2,const Vector3f &v3)
			{
				if(!this->access||this->access+12>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::WriteTriangle(vec3,vec3,vec3) out"));
					return(false);
				}

				*this->access++=v1.x;
				*this->access++=v1.y;
				*this->access++=v1.z;
				*this->access++=1.0f;

				*this->access++=v2.x;
				*this->access++=v2.y;
				*this->access++=v2.z;
				*this->access++=1.0f;

				*this->access++=v3.x;
				*this->access++=v3.y;
				*this->access++=v3.z;
				*this->access++=1.0f;

				return(true);
			}

			/**
			* 写入3D三角形
			*/
			bool WriteTriangle(const Vector3f *v)
			{
				if(!this->access||this->access+12>this->mem_end)
				{
					LOG_HINT(OS_TEXT("VertexBuffer4::WriteTriangle(vec3 *) out"));
					return(false);
				}

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;
				*this->access++=1.0f;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;
				*this->access++=1.0f;
				v++;

				*this->access++=v->x;
				*this->access++=v->y;
				*this->access++=v->z;
				*this->access++=1.0f;

				return(true);
			}
		};//class VertexBuffer4

		//缓冲区具体数据类型定义
		typedef VertexBuffer1<int8	>	VB1i8	,VB1b;	template<> inline uint VertexBuffer1<int8	>::GetDataType()const{return HGL_BYTE;			}
		typedef VertexBuffer1<int16	>	VB1i16	,VB1s;	template<> inline uint VertexBuffer1<int16	>::GetDataType()const{return HGL_SHORT;			}
		typedef VertexBuffer1<int32	>	VB1i32	,VB1i;	template<> inline uint VertexBuffer1<int32	>::GetDataType()const{return HGL_INT;			}
		typedef VertexBuffer1<uint8	>	VB1u8	,VB1ub;	template<> inline uint VertexBuffer1<uint8	>::GetDataType()const{return HGL_UNSIGNED_BYTE;	}
		typedef VertexBuffer1<uint16>	VB1u16	,VB1us;	template<> inline uint VertexBuffer1<uint16	>::GetDataType()const{return HGL_UNSIGNED_SHORT;}
		typedef VertexBuffer1<uint32>	VB1u32	,VB1ui;	template<> inline uint VertexBuffer1<uint32	>::GetDataType()const{return HGL_UNSIGNED_INT;	}
		typedef VertexBuffer1<float	>	VB1f;			template<> inline uint VertexBuffer1<float	>::GetDataType()const{return HGL_FLOAT;			}
		typedef VertexBuffer1<double>	VB1d;			template<> inline uint VertexBuffer1<double	>::GetDataType()const{return HGL_DOUBLE;		}

		typedef VertexBuffer2<int8	>	VB2i8	,VB2b;	template<> inline uint VertexBuffer2<int8	>::GetDataType()const{return HGL_BYTE;			}
		typedef VertexBuffer2<int16	>	VB2i16	,VB2s;	template<> inline uint VertexBuffer2<int16	>::GetDataType()const{return HGL_SHORT;			}
		typedef VertexBuffer2<int32	>	VB2i32	,VB2i;	template<> inline uint VertexBuffer2<int32	>::GetDataType()const{return HGL_INT;			}
		typedef VertexBuffer2<uint8	>	VB2u8	,VB2ub;	template<> inline uint VertexBuffer2<uint8	>::GetDataType()const{return HGL_UNSIGNED_BYTE;	}
		typedef VertexBuffer2<uint16>	VB2u16	,VB2us;	template<> inline uint VertexBuffer2<uint16	>::GetDataType()const{return HGL_UNSIGNED_SHORT;}
		typedef VertexBuffer2<uint32>	VB2u32	,VB2ui;	template<> inline uint VertexBuffer2<uint32	>::GetDataType()const{return HGL_UNSIGNED_INT;	}
		typedef VertexBuffer2<float	>	VB2f;			template<> inline uint VertexBuffer2<float	>::GetDataType()const{return HGL_FLOAT;			}
		typedef VertexBuffer2<double>	VB2d;			template<> inline uint VertexBuffer2<double	>::GetDataType()const{return HGL_DOUBLE;		}

		typedef VertexBuffer3<int8	>	VB3i8	,VB3b;	template<> inline uint VertexBuffer3<int8	>::GetDataType()const{return HGL_BYTE;			}
		typedef VertexBuffer3<int16	>	VB3i16	,VB3s;	template<> inline uint VertexBuffer3<int16	>::GetDataType()const{return HGL_SHORT;			}
		typedef VertexBuffer3<int32	>	VB3i32	,VB3i;	template<> inline uint VertexBuffer3<int32	>::GetDataType()const{return HGL_INT;			}
		typedef VertexBuffer3<uint8	>	VB3u8	,VB3ub;	template<> inline uint VertexBuffer3<uint8	>::GetDataType()const{return HGL_UNSIGNED_BYTE;	}
		typedef VertexBuffer3<uint16>	VB3u16	,VB3us;	template<> inline uint VertexBuffer3<uint16	>::GetDataType()const{return HGL_UNSIGNED_SHORT;}
		typedef VertexBuffer3<uint32>	VB3u32	,VB3ui;	template<> inline uint VertexBuffer3<uint32	>::GetDataType()const{return HGL_UNSIGNED_INT;	}
		typedef VertexBuffer3<float	>	VB3f;			template<> inline uint VertexBuffer3<float	>::GetDataType()const{return HGL_FLOAT;			}
		typedef VertexBuffer3<double>	VB3d;			template<> inline uint VertexBuffer3<double	>::GetDataType()const{return HGL_DOUBLE;		}

		typedef VertexBuffer4<int8	>	VB4i8	,VB4b;	template<> inline uint VertexBuffer4<int8	>::GetDataType()const{return HGL_BYTE;			}
		typedef VertexBuffer4<int16	>	VB4i16	,VB4s;	template<> inline uint VertexBuffer4<int16	>::GetDataType()const{return HGL_SHORT;			}
		typedef VertexBuffer4<int32	>	VB4i32	,VB4i;	template<> inline uint VertexBuffer4<int32	>::GetDataType()const{return HGL_INT;			}
		typedef VertexBuffer4<uint8	>	VB4u8	,VB4ub;	template<> inline uint VertexBuffer4<uint8	>::GetDataType()const{return HGL_UNSIGNED_BYTE;	}
		typedef VertexBuffer4<uint16>	VB4u16	,VB4us;	template<> inline uint VertexBuffer4<uint16	>::GetDataType()const{return HGL_UNSIGNED_SHORT;}
		typedef VertexBuffer4<uint32>	VB4u32	,VB4ui;	template<> inline uint VertexBuffer4<uint32	>::GetDataType()const{return HGL_UNSIGNED_INT;	}
		typedef VertexBuffer4<float	>	VB4f;			template<> inline uint VertexBuffer4<float	>::GetDataType()const{return HGL_FLOAT;			}
		typedef VertexBuffer4<double>	VB4d;			template<> inline uint VertexBuffer4<double	>::GetDataType()const{return HGL_DOUBLE;		}
	}//namespace graph
}//namespace hgl
#endif//HGL_VERTEX_BUFFER_OBJECT_INCLUDE
