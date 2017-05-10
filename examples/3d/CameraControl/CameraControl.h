#ifndef CAMERA_CONTROL_INCLUDE
#define CAMERA_CONTROL_INCLUDE

#include<hgl/graph/Camera.h>
#include<hgl/platform/InputDevice.h>
#include<hgl/object/FlowObject.h>

using namespace hgl;
using namespace hgl::graph;

class CameraControlFlowObject:public FlowObject
{
private:    

	bool mouse_down[mbEnd];
	Vector2f mouse_coord;
	Vector2f push_coord[mbEnd];
	
	Vector3f push_center[mbEnd];
	Vector3f push_eye[mbEnd];

protected:

	WalkerCamera cam;
	float step;

	Matrix4f proj;
	Matrix4f mv;

public:
    
	virtual void OnResize       (int,int);
    
	virtual bool OnMouseMove    (int,int);
	virtual bool OnMouseWheel   (int,int);

	virtual bool OnMouseDown    (MouseButton);
    virtual bool OnMouseUp      (MouseButton);
    
    virtual bool OnKeyDown      (KeyboardButton);
    virtual bool OnKeyRepeat    (KeyboardButton kb){return OnKeyDown(kb);}

public:

	CameraControlFlowObject();

	virtual void Update();
};//class CameraControl
#endif//CAMERA_CONTROL_INCLUDE
