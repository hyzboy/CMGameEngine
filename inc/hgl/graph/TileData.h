#ifndef HGL_GRAPH_TILE_DATA_INCLUDE
#define HGL_GRAPH_TILE_DATA_INCLUDE

#include<hgl/graph/TextureFormat.h>
namespace hgl
{
    namespace graph
    {
        class Texture2D;
        class Renderable;
        class Bitmap2D;

        /**
        * TileData是一种处理将大量等同贴图的管理机制，程序会自动根据显卡最大贴图处理能力来创建尽可能符合的贴图。(注意:Tile的宽高不必是2的幂)。<br>
        * Tile的增加删除，程序会做自动排序，尽可能小的影响效能。
        */
        class TileData                                                                              ///Tile数据管理
        {
        public:

            struct Object                                                                           ///Tile对象
            {
                int index;

                double fl,ft;
                double fw,fh;

                int width,height;
            };

        protected:

            Texture2D *tile_texture;                                                                ///<Tile所用贴图

            TileData::Object **tile_object;                                                         ///<所有Tile对象指针

            int tile_width,tile_height;                                                             ///<tile的宽和高
            int tile_count,tile_total;                                                              ///<当前tile数量与最大数量
            int tile_rows,tile_cols;                                                                ///<贴图中可用的tile行数和列数

        protected:

            int FindSpace();                                                                        ///<寻找一个空位
            void WriteTile(int,TileData::Object *,void *,unsigned int,TextureSourceFormat,int,int); ///<写入一个Tile

        public:

            int         GetWidth    ()const{return tile_width;}                                     ///<取得Tile宽
            int         GetHeight   ()const{return tile_height;}                                    ///<取得Tile高
            int         GetCount    ()const{return tile_count;}                                     ///<取得Tile数量
            int         GetMaxCount ()const{return tile_total;}                                     ///<取得Tile最大数量
            int         GetFreeCount()const{return tile_total-tile_count;}                          ///<取得空余Tile数量

            Texture2D * GetTexture  ()const{return tile_texture;}                                   ///<取得贴图

        public:

            TileData(int,int,int, TextureSourceFormat);
            virtual ~TileData();

            TileData::Object *Add(void *,unsigned int,TextureSourceFormat,int=-1,int=-1);           ///<增加一个Tile

            bool Delete(TileData::Object *);                                                        ///<删除一个Tile
            bool Change(TileData::Object *,void *,unsigned int,TextureSourceFormat,int=-1,int=-1);  ///<更改一个Tile的数据内容
            void Clear();                                                                           ///<清除Tile数据
        };//class TileData

        template<typename T> class VertexBuffer2;

        /**
        * 渲染Tile为一个2D矩形数据到顶点缓冲区上
        * @param obj 要渲制的Tile对象
        * @param vertex 渲染到的2d顶点坐标缓冲区
        * @param tex_coord 渲染到的贴图坐标缓冲区
        * @param left 显示的左边界
        * @param top 显示的上边界
        * @param scale_width 宽度缩放比
        * @param scale_height 高度缩放比
        */
        template<typename T1,typename T2>
        __inline void RenderToVB2DRect(         VertexBuffer2<T1> *vertex,
                                                VertexBuffer2<T2> *tex_coord,
                                        const   TileData::Object *obj,
                                        const   float left,
                                        const   float top,
                                        const   float scale_width=1.0f,
                                        const   float scale_height=1.0f)
        {
            if(!obj||!vertex||!tex_coord)return;

            tex_coord->WriteRect(   obj->fl,
                                    obj->ft,
                                    obj->fw,
                                    obj->fh);

            vertex->WriteRect(  left,
                                top,
                                scale_width*float(obj->width),
                                scale_height*float(obj->height));
        }
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TILE_DATA_INCLUDE
