#ifndef HGL_GRAPH_SAMPLER_OBJECT_INCLUDE
#define HGL_GRAPH_SAMPLER_OBJECT_INCLUDE

#include<hgl/graph/TextureFormat.h>
#include<hgl/graph/GL/glew.h>
namespace hgl
{
    namespace graph
    {
        class SamplerObject
        {
            static float max_texture_max_anistropy;
            static float default_max_anistropy;
            static float default_lod_bias;

            friend void InitSamplerObject(float,float);

        protected:

            uint    id;

            int     min_filter;
            int     mag_filter;

            float   max_anisotropy;

            float   lod_bias;

        public:

            SamplerObject()
            {
                glGenSamplers(1,&id);

                min_filter=HGL_FILTER_LINEAR;
                mag_filter=HGL_FILTER_LINEAR;

                max_anisotropy=default_max_anistropy;

                lod_bias =0;
            }

            ~SamplerObject()
            {
                glDeleteSamplers(1,&id);
            }

            void SetMinFilter(int mf)
            {
                if(min_filter==mf)return;

                min_filter=mf;

                glSamplerParameteri(id,GL_TEXTURE_MIN_FILTER,min_filter);
            }

            void SetMagFilter(int mf)
            {
                if (mag_filter == mf)return;

                mag_filter = mf;

                glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
            }

            void SetFilter(int min_f, int mag_f)
            {
                if (min_filter != min_f)
                {
                    min_filter = min_f;

                    glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, min_filter);
                }

                if (mag_filter != mag_f)
                {
                    mag_filter=mag_f;

                    glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
                }
            }

            /**
            * 设置纹理最大各向异性过滤
            * @param ma 最大各向异性过滤值
            */
            void SetMaxAnisotropy(float ma)
            {
                if(max_anisotropy==ma)return;

                max_anisotropy=ma;

                glSamplerParameterf(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
            }

            /**
            * 直接使用最大纹理最大各向异性过滤
            */
            void UseMaxAnisotropy(float ma=1.0f)
            {
                SetMaxAnisotropy(max_texture_max_anistropy*ma);
            }

            void SetLodBias(float lb)
            {
                if(lod_bias ==lb)return;

                lod_bias=lb;

                glSamplerParameterf(id, GL_TEXTURE_LOD_BIAS,lod_bias);
            }
        };//class SamplerObject

        class SamplerObject1D:public SamplerObject
        {
        protected:

            int wrap;

        public:

            SamplerObject1D(){wrap=HGL_WRAP_REPEAT;}

            void SetWrap(int w)
            {
                if(wrap==w)return;

                wrap=w;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_S,wrap);
            }
        };//class SamplerObject1D:public SamplerObject

        class SamplerObject2D :public SamplerObject
        {
        protected:

            int wrap_s,wrap_t;

        public:

            SamplerObject2D() { wrap_s=wrap_t = HGL_WRAP_REPEAT; }

            void SetWrapS(int s)
            {
                if (wrap_s == s)return;

                wrap_s = s;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrap_s);
            }

            void SetWrapT(int t)
            {
                if (wrap_t == t)return;

                wrap_t = t;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrap_t);
            }

            void SetWrap(int s,int t)
            {
                if (wrap_s != s)
                {
                    wrap_s = s;

                    glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrap_s);
                }

                if (wrap_t != t)
                {
                    wrap_t = t;

                    glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrap_t);
                }
            }
        };//class SamplerObject2D :public SamplerObject

        class SamplerObject3D :public SamplerObject
        {
        protected:

            int wrap_s,wrap_t,wrap_r;

        public:

            SamplerObject3D() { wrap_s = wrap_t =wrap_r= HGL_WRAP_REPEAT; }

            void SetWrapS(int s)
            {
                if (wrap_s == s)return;

                wrap_s = s;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrap_s);
            }

            void SetWrapT(int t)
            {
                if (wrap_t == t)return;

                wrap_t = t;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrap_t);
            }

            void SetWrapR(int r)
            {
                if (wrap_r == r)return;

                wrap_r = r;

                glSamplerParameteri(id, GL_TEXTURE_WRAP_R, wrap_r);
            }

            void SetWrap(int s, int t,int r)
            {
                if (wrap_s != s)
                {
                    wrap_s = s;

                    glSamplerParameteri(id, GL_TEXTURE_WRAP_S, wrap_s);
                }

                if (wrap_t != t)
                {
                    wrap_t = t;

                    glSamplerParameteri(id, GL_TEXTURE_WRAP_T, wrap_t);
                }

                if (wrap_r != r)
                {
                    wrap_r = r;

                    glSamplerParameteri(id, GL_TEXTURE_WRAP_R, wrap_r);
                }
            }
        };//class SamplerObject3D :public SamplerObject
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_SAMPLER_OBJECT_INCLUDE
