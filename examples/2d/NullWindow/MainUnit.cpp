#include<hgl/Graphics.h>				//GraphicsApplication,SystemInitInfo
#include<hgl/graph/Render.h>			//SetClearColor,ClearScreen
#include<hgl/object/FlowObject.h>		//FlowObject

using namespace hgl;
using namespace hgl::graph;

class TestObject:public FlowObject
{
public:

	TestObject()
	{
		SetClearColor(0,0,0);
	}

	~TestObject()
	{
	}

	void Draw()
	{
		ClearScreen();
	}
};//class TestObject

HGL_GRAPHICS_APPLICATION("空窗口","NullWindow",new TestObject());
