#include<hgl/graph/TileData.h>
#include<hgl/graph/Texture.h>
#include<hgl/graph/Render.h>

namespace hgl
{
	namespace graph
	{
		namespace
		{
			void AnalyseSize(int &fw,int &fh,int w,int h,int max)
			{
				int total,tw,th,t;

				fw=fh=0;

				tw=HGL_MAX_TEXTURE_SIZE;
				while(tw>=w)
				{
					th=HGL_MAX_TEXTURE_SIZE;
					while(th>=h)
					{
						t=(tw/w)*(th/h);

						if(!fw)
						{
							fw=tw;
							fh=th;

							total=t;
						}
						else
						{
							if(t==max)
							{
								//正好，就要这么大的

								fw=tw;
								fh=th;

								return;
							}
							else
							if(t>max)                       //要比要求的最大值大
							{
								if(t<total)            //找到最接近最大值的
								{
									//比现在选中的更节省
									fw=tw;
									fh=th;

									total=t;
								}
							}
						}

						th>>=1;
					}

					tw>>=1;
				}
			}
		}//namespace

		/**
		* 本类构造函数
		* @param w Tile宽度
		* @param h Tile高度
		* @param max_count 最大Tile数量,(注:程序会根据显卡能力来计算最适合的贴图，所以最大数量一般会大于指定值)
		* @param video_format Tile的显存格式
		*/
		TileData::TileData(int w,int h,int max_count,uint video_format)
		{
			int tw,th;

			AnalyseSize(tw,th,w,h,max_count);

			tile_rows=th/h;
			tile_cols=tw/w;

			tile_width	=w;
			tile_height	=h;
			tile_count	=0;
			tile_total	=tile_rows*tile_cols;

			tile_texture=new Texture2D(tw,th,video_format);

			//tile_object=new TileData::Object *[tile_total];
			//memset(tile_object,0,tile_total*sizeof(TileData::Object *));
			NEW_NULL_ARRAY(tile_object,TileData::Object *,tile_total);
		}

		TileData::~TileData()
		{
			delete tile_texture;

			SAFE_CLEAR_OBJECT_ARRAY(tile_object,tile_total);
		}

		int TileData::FindSpace()
		{
			if(!tile_object)return(-1);
			if(tile_count>=tile_total)return(-1);

			int n=tile_total;

			while(n--)
				if(!(tile_object[n]))
					return(n);

			LOG_PROBLEM(OS_TEXT("无法在Tile数据区内找到足够的空间！"));
			return(-1);
		}

		void TileData::WriteTile(int index,TileData::Object *obj,void *data,uint bytes,TextureSourceFormat format,int ctw,int cth)
		{
			int col,row;
			double left,top;

    		col=index%tile_cols;
			row=index/tile_cols;

			left=tile_width *col;
			top =tile_height*row;

			obj->index  =index;

			obj->width	=(ctw==-1)?tile_width:ctw;
			obj->height	=(cth==-1)?tile_height:cth;

			obj->fl=left/double(tile_texture->GetWidth());
			obj->ft=top /double(tile_texture->GetHeight());
			obj->fw=double(obj->width)/double(tile_texture->GetWidth());
			obj->fh=double(obj->height)/double(tile_texture->GetHeight());

			tile_object[index]=obj;

			tile_texture->ChangeImage(	left,
										top,
										tile_width,
										tile_height,
										data,
										bytes,
										format);
			//请保留这段代码，以便未来使用时该数据时不会使用
			//{
			//	vertex->Begin(index*6);
			//	texcoord->Begin(index*6);

			//		texcoord->WriteRect(obj->fl,obj->ft,obj->fw,obj->fh);
			//		vertex->WriteRect(0,0,obj->width,obj->height);

			//	texcoord->End();
			//	vertex->End();
			//}
		}

		/**
		* 增加一个Tile
		* @param data 图形原始数据
		* @param bytes 图形原始数据字节数
		* @param format 图形的色彩格式
		* @param ctw 当前tile宽度,-1表示等同全局设置
		* @param cth 当前tile高度,-1表示等同全局设置
		* @return 为增加的Tile创建的对象
		*/
		TileData::Object *TileData::Add(void *data,uint bytes,TextureSourceFormat format,int ctw,int cth)
		{
			if(!tile_object)return(nullptr);

			int index;

			index=FindSpace();
			if(index==-1)
			{
				LOG_PROBLEM(OS_TEXT("找不到空的Tile数据区!"));
				return(nullptr);
			}

   			TileData::Object *obj=new TileData::Object;

			WriteTile(index,obj,data,bytes,format,ctw,cth);

			tile_count++;
			return(obj);
		}

		/**
		* 删除一个Tile
		* @param obj 要删除的Tile的对象指针
		* @return 删除是否成功
		*/
		bool TileData::Delete(TileData::Object *obj)
		{
			if(!tile_object)return(false);

			if(tile_object[obj->index])
			{
				if(tile_object[obj->index]!=obj)
				{
					LOG_PROBLEM(OS_TEXT("要删除的TileData::Object和TileData中的不对应！"));
					return(false);
				}
				else
				{
					tile_object[obj->index]=nullptr;
					tile_count--;

					delete obj;

					return(true);
				}
			}
			else
			{
				LOG_PROBLEM(OS_TEXT("要删除的TileData::Object对象在TileData中不存在！"));
				return(false);
			}
		}

		/**
		* 更改一个Tile的数据内容
		* @param obj 要更改的Tile的对象指针
		* @param data 图形原始数据
		* @param bytes 图形原始数据字节数
		* @param format 图形的色彩格式
		* @param ctw 当前tile宽度,-1表示等同全局设置
		* @param cth 当前tile高度,-1表示等同全局设置
		* @return 更改是否成功
		*/
		bool TileData::Change(TileData::Object *obj,void *data,uint bytes,TextureSourceFormat format,int ctw,int cth)
		{
			if(!tile_object)return(false);

			if(tile_object[obj->index])
			{
				if(tile_object[obj->index]!=obj)
				{
					LOG_PROBLEM(OS_TEXT("要更改的TileData::Object和TileData中的不对应！"));
					return(false);
				}
				else
				{
            		WriteTile(obj->index,obj,data,bytes,format,ctw,cth);

					return(true);
				}
			}
			else
			{
				LOG_PROBLEM(OS_TEXT("要更改的TileData::Object对象在TileData中不存在！"));
				return(false);
			}
		}

		/**
		* 清除所有Tile数据
		*/
		void TileData::Clear()
		{
			if(!tile_object)return;

			int n=tile_total;

			while(n--)
				if(tile_object[n])
				{
					delete tile_object[n];
					tile_object[n]=nullptr;
				}
		}
	}//namespace graph
}//namespace hgl
