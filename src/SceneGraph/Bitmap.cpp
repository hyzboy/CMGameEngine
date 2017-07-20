#include<hgl/graph/Bitmap.h>
#include<hgl/PlugIn.h>
#include<string.h>

namespace hgl
{
    namespace graph
    {
        void L2LA(uint8 *tar,uint8 *src,uint8 *tcolor,uint32 size)
        {
            int n=size;

            while(n--)
            {
                if(*src==*tcolor)
                {
                    *tar++=*src++;

                    *tar++=0;
                }
                else
                {
                    *tar++=*src++;
                    *tar++=255;
                }
            }
        }

        void RGB2RGBA(uint8 *tar,uint8 *src,uint8 *tcolor,uint32 size)
        {
            int n=size;

            while(n--)
            {
                if(memcmp(src,tcolor,3)==0)
                {
                    *tar++=*src++;
                    *tar++=*src++;
                    *tar++=*src++;
                    *tar++=0;
                }
                else
                {
                    *tar++=*src++;
                    *tar++=*src++;
                    *tar++=*src++;
                    *tar++=255;
                }
            }
        }

        void RGBA2RGB(uint8 *rgba,uint8 *rgb,uint32 size)
        {
            int n=size;

            while(n--)
            {
                *rgb++=*rgba++;
                *rgb++=*rgba++;
                *rgb++=*rgba++;
                rgba++;
            }
        }

        void RGBA2RGB_A(uint8 *rgba,uint8 *rgb,uint8 *a,uint32 size)
        {
            int n=size;

            while(n--)
            {
                *rgb++=*rgba++;
                *rgb++=*rgba++;
                *rgb++=*rgba++;
                *a++=*rgba++;
            }
        }

        void RGB_A2RGBA(uint8 *rgba,uint8 *rgb,uint8 *a,uint32 size)
        {
            int n=size;

            while(n--)
            {
                *rgba++=*rgb++;
                *rgba++=*rgb++;
                *rgba++=*rgb++;
                *rgba++=*a++;
            }
        }

        bool CheckMono(uint8 *data,uint bytes,uint width,uint height)
        {
            if(width==0||height==0||data==0||bytes==0)return(false);

            uint count=width*height;

            while(count--)
            {
                if((*(data)==*(data+1))&&(*(data)==*(data+2)))
                    data+=bytes;
                else
                    return(false);
            }

            return(true);
        }

        void *ConvertToMono(uint8 *data,uint bytes,uint width,uint height)
        {
            if(width==0||height==0||data==0||bytes==0)return(nullptr);

            uint count=width*height;
            uint8 *target;
            uint8 *result=target=new uint8[count*(bytes-2)];

            while(count--)
            {
                double r=*data++;
                double g=*data++;
                double b=*data++;

                double lum=r*0.299+g*0.587+b*0.114;

                if(lum<0)*target++=0;else
                if(lum>255)*target++=255;else
                 *target++=lum;

                if(bytes==4)
                    *target++=*data++;
            }

            return(result);
        }

        int CheckAlpha(uint8 *data,uint bytes,uint width,uint height)
        {
            if(width==0||height==0||data==0||bytes==0)return(0);

            uint count=width*height;
            uint8 *p=data+(bytes-1);
            bool all_zero=true;
            bool all_one=true;

            while(count--)
            {
                if(*p!=0)all_zero=false;
                if(*p!=255)all_one=false;

                if(!all_zero&&!all_one)return(-1);

                p+=bytes;
            }

            if(all_zero)return(0);
            if(all_one)return(1);

            return(-1);
        }

        uint8 *ClearAlpha(uint8 *data,uint bytes,uint width,uint height)
        {
            if(width==0||height==0||data==0||bytes==0)return(0);

            uint sgap=bytes;
            uint tgap=sgap-1;

            uint count=width*height;
            uint8 *target;
            uint8 *result=target=new uint8[count*tgap];

            while(count--)
            {
                memcpy(target,data,tgap);

                target+=tgap;
                data+=sgap;
            }

            return(result);
        }

/*        void ClearColor(uchar *data,uint total,uint color)
        {
            if(color!=ctLuminanceAlpha&&color!=ctRGBA)return;

            uchar *p =data;
            uchar *ap=data+color-1;

            while(total--)
            {
                if(*ap==0)
                    memset(p,0,color-1);

                p+=color;
                ap+=color;
            }
        }

        void ClearColor(uchar *dst,uchar *src,uint total,uint color)
        {
            memcpy(dst,src,total*color);

            ClearColor(dst,total,color);
        }*/

        void SplitToChannels(uint8 *out,uint8 *in,int size,int number)
        {
            for(int i=0;i<number;i++)
            {
                int n=size;
                uint8 *sp=in+i;
                uint8 *tp=out+i*size;

                while(n--)
                {
                    *tp++=*sp;
                    sp+=number;
                }
            }
        }

        void MergeChannels(uint8 *out,uint8 *in,int size,int number)
        {
            for(int i=0;i<number;i++)
            {
                int n=size;
                uint8 *tp=out+i;

                while(n--)
                {
                    *tp=*in++;
                    tp+=number;
                }
            }
        }
        
//         bool SaveToTGA(const char *filename,void *data,const uint16 width,const uint16 height,const uint8 bit,const bool v_flip)
//         {
//         #pragma pack(push,1)
//             
//             struct TGAHeader
//             {
//                 uint8 id;
//                 uint8 color_map_type;
//                 uint8 image_type;               // 1 colormap image ,2 true-color,3 grayscale
//                 
//                 uint16 color_map_first;
//                 uint16 color_map_length;
//                 uint8 color_map_size;
//                 
//                 uint16 x_origin;
//                 uint16 y_origin;
//                 
//                 uint16 width;
//                 uint16 height;
//                 uint8 bit;
//                 
//                 union
//                 {
//                     uint8 image_desc;
//                     struct
//                     {
//                         uint alpha_depth:4;
//                         uint reserved:1;
//                         uint direction:1;       //0 lower-left,1 upper left
//                     };
//                 };
//             };
//         #pragma pack(pop)
// 
//             int size;
// 
//             int fp=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0640);
// 
//             if(fp==-1)return(false);
//             
//             TGAHeader header;
//             
//             memset(&header,0,sizeof(TGAHeader));
//             
//             if(bit==8)
//                 header.image_type=3;
//             else
//                 header.image_type=2;
//             
//             header.width=width;
//             header.height=height;
//             header.bit=bit;
//             header.alpha_depth=8;
//             header.direction=v_flip?1:0;
// 
//             write(fp,&header,sizeof(TGAHeader));
//             write(fp,data,width*height*(bit>>3));
// 
//             close(fp);
//             
//             printf("Save Screen to %s\n",filename);
// 
//             return(true);
//         }
    }//namespace graph
}//namespace hgl
