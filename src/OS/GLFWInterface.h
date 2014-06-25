#ifndef HGL_GLFW_INTERFACE_INCLUDE
#define HGL_GLFW_INTERFACE_INCLUDE

#include <hgl/platform/InputDevice.h>

#define GLFW_MOUSE_BUTTON_LEFT		mbLeft
#define GLFW_MOUSE_BUTTON_RIGHT		mbRight
#define GLFW_MOUSE_BUTTON_MIDDLE	mbMiddle
#define GLFW_MOUSE_BUTTON_4			mbX1
#define GLFW_MOUSE_BUTTON_5			mbX2

#define GLFW_MOUSE_BUTTON_LAST		mbEnd

#define GLFW_KEY_LAST				kb_END

#define GLFW_JOYSTICK_1				0

//#include <wacom/wacom.h>

namespace hgl
{
	uint ConvertOSKey(uint key);
}
#endif//HGL_GLFW_INTERFACE_INCLUDE
