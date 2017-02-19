#include"TextureBaseControl.h"

namespace hgl
{
	namespace graph
	{
        static HGL_TBC_CREATE TextureBaseControlCreate=nullptr;

        TextureBaseControl *TextureBaseControlCreateDSA(uint t,uint bt);
        TextureBaseControl *TextureBaseControlCreateBind(uint t,uint bt);

        void InitTextureBaseControlDSA()
        {
            TextureBaseControlCreate=TextureBaseControlCreateDSA;
        }

        void InitTextureBaseControlBind()
        {
            TextureBaseControlCreate=TextureBaseControlCreateBind;
        }
    }//namespace graph
}//namespace hgl
