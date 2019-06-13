#ifndef HGL_GRAPH_TEXTURE_BASE_CONTROL_INCLUDE
#define HGL_GRAPH_TEXTURE_BASE_CONTROL_INCLUDE

#include<hgl/type/DataType.h>
namespace hgl
{
    namespace graph
    {
        class TextureBaseControl
        {
        public:

            uint type,binding_type,texture_id;

        public:

            TextureBaseControl(uint t,uint bt,uint id)
            {
                type=t;
                binding_type=bt;
                texture_id=id;
            }

        public:     //mipmaps

            virtual void    GenMipmaps      ()=0;                                                   ///<生成mipmaps
            virtual void    GetMipmapLevel  (int &,int &)=0;                                        ///<取得贴图mipmaps级别
        };//class TextureBaseControl

        using HGL_TBC_CREATE=TextureBaseControl * (*)(uint t,uint bt);

        extern HGL_TBC_CREATE TextureBaseControlCreate;
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_BASE_CONTROL_INCLUDE
