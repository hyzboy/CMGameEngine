#include"TextureBaseControl.h"

namespace hgl
{
	namespace graph
	{
        static HGL_TBC_CREATE TextureBaseControlCreate=nullptr;

        TextureBaseControl *TextureBaseControlCreateDSA(uint t,uint bt);
        TextureBaseControl *TextureBaseControlCreateBind(uint t,uint bt);

        void InitTextureBaseControl(bool use_dsa)
        {
            if(use_dsa)
                TextureBaseControlCreate=TextureBaseControlCreateDSA;
            else
                TextureBaseControlCreate=TextureBaseControlCreateBind;
        }
    }//namespace graph
}//namespace hgl
