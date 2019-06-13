#ifndef HGL_GRAPH_BITMAP_INCLUDE
#define HGL_GRAPH_BITMAP_INCLUDE

#include<hgl/graph/TextureSourceFormat.h>
#include<hgl/type/BaseString.h>
#include<hgl/io/FileSystem.h>
#include<hgl/LogInfo.h>
namespace hgl
{
    namespace graph
    {
#pragma pack(push,1)
        const uint32 TexFileFlagMagic=HGL_MERGE32('T','e','x',0x1A);

        struct TexFileHeader
        {
            uint32  flag;               ///<文件头标识
            uint8   version;            ///<版本号
            bool    mipmaps;            ///<是否有mipmaps
            char    format[16];         ///<显存格式
            uint32  image_bytes;        ///<数据字节数
            bool    use_color_key;      ///<是否有透明色
            uint8   color_key[3];       ///<透明色RGB
        };//struct TexFileHeader

        struct Tex1DFileHeader          :public TexFileHeader{uint32 length;                    };
        struct Tex2DFileHeader          :public TexFileHeader{uint32 width,height;              };
        struct Tex3DFileHeader          :public TexFileHeader{uint32 width,height,depth;        };
        struct Tex1DArrayFileHeader     :public TexFileHeader{uint32 length,count;              };
        struct Tex2DArrayFileHeader     :public TexFileHeader{uint32 width,height,layer;        };
        struct TexCubeMapFileHeader     :public TexFileHeader{uint32 width,height,faces;        };
        struct TexCubeMapArrayFileHeader:public TexFileHeader{uint32 width,height,faces,count;  };
#pragma pack(pop)

        /**
        * 位图数据文件格式基类
        */
        template<typename HEADER> class BitmapBase
        {
        protected:

            uint8 *data;

            HEADER *header;

            TextureSourceFormat format;

        public:

            TextureSourceFormat GetFormat       ()const{return format; }
            void *              GetData         ()const{return header?data+sizeof(HEADER):nullptr;}
            uint32              GetDataLength   ()const{return header?header->image_bytes:0;}

        public:

            BitmapBase()
            {
                data=nullptr;
                header=nullptr;
                format=HGL_SF_NONE;
            }

            BitmapBase(uint8 *ptr,uint size)
            {
                data=nullptr;
                header=nullptr;
                format=HGL_SF_NONE;

                Load(ptr,size);
            }

            virtual ~BitmapBase()
            {
                delete[] data;
            }

            virtual bool Load(uint8 *ptr, uint size)
            {
                delete[] data;

                if(!ptr||size<sizeof(HEADER))RETURN_FALSE;

                data=ptr;

                header=(HEADER *)data;

                if (header->flag != TexFileFlagMagic)
                {
                    delete[] data;

                    data=nullptr;
                    header=nullptr;

                    return(false);
                }

                format=GetTextureFormatEnum(header->format);

                return(true);
            }
        };//template<typename HEADER> class BitmapBase

        class Bitmap1D:public BitmapBase<Tex1DFileHeader>
        {
        public:

            using BitmapBase<Tex1DFileHeader>::BitmapBase;

            const uint32 GetLength  ()const{return header?header->length:0;}
        };//class Bitmap1D

        class Bitmap2D:public BitmapBase<Tex2DFileHeader>
        {
        public:

            using BitmapBase<Tex2DFileHeader>::BitmapBase;

            const uint32 GetWidth   ()const{return header?header->width :0;}
            const uint32 GetHeight  ()const{return header?header->height:0;}
        };//class Bitmap2D

        class Bitmap3D:public BitmapBase<Tex3DFileHeader>
        {
        public:

            using BitmapBase<Tex3DFileHeader>::BitmapBase;

            const uint32 GetWidth   ()const{return header?header->width :0;}
            const uint32 GetHeight  ()const{return header?header->height:0;}
            const uint32 GetDepth   ()const{return header?header->depth :0;}
        };//class Bitmap3D

        class Bitmap1DArray:public BitmapBase<Tex1DArrayFileHeader>
        {
        public:

            using BitmapBase<Tex1DArrayFileHeader>::BitmapBase;

            const uint32 GetLength  ()const{return header?header->length:0;}
            const uint32 GetCount   ()const{return header?header->count :0;}
        };//class Bitmap1DArray

        class Bitmap2DArray:public BitmapBase<Tex2DArrayFileHeader>
        {
        public:

            using BitmapBase<Tex2DArrayFileHeader>::BitmapBase;

            const uint32 GetWidth   ()const{return header?header->width :0;}
            const uint32 GetHeight  ()const{return header?header->height:0;}
            const uint32 GetLayer   ()const{return header?header->layer :0;}
        };//class Bitmap2DArray

        class BitmapCubeMap:public BitmapBase<TexCubeMapFileHeader>
        {
        public:

            using BitmapBase<TexCubeMapFileHeader>::BitmapBase;

            const uint32 GetWidth   ()const{return header?header->width :0;}
            const uint32 GetHeight  ()const{return header?header->height:0;}
            const uint32 GetFaces   ()const{return header?header->faces :0;}
        };//class BitmapCubeMap

        class BitmapCubeMapArray:public BitmapBase<TexCubeMapArrayFileHeader>
        {
        public:

            using BitmapBase<TexCubeMapArrayFileHeader>::BitmapBase;

            const uint32 GetWidth   ()const{return header?header->width :0;}
            const uint32 GetHeight  ()const{return header?header->height:0;}
            const uint32 GetFaces   ()const{return header?header->faces :0;}
            const uint32 GetCount   ()const{return header?header->count :0;}
        };//class BitmapCubeMapArray

        template<typename T> inline bool LoadBitmapFromFile(T *bmp,const OSString &filename)
        {
            if(!bmp||filename.IsEmpty())
                return(false);

            uint8 *filedata;
            int64 filesize=filesystem::LoadFileToMemory(filename,(void **)&filedata);

            return bmp->Load(filedata,filesize);
        }
    }//namespace graph

    namespace graph
    {
        void L2LA(uint8 *,uint8 *,uint8 *,uint32);
        void RGB2RGBA(uint8 *,uint8 *,uint8 *,uint32);
        void RGBA2RGB(uint8 *,uint8 *,uint32);
        void RGBA2RGB_A(uint8 *,uint8 *,uint8 *,uint32);

        bool CheckMono(uint8 *,uint,uint,uint);
        void *ConvertToMono(uint8 *,uint,uint,uint);

        int CheckAlpha(uint8 *,uint,uint,uint);
        uint8 *ClearAlpha(uint8 *,uint,uint,uint);

        //void ClearColor(uchar *,uint,uint);
        //void ClearColor(uchar *,uchar *,uint,uint);

        void SplitToChannels(uint8 *,uint8 *,int,int);

        bool SaveToTGA(const OSString &filename,void *data,const uint16 width,const uint16 height,const uint8 bit,const bool upper_to_bottom=true);
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_BITMAP_INCLUDE
