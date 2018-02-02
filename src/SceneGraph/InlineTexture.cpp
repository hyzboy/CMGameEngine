#include<hgl/graph/Texture1D.h>
#include<hgl/graph/Texture2D.h>

namespace hgl
{
    namespace graph
    {
        Texture1D *WhiteLine        =nullptr;
        Texture1D *BlackWhiteLine   =nullptr;

        Texture2D *BlackWhiteGrid   =nullptr;
        Texture2D *GrayWhiteGrid    =nullptr;
        Texture2D *BlueWhiteGrid    =nullptr;

        void CreateInlineTexture()
        {
            {
                const uint8 white_line[4]={245,255,248,255};

                WhiteLine=CreateTexture1D(4,(void *)white_line,4,HGL_SF_R8,HGL_R8);
            }

            {
                const uint8 black_white_line[4]={255,0,255,0};

                BlackWhiteLine=CreateTexture1D(4,(void *)black_white_line,4,HGL_SF_R8,HGL_R8);
            }

            {
                const uint8 black_white_grid[16]={  0xFF,0xFF,0x00,0x00,
                                                    0xFF,0xFF,0x00,0x00,
                                                    0x00,0x00,0xFF,0xFF,
                                                    0x00,0x00,0xFF,0xFF};

                BlackWhiteGrid=CreateTexture2D(4,4,(void *)black_white_grid,16,HGL_SF_R8,HGL_R8);
            }

            {
                const uint8 gray_white_grid[16]={   0xFF,0xFF,0x7F,0x7F,
                                                    0xFF,0xFF,0x7F,0x7F,
                                                    0x7F,0x7F,0xFF,0xFF,
                                                    0x7F,0x7F,0xFF,0xFF};

                GrayWhiteGrid=CreateTexture2D(4,4,(void *)gray_white_grid,16,HGL_SF_R8,HGL_R8);
            }

            {
                const uint8 blue_white_grid[16][3]=
                {
                    255,255,255,    255,255,255,     61,174,233,     61,174,233,
                    255,255,255,    255,255,255,     61,174,233,     61,174,233,
                     61,174,233,     61,174,233,    255,255,255,    255,255,255,
                     61,174,233,     61,174,233,    255,255,255,    255,255,255
                };

                BlueWhiteGrid=CreateTexture2D(4,4,(void *)blue_white_grid,48,HGL_SF_RGB8,HGL_RGB8);
            }
        }

        void ClearInlineTexture()
        {
            SAFE_CLEAR(BlueWhiteGrid);
            SAFE_CLEAR(GrayWhiteGrid);
            SAFE_CLEAR(BlackWhiteGrid);

            SAFE_CLEAR(BlackWhiteLine);
            SAFE_CLEAR(WhiteLine);
        }
    }//namespace graph
}//namespace hgl
