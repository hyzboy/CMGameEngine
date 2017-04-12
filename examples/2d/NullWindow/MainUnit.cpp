#include<hgl/Graphics.h>                //GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>            //SetClearColor,ClearColorDepthBuffer
#include<hgl/object/FlowObject.h>        //FlowObject

using namespace hgl;
using namespace hgl::graph;

class TestObject:public FlowObject
{
public:

    TestObject()
    {
        SetClearColor(ceSlateBlue);
    }

    ~TestObject()
    {
    }

    void Draw(const Matrix4f *)
    {
        ClearColorDepthBuffer();
    }
};//class TestObject

HGL_GRAPHICS_APPLICATION("空窗口","NullWindow",new TestObject());
