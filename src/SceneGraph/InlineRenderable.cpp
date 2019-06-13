#include<hgl/graph/InlineRenderable.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/graph/VertexArray.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/Shader.h>
#include<hgl/graph/TextureFormat.h>

namespace hgl
{
    namespace graph
    {
        VertexArray *SolidCube      =nullptr;
        VertexArray *WireCube       =nullptr;

        VertexArray *SolidRect      =nullptr;
        VertexArray *WireRect       =nullptr;
        VertexArray *TextureRect    =nullptr;

        VertexArray *SolidCircle    =nullptr;
        VertexArray *WireCircle     =nullptr;
    }//namespace graph

    namespace graph
    {
        ///**
        //* 创建一个点的可渲染数据
        //* @param vertex 顶点坐标
        //* @param color 颜色
        //* @param size 点尺寸
        //* return 可渲染数据
        //*/
        //VertexArray *CreateRenderablePoint(const Vector3f &vertex,const Color4f &color,double size)
        //{
        //    VertexArray *obj=CreateRenderable();
        //    Material *mtl=CreateMaterial();

        //    //设置材质
        //    mtl->SetColor(color);                    //设置颜色

        //    obj->SetMaterial(mtl);                    //设置材质

        //    //设置图元类型
        //    obj->SetPrimitive(HGL_POINTS);            //设置为画点
        //    //obj->SetPointSize(size);                //设置点尺寸

        //    //创建顶点缓冲区
        //    VB3f *vertex=new VB3f(1);                //创建一个3D坐标点、浮点数据的缓冲区

        //    vertex->Begin();
        //        //vertex->Write(vertex.x,vertex.y,vertex.z);
        //        vertex->Write(vertex);
        //    vertex->End();

        //    obj->SetVertexBuffer(vbtVertex,vertex);

        //    return(obj);
        //}

//         /**
//         * 创建一条线的可渲染数据
//         * @param start 线条起始点
//         * @param end 线条终点
//         * @param color 线条颜色
//         */
//         VertexArray *CreateRenderableLine(const Vector3f &start,const Vector3f &end,const Color4f &color)
//         {
//             VertexArray *obj=CreateRenderable();
//             Material *mtl=CreateMaterial();
//
//             //设置材质
//             mtl->SetColorMaterial(true);            //设置使用颜色追踪(默认其实就是)
//             mtl->SetColor(color);                    //设置颜色
//             obj->SetMaterial(mtl,true);                //设置材质
//
//             //设置图元类型
//             obj->SetPrimitive(HGL_PRIM_LINES);        //设置为画线
//             //obj->SetPointSize(size);                //设置点尺寸
//
//             //创建顶点缓冲区
//             VB3f *vertex=new VB3f(2);                //创建一个3D坐标点、浮点数据的缓冲区
//
//             vertex->Begin();
//                 vertex->Write(start);
//                 vertex->Write(end);
//             vertex->End();
//
//             obj->SetVertex(vertex);
//
//             return(obj);
//         }
//
         /**
          * 创建一个画坐标轴的可渲染数据(默认尺寸为1)
          * @param size 坐标轴尺寸
          * @return 可渲染数据
          */
         VertexArray *CreateRenderableAxis(const float size)
         {
             VertexArray *obj=new VertexArray(HGL_PRIM_LINES);

             //创建顶点缓冲区
             VB3f *vertex=new VB3f(6);               //创建一个3D坐标点、浮点数据的缓冲区
             VB3f *color=new VB3f(6);                //创建一个4元颜色、浮点数据的缓冲区

             color->Begin();
             vertex->Begin();
                 color->Write(1,0,0);vertex->Write(0,0,0);color->Write(1,0,0);vertex->Write(size,0,0);
                 color->Write(0,1,0);vertex->Write(0,0,0);color->Write(0,1,0);vertex->Write(0,size,0);
                 color->Write(0,0,1);vertex->Write(0,0,0);color->Write(0,0,1);vertex->Write(0,0,size);
             vertex->End();
             color->End();

             obj->SetVertex(vertex);
             obj->SetColor(color,HGL_PC_RGB);

             return(obj);
         }

        /**
         * 画一个2D平面矩形
         */
        VertexArray *CreateRenderableRect(const RectScope2f &rs)
        {
            VertexArray *obj=new VertexArray(HGL_PRIM_TRIANGLE_FAN);

            VB2f *vertex=new VB2f(6);

            vertex->Begin();
                vertex->WriteRect(rs.Left,rs.Top,rs.Width,rs.Height);
            vertex->End();

            obj->SetVertex(vertex);

            return obj;
        }

        /**
         * 画一个带纹理的2D平面矩形
         */
        VertexArray *CreateRenderableRect(const RectScope2f &rs,const VertexBufferType &tex_coord_vbt,const RectScope2f &ts)
        {
            VertexArray *obj=new VertexArray(HGL_PRIM_TRIANGLE_FAN);

            VB2f *vertex=new VB2f(6);
            VB2f *texcoord=new VB2f(6);

            vertex->Begin();
                vertex->WriteRect(rs.Left,rs.Top,rs.Width,rs.Height);
            vertex->End();

            texcoord->Begin();
                texcoord->WriteRect(ts.Left,ts.Top,ts.Width,ts.Height);
            texcoord->End();

            obj->SetVertex(vertex);
            obj->SetVertexBuffer(tex_coord_vbt,texcoord);

            return obj;
        }

        /**
        * 画一个平面网格
        * @param v0 网格顶点0
        * @param v1 网格顶点1
        * @param v2 网格顶点2
        * @param v3 网格顶点3
        * @param step01 顶点0至1之间的格数
        * @param step12 顶点2至3之间的格数
        * @return 可渲染数据
        */
        VertexArray *CreateRenderablePlaneGrid( const Vector3f &v0,                                                        ///<创建一个平面网格的可渲染数据
                                                const Vector3f &v1,
                                                const Vector3f &v2,
                                                const Vector3f &v3,
                                                int step01,
                                                int step12)
        {
            VertexArray *obj=new VertexArray(HGL_PRIM_LINES);

            //创建顶点缓冲区
            VB3f *vertex=new VB3f(((step01+1)+(step12+1))*2);                //创建一个3D坐标点、浮点数据的缓冲区

            vertex->Begin();
                for(int row=0;row<=step01;row++)
                {
                    float pos=float(row)/float(step01);

                    Vector3f start  =to(v0,v1,pos);
                    Vector3f end    =to(v3,v2,pos);

                    vertex->WriteLine(start,end);
                }

                for(int col=0;col<=step12;col++)
                {
                    float pos=float(col)/float(step12);

                    Vector3f start  =to(v1,v2,pos);
                    Vector3f end    =to(v0,v3,pos);

                    vertex->WriteLine(start,end);
                }
            vertex->End();

            obj->SetVertex(vertex);

            return(obj);
        }

        VertexArray *CreateRenderablePlane(bool use_normal, const VertexBufferType tex_coord_vbt)
        {
            const float xy_vertices [] = { -0.5f,-0.5f,0.0f,  +0.5f,-0.5f,0.0f,    -0.5f,+0.5f,0.0f,   +0.5f,+0.5f,0.0f};
            const float xy_normals  [] = {  0.0f, 0.0f,1.0f,   0.0f, 0.0f,1.0f,     0.0f, 0.0f,1.0f,    0.0f, 0.0f,1.0f };
            const float xy_tangents [] = {  1.0f, 0.0f,0.0f,   1.0f, 0.0f,0.0f,     1.0f, 0.0f,0.0f,    1.0f, 0.0f,0.0f };
            const float xy_texCoords[] = {  0.0f, 0.0f,        1.0f,0.0f,           0.0f,1.0f,          1.0f, 1.0f};
            const uint16 xy_indices [] = {  0, 1, 2, 1, 3, 2 };

            VertexArray *obj=new VertexArray(HGL_PRIM_TRIANGLES);

            obj->SetVertex(new VB3f(12,xy_vertices));

            if(use_normal)
            {
                obj->SetNormal(new VB3f(12,xy_normals));
//                obj->SetTangent(new VB3f(12,xy_tangents));
            }

            if (tex_coord_vbt != vbtNone)
                obj->SetVertexBuffer(tex_coord_vbt,new VB2f(8,xy_texCoords));

            obj->SetIndex(new VB1u16(6,xy_indices));

            return obj;
        }

        /**
        * 创建一个立方体的可渲染数据
        * @param use_normal 是否使用法线和切线数据(默认不使用)
        * @param tex_coord_vbt 纹理坐标数据所对应的顶点缓冲区类型(默认不使用)
        * @param tex_scale 纹理坐标缩放倍率(默认为1)
        * @return 可渲染数据
        */
        VertexArray *CreateRenderableCube(bool use_normal,const VertexBufferType tex_coord_vbt,const float tex_scale)
        {                                // Points of a cube.
            /*     4            5 */    const float points[]={  -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, +0.5f,    +0.5f, -0.5f, +0.5f,    +0.5f, -0.5f, -0.5f,    -0.5f, +0.5f, -0.5f,    -0.5f, +0.5f, +0.5f,
            /*     *------------* */                            +0.5f, +0.5f, +0.5f,    +0.5f, +0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,    -0.5f, +0.5f, -0.5f,    +0.5f, +0.5f, -0.5f,    +0.5f, -0.5f, -0.5f,
            /*    /|           /| */                            -0.5f, -0.5f, +0.5f,    -0.5f, +0.5f, +0.5f,    +0.5f, +0.5f, +0.5f,    +0.5f, -0.5f, +0.5f,    -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, +0.5f,
            /*  0/ |         1/ | */                            -0.5f, +0.5f, +0.5f,    -0.5f, +0.5f, -0.5f,    +0.5f, -0.5f, -0.5f,    +0.5f, -0.5f, +0.5f,    +0.5f, +0.5f, +0.5f,    +0.5f, +0.5f, -0.5f    };
            /*  *--+---------*  | */    // Normals of a cube.
            /*  |  |         |  | */    const float normals[]={ +0.0f, -1.0f, +0.0f,    +0.0f, -1.0f, +0.0f,    +0.0f, -1.0f, +0.0f,    +0.0f, -1.0f, +0.0f,    +0.0f, +1.0f, +0.0f,    +0.0f, +1.0f, +0.0f,
            /*  | 7|         | 6| */                            +0.0f, +1.0f, +0.0f,    +0.0f, +1.0f, +0.0f,    +0.0f, +0.0f, -1.0f,    +0.0f, +0.0f, -1.0f,    +0.0f, +0.0f, -1.0f,    +0.0f, +0.0f, -1.0f,
            /*  |  *---------+--* */                            +0.0f, +0.0f, +1.0f,    +0.0f, +0.0f, +1.0f,    +0.0f, +0.0f, +1.0f,    +0.0f, +0.0f, +1.0f,    -1.0f, +0.0f, +0.0f,    -1.0f, +0.0f, +0.0f,
            /*  | /          | /  */                            -1.0f, +0.0f, +0.0f,    -1.0f, +0.0f, +0.0f,    +1.0f, +0.0f, +0.0f,    +1.0f, +0.0f, +0.0f,    +1.0f, +0.0f, +0.0f,    +1.0f, +0.0f, +0.0f    };
            /*  |/          2|/   */    // The associated indices.
            /* 3*------------*    */    const uint16 indices[]={    0,    2,    1,    0,    3,    2,    4,    5,    6,    4,    6,    7,    8,    9,    10,    8,    10,    11, 12,    15,    14,    12,    14,    13, 16,    17,    18,    16,    18,    19, 20,    23,    22,    20,    22,    21    };

            const float tangents[] = {  +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,
                                        +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,
                                        +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,      +1.0f, 0.0f, 0.0f,       0.0f, 0.0f,+1.0f,       0.0f, 0.0f,+1.0f,
                                         0.0f, 0.0f,+1.0f,       0.0f, 0.0f,+1.0f,       0.0f, 0.0f,-1.0f,       0.0f, 0.0f,-1.0f,       0.0f, 0.0f,-1.0f,       0.0f, 0.0f,-1.0f };

            const float tex_coords[] ={ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                                        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

            VertexArray *obj=new VertexArray(HGL_PRIM_TRIANGLES);

            obj->SetVertex(new VB3f(24,points));

            if(use_normal)
            {
                obj->SetNormal(new VB3f(24,normals));
                //obj->SetTangents(new VB3f(24,tangents));
            }

            if(tex_coord_vbt!=vbtNone)
            {
                VB2f *vb_tex=new VB2f(24,tex_coords);

                if(tex_scale!=1)
                {
                    float *p=(float *)(vb_tex->GetData());

                    for(int i=0;i<48;i++)
                    {
                        *p=(*p)*tex_scale;
                        ++p;
                    }
                }

                obj->SetVertexBuffer(tex_coord_vbt,vb_tex);
            }

            obj->SetIndex(new VB1u16(6*2*3,indices));

            return(obj);
        }

        template<typename T> VertexBuffer1<T> *CreateSphereIndices(const uint numberParallels,const uint numberSlices)
        {
            VertexBuffer1<T> *vb=new VertexBuffer1<T>(numberParallels * numberSlices * 6);

            vb->Begin();

            for (uint i = 0; i < numberParallels; i++)
            {
                for (uint j = 0; j < numberSlices; j++)
                {
                    vb->Write(i * (numberSlices + 1) + j);
                    vb->Write((i + 1) * (numberSlices + 1) + j);
                    vb->Write((i + 1) * (numberSlices + 1) + (j + 1));

                    vb->Write(i * (numberSlices + 1) + j);
                    vb->Write((i + 1) * (numberSlices + 1) + (j + 1));
                    vb->Write(i * (numberSlices + 1) + (j + 1));
                }
            }

            vb->End();
            return vb;
        }

        /**
        * 创建一个球体的可渲染数据
        * @param numberSlices 切片数
        * @return 可渲染数据
        */
        VertexArray *CreateRenderableSphere(const uint numberSlices)
        {
            VertexArray *obj=new VertexArray(HGL_PRIM_TRIANGLES);

            uint numberParallels = (numberSlices+1) / 2;
            uint numberVertices = (numberParallels + 1) * (numberSlices + 1);
            uint numberIndices = numberParallels * numberSlices * 6;

            const double angleStep = double(2.0f * HGL_PI) / ((double) numberSlices);

//            uint indexIndices;

            // used later to help us calculating tangents vectors
            //float helpVector[3] = { 1.0f, 0.0f, 0.0f };
//            float helpQuaternion[4];
//            float helpMatrix[16];

            VB3f *vertex;
            VB3f *normal;

            vertex=new VB3f(numberVertices);
            normal=new VB3f(numberVertices);

            vertex->Begin();
            normal->Begin();

            for (uint i = 0; i < numberParallels + 1; i++)
            {
                for (uint j = 0; j < numberSlices + 1; j++)
                {
                    float x=sin(angleStep * (double) i) * sin(angleStep * (double) j);
                    float y=cos(angleStep * (double) i);
                    float z=sin(angleStep * (double) i) * cos(angleStep * (double) j);

                    vertex->Write(x,y,z);
                    normal->Write(x,y,z);

//                     shape->texCoords[texCoordsIndex + 0] = (float) j / (float) numberSlices;
//                     shape->texCoords[texCoordsIndex + 1] = 1.0f - (float) i / (float) numberParallels;
//
//                     // use quaternion to get the tangent vector
//                     glusQuaternionRotateRyf(helpQuaternion, 360.0f * shape->texCoords[texCoordsIndex + 0]);
//                     glusQuaternionGetMatrix4x4f(helpMatrix, helpQuaternion);
//
//                     glusMatrix4x4MultiplyVector3f(&shape->tangents[tangentIndex], helpMatrix, helpVector);
                }
            }

            normal->End();
            vertex->End();

            obj->SetVertex(vertex);
            obj->SetNormal(normal);

            if(numberVertices<=0xffff)
                obj->SetIndex(CreateSphereIndices<uint16>(numberParallels,numberSlices));
            else
                obj->SetIndex(CreateSphereIndices<uint32>(numberParallels,numberSlices));

            return obj;
        }

         /**
         * 创建一个线框立方体的可渲染数据
         * @return 可渲染数据
         */
         VertexArray *CreateRenderableWireCube()
         {                                // Points of a cube.
            /*     4            5 */    const float points[]={  -0.5f, +0.5f, -0.5f,    //    0
            /*     *------------* */                            +0.5f, +0.5f, -0.5f,    //    1
            /*    /|           /| */                            +0.5f, -0.5f, -0.5f,    //    2
            /*  0/ |         1/ | */                            -0.5f, -0.5f, -0.5f,    //    3
            /*  *--+---------*  | */                            -0.5f, +0.5f, +0.5f,    //    4
            /*  |  |         |  | */                            +0.5f, +0.5f, +0.5f,    //    5
            /*  | 7|         | 6| */                            +0.5f, -0.5f, +0.5f,    //    6
            /*  |  *---------+--* */                            -0.5f, -0.5f, +0.5f};   //    7
            /*  | /          | /  */
            /*  |/          2|/   */    // The associated indices.
            /* 3*------------*    */    const uint16 indices[]={    0,1,1,2,2,3,3,0,    0,4,1,5,2,6,3,7,    4,5,5,6,6,7,7,4};

            VertexArray *obj=new VertexArray(HGL_PRIM_LINES);

            obj->SetVertex(new VB3f(8,points));
            obj->SetIndex(new VB1u16(12*2,indices));

            return(obj);
        }
//
//         const float RectVertex[8]={0,0, 1,0, 1,1, 0,1};
//
//         VertexArray *CreateRenderableRect(uint prim)
//         {
//             VertexArray *obj=CreateRenderable();
//             Material *mtl=CreateMaterial();
//
//             obj->SetMaterial(mtl,true);
//
//             obj->SetPrimitive(prim);
//
//             obj->SetVertex(new VB2f(4,RectVertex));
//
//             obj->AutoCreateShader(true);
//
//             return(obj);
//         }
//
//         /**
//         * 创建一个2D矩形的可渲染数据
//         */
//         VertexArray *CreateRenderableRect()
//         {
//             return CreateRenderableRect(HGL_PRIM_TRIANGLE_FAN);
//         }
//
//         /**
//         * 创建一个2D线框矩形的可渲染数据
//         */
//         VertexArray *CreateRenderableWireRect()
//         {
//             return CreateRenderableRect(HGL_PRIM_LINE_LOOP);
//         }
//
//         VertexArray *CreateRenderableTextureRect()
//         {
//             VertexArray *obj=CreateRenderableRect(HGL_PRIM_TRIANGLE_FAN);
//             Material *mtl=obj->GetMaterial();
//
//             obj->SetTexCoord(mtcDiffuse,vbtDiffuseTexCoord,new VB2f(4,RectVertex));
//             mtl->SetTexture(mtcDiffuse,BlackWhiteGrid);
//
//             obj->AutoCreateShader(true);
//
//             mtl->ClearTexture(mtcDiffuse);
//
//             return(obj);
//         }
//
//         /**
//         * 创建一个2D空心圆,圆心为0,0,半径为1
//         * @param edge 边数
//         */
//         VertexArray *CreateRenderableWireCircle(int edge)
//         {
//             const Vector4f start(1,0,0,1);
//             const Vector3f axis(0,0,1);
//
//             VertexArray *obj=CreateRenderable();
//             Material *mtl=CreateMaterial();
//             VB2f *vertex=new VB2f(edge);
//
//             obj->SetMaterial(mtl,true);
//
//             obj->SetPrimitive(HGL_PRIM_LINE_LOOP);
//
//             vertex->Begin();
//
//             for(int i=0;i<edge;i++)
//             {
//                 const Vector4f pos=start*rotate(float(i)/float(edge)*360.0,axis);
//
//                 vertex->Write((const float *)&pos);
//             }
//
//             vertex->End();
//
//             obj->SetVertex(vertex);
//
//             obj->AutoCreateShader(true);
//
//             return(obj);
//         }
//
//         /**
//         * 创建一个2D实心圆,圆心为0,0,半径为1
//         * @param edge 边数
//         */
//         VertexArray *CreateRenderableCircle(int edge)
//         {
//             const Vector4f start(1,0,0,1);
//             const Vector3f axis(0,0,1);
//
//             VertexArray *obj=CreateRenderable();
//             Material *mtl=CreateMaterial();
//             VB2f *vertex=new VB2f(edge+2);
//
//             obj->SetMaterial(mtl,true);
//
//             obj->SetPrimitive(HGL_PRIM_TRIANGLE_FAN);
//
//             vertex->Begin();
//
//             vertex->Write(0,0);
//
//             for(int i=0;i<=edge;i++)
//             {
//                 const Vector4f pos=start*rotate(float(i)/float(edge)*360.0,axis);
//
//                 vertex->Write((const float *)&pos);
//             }
//
//             vertex->End();
//
//             obj->SetVertex(vertex);
//
//             obj->AutoCreateShader(true);
//
//             return(obj);
//         }
    }//namespace graph

    namespace graph
    {
        void InitInlineRenderable()
        {
//             SolidCube=CreateRenderableCube(false,vbtNone);
//             WireCube=CreateRenderableWireCube();
//
//             SolidRect=CreateRenderableRect();
//             WireRect=CreateRenderableWireRect();
//             TextureRect=CreateRenderableTextureRect();
//
//             SolidCircle=CreateRenderableCircle(128);
//             WireCircle=CreateRenderableWireCircle(128);
        }

        void ClearInlineRenderable()
        {
//             SAFE_CLEAR(WireCircle);
//             SAFE_CLEAR(SolidCircle);
//
//             SAFE_CLEAR(WireRect);
//             SAFE_CLEAR(SolidRect);
//             SAFE_CLEAR(TextureRect);
//
//             SAFE_CLEAR(WireCube);
//             SAFE_CLEAR(SolidCube);

        }
    }//namespace graph
}//namespace hgl
