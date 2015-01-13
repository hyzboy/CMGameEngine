#ifndef HGL_GRAPH_RENDERABLE_INCLUDE
#define HGL_GRAPH_RENDERABLE_INCLUDE

#include<hgl/graph/Shader.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/ColorFormat.h>
namespace hgl
{
	namespace graph
	{
		enum ColorFormat;

		/**
		* 顶点缓冲区类型描述
		*/
		enum VertexBufferType:uint
		{
			vbtIndex=0,

			vbtVertex,
			vbtColor,
			vbtNormal,
			vbtSecondColor,
			vbtFogCoord,

			vbtAmbient,				//环境光颜色
			vbtSpecular,			//镜面光颜色

			vbtDiffuseTexCoord,		//漫反射纹理坐标
			vbtAlphaTexCoord,		//透明度纹理坐标
			vbtNormalTexCoord,		//法线纹理坐标
			vbtAmbientTexCoord,		//环境光颜色纹理坐标
			vbtSpecularTexCoord,	//镜面光颜色纹理坐标

			vbtTexCoord0,	vbtTexCoord1,	vbtTexCoord2,	vbtTexCoord3,	vbtTexCoord4,	vbtTexCoord5,	vbtTexCoord6,	vbtTexCoord7,
			vbtTexCoord8,	vbtTexCoord9,	vbtTexCoord10,	vbtTexCoord11,	vbtTexCoord12,	vbtTexCoord13,	vbtTexCoord14,	vbtTexCoord15,
			vbtTexCoord16,	vbtTexCoord17,	vbtTexCoord18,	vbtTexCoord19,	vbtTexCoord20,	vbtTexCoord21,	vbtTexCoord22,	vbtTexCoord23,
			vbtTexCoord24,	vbtTexCoord25,	vbtTexCoord26,	vbtTexCoord27,	vbtTexCoord28,	vbtTexCoord29,	vbtTexCoord30,	vbtTexCoord31,

			vbtEnd
		};//enum VertexBufferType

		extern int HGL_MAX_VERTEX_ATTRIBS;					///<最大顶点属性数量

		extern const char VertexBufferName[vbtEnd][32];		///<顶点缓冲区类型名称字串

		//可绘制的图元
		#define HGL_PRIM_POINTS						GL_POINTS					///<点
		#define HGL_PRIM_LINES						GL_LINES					///<线
		#define HGL_PRIM_LINE_STRIP					GL_LINE_STRIP				///<连续线
		#define HGL_PRIM_LINE_LOOP					GL_LINE_LOOP				///<线圈
		#define HGL_PRIM_TRIANGLES					GL_TRIANGLES				///<三角形
		#define HGL_PRIM_TRIANGLE_STRIP				GL_TRIANGLE_STRIP			///<三角形条
		#define HGL_PRIM_TRIANGLE_FAN				GL_TRIANGLE_FAN				///<扇形
		#define HGL_PRIM_LINES_ADJACENCY			GL_LINES_ADJACENCY			///<代表一個有四個頂點的Primitive,其中第二個點與第三個點會形成線段,而第一個點與第四個點則用來提供2,3鄰近點的資訊.
		#define HGL_PRIM_LINE_STRIP_ADJACENCY		GL_LINE_STRIP_ADJACENCY		///<與GL_LINES_ADJACENCY類似,第一個點跟最後一個點提供資訊,剩下的點則跟Line Strip一樣形成線段.
		#define HGL_PRIM_TRIANGLES_ADJACENCY		GL_TRIANGLES_ADJACENCY		///<代表一個有六個頂點的Primitive,其中第1,3,5個點代表一個Triangle,而地2,4,6個點提供鄰近資訊.(由1起算)
		#define HGL_PRIM_TRIANGLE_STRIP_ADJACENCY	GL_TRIANGLE_STRIP_ADJACENCY	///<4+2N個Vertices代表N個Primitive,其中1,3,5,7,9...代表原本的Triangle strip形成Triangle,而2,4,6,8,10...代表鄰近提供資訊的點.(由1起算)
		#define HGL_PRIM_PATCHES					GL_PATCHES

		/**
		* 可渲染数据
		*/
		class Renderable
		{
		protected:

			Material *material;																							///<材质
			bool mtl_private;																							///<材质是否私有

			VertexBufferBase *vertex_buffer[vbtEnd];																	///<顶点数据缓冲区

			VertexBufferType TextureChannels[mtcMax];																	///<贴图通道对应表

			ColorFormat vb_color_format;																				///<颜色顶点属性格式

			Shader *shader;																								///<着色程序

			AABB BoundingBox;																							///<绑定盒

			uint prim;																									///<绘制的图元类型

			int DrawStart,DrawCount;																					///<可绘制数量

		public:

			Renderable();					//请通过CreateRenderable来创建可渲染对像,函数在Render.H中定义
			virtual ~Renderable();

		public:

			virtual void						SetPrimitive		(uint dp)					{prim=dp;}									///<设置的绘制的图元类型
			virtual uint						GetPrimitive		()const						{return prim;}								///<取得要绘制的图元类型

			virtual void						SetMaterial			(Material *,bool);														///<设置材质数据
			virtual Material * 					GetMaterial			()const						{return material;}							///<取得材质指针

//			virtual void						SetShader			(Shader *s)					{shader=s;}									///<设置着色程序
			virtual Shader *					GetShader			()const						{return shader;}							///<取得着色程序

			/**
			* 自动创建生成shader
			* @param mvp 渲染时是否使用projection矩阵与modelview矩阵
			* @return 创建好的shader程序
			*/
			virtual Shader *					AutoCreateShader	(bool mvp=true															///<自动创建着色程序
#ifdef _DEBUG
																	,const u16char *filename=nullptr
#endif//_DEBUG
			)=0;

			virtual bool						SetVertexBuffer		(VertexBufferType,VertexBufferBase *);									///<设置顶点缓冲区数据
			virtual bool						ClearVertexBuffer	(VertexBufferType);														///<清除顶点缓冲区数据
			virtual VertexBufferBase *			GetVertexBuffer		(VertexBufferType);														///<取得顶点缓冲区数据

			virtual bool						SetDrawCount		(int,int);																///<设置要绘制的数据数量
			virtual void						GetDrawCount		(int &,int &);															///<取得指定的要绘制的数据数量
			virtual int							GetDrawCount		();																		///<取得可绘制的数据总数量

			virtual void						SetVertexColorFormat(ColorFormat fmt)	{vb_color_format=fmt;}								///<设置颜色顶点属性格式
			virtual ColorFormat					GetVertexColorFormat()const				{return vb_color_format;}							///<取得颜色顶点属性格式

			virtual bool						SetTexCoord			(int mtc,VertexBufferType);												///<设定贴图坐标对应缓冲区
					bool						SetTexCoord			(int mtc,VertexBufferType vbt,VertexBufferBase *vb)						///<设定贴图坐标对应缓冲区
			{
				if(!SetTexCoord(mtc,vbt))return(false);

				return SetVertexBuffer(vbt,vb);
			}

			virtual VertexBufferBase *			GetTexCoord			(int mtc,VertexBufferType *vbt=0);										///<取得贴图坐标对应的缓冲区

			virtual void						SetBoundingBox		(const AABB &box)	{BoundingBox=box;}									///<设置绑定盒
			virtual void						GetBoundingBox		(AABB &box)		{box=BoundingBox;}										///<取得绑定盒

			virtual const Vector3f 				GetCenter			()const																	///<取得中心点
			{
				Vector3f result;

				result.x=(BoundingBox.minPoint.x+BoundingBox.maxPoint.x)/2.0f;
				result.y=(BoundingBox.minPoint.y+BoundingBox.maxPoint.y)/2.0f;
				result.z=(BoundingBox.minPoint.z+BoundingBox.maxPoint.z)/2.0f;

				return result;
			}

			virtual void						GetMinMaxVertex		(Vector3f &min_v,Vector3f &max_v)										///<取得最小顶点和最大顶点
			{
				min_v=POINT_TO_FLOAT3(BoundingBox.minPoint);
				max_v=POINT_TO_FLOAT3(BoundingBox.maxPoint);
			}

			template<typename T>
					bool						SetVertex			(VertexBuffer3<T> *vb)													///<设定顶点数据
			{
				if(!vb)return(false);

				Vector3f min_v,max_v,len;

				vb->GetBoundingBox(min_v,max_v);

				BoundingBox.minPoint=POINT_VEC(min_v);
				BoundingBox.maxPoint=POINT_VEC(max_v);

				return SetVertexBuffer(vbtVertex,vb);
			}

			template<typename T>
					bool						SetVertex			(VertexBuffer2<T> *vb){return SetVertexBuffer(vbtVertex,vb);}			///<设定顶点数据

			virtual bool						SetIndex			(VertexBufferBase *vb){return SetVertexBuffer(vbtIndex,vb);}			///<设置渲染数据索引
			virtual bool						SetColor			(VertexBufferBase *vb,ColorFormat cf)									///<设置顶点颜色数据
			{
				if(!SetVertexBuffer(vbtColor,vb))
					return(false);

				SetVertexColorFormat(cf);
				return(true);
			}
			virtual bool						SetNormal			(VertexBufferBase *vb){return SetVertexBuffer(vbtNormal,vb);}			///<设置渲染顶点法线数据
			virtual bool						SetSecondColor		(VertexBufferBase *vb){return SetVertexBuffer(vbtSecondColor,vb);}		///<设置顶点第二颜色数据
			virtual bool						SetFogCoord			(VertexBufferBase *vb){return SetVertexBuffer(vbtFogCoord,vb);}			///<设置顶点雾数据
		};//class Renderable
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDERABLE_INCLUDE
