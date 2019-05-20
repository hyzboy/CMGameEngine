#include<hgl/type/BaseString.h>
#include<hgl/Str.h>
#include<hgl/io/OutputStream.h>

namespace hgl
{
    namespace algorithm
    {
        namespace
        {
            constexpr uchar base64_chars[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789+/";

            const uchar base64_char_find(const uchar ch)
            {
                for(uchar i=0;i<sizeof(base64_chars);i++)
                    if(base64_chars[i]==ch)
                        return i;

                return 0;
            }
        }//namespace

        bool base64_encode(io::OutputStream *os,const uchar *input, size_t len)
        {
            int i = 0;
            int j = 0;
            uchar char_array_3[3];
            uchar char_array_4[4];
            uchar out[4];

            while (len--)
            {
                char_array_3[i++] = *(input++);

                if (i != 3)
                    continue;

                char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                                  ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                                  ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] =   char_array_3[2] & 0x3f;

                for(i = 0; i <4 ; i++)
                    out[i]=base64_chars[char_array_4[i]];

                if(os->Write(out,4)!=4)
                    return(false);

                i = 0;
            }

            if (i)
            {
                for(j = i; j < 3; j++)
                    char_array_3[j] = '\0';

                char_array_4[0] =  (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                                  ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                                  ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] =   char_array_3[2] & 0x3f;

                for (j = 0; j < i + 1; j++)
                    out[j]=base64_chars[char_array_4[j]];

                if(os->Write(out,i+1)!=i+1)
                    return(false);

                for(j=0;j<3-i;j++)
                    out[j]='=';

                if(os->Write(out,3-i)!=3-i)
                    return(false);
            }

            return(true);
        }

        bool base64_decode(io::OutputStream *os,const uchar *input,size_t in_len)
        {
            int i = 0;
            int j = 0;
            int in_ = 0;
            uchar char_array_4[4], char_array_3[3];

            while (in_len-- && ( input[in_] != '=') && isbase64(input[in_]))
            {
                char_array_4[i++] = input[in_]; in_++;

                if (i ==4)
                {
                    for (i = 0; i <4; i++)
                        char_array_4[i] = base64_char_find(char_array_4[i]);

                    char_array_3[0] =  (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    if(os->Write(char_array_3,3)!=3)
                        return(false);

                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j <4; j++)
                    char_array_4[j] = 0;

                for (j = 0; j <4; j++)
                    char_array_4[j] = base64_char_find(char_array_4[j]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                if(os->Write(char_array_3,i-1)!=i-1)
                    return(false);
            }

            return(true);
        }
    }//namespace algorithm
}//namespace hgl
