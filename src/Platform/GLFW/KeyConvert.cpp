#include<hgl/type/DataType.h>
#include<hgl/platform/InputDevice.h>
#include<GLFW/glfw3.h>

#ifdef _DEBUG
#include<hgl/LogInfo.h>
#endif//_DEBUG

namespace hgl
{
	static KeyboardButton KeyConvert[512];

	void InitKeyConvert()
	{
		int i;

		memset(KeyConvert,0,sizeof(KeyConvert));

		KeyConvert[GLFW_KEY_ESCAPE]=kbEsc;

		for(i=0;i<12;i++)KeyConvert[GLFW_KEY_F1+i]=(KeyboardButton)(kbF1+i);

		KeyConvert[GLFW_KEY_GRAVE_ACCENT]=kbGrave;
		for(i=0;i<=9;i++)KeyConvert[GLFW_KEY_0+i]=(KeyboardButton)(kb0+i);

        for(i=0;i<26;i++)KeyConvert[GLFW_KEY_A+i]=(KeyboardButton)(kbA+i);

		KeyConvert[GLFW_KEY_MINUS           ]=kbMinus;
		KeyConvert[GLFW_KEY_EQUAL           ]=kbEquals;
		KeyConvert[GLFW_KEY_BACKSLASH       ]=kbBackSlash;
		KeyConvert[GLFW_KEY_BACKSPACE       ]=kbBackSpace;

		KeyConvert[GLFW_KEY_TAB             ]=kbTab;
		KeyConvert[GLFW_KEY_LEFT_BRACKET    ]=kbLeftBracket;
		KeyConvert[GLFW_KEY_RIGHT_BRACKET   ]=kbRightBracket;

		KeyConvert[GLFW_KEY_CAPS_LOCK       ]=kbCapsLock;
		KeyConvert[GLFW_KEY_SEMICOLON       ]=kbSemicolon;
		KeyConvert[GLFW_KEY_APOSTROPHE      ]=kbApostrophe;
		KeyConvert[GLFW_KEY_ENTER           ]=kbEnter;

		KeyConvert[GLFW_KEY_LEFT_SHIFT      ]=kbLeftShift;
		KeyConvert[GLFW_KEY_COMMA           ]=kbComma;
		KeyConvert[GLFW_KEY_PERIOD          ]=kbPeriod;
		KeyConvert[GLFW_KEY_SLASH           ]=kbSlash;
		KeyConvert[GLFW_KEY_RIGHT_SHIFT     ]=kbRightShift;

		KeyConvert[GLFW_KEY_LEFT_CONTROL    ]=kbLeftCtrl;
		KeyConvert[GLFW_KEY_LEFT_SUPER		]=kbLeftOS;
		KeyConvert[GLFW_KEY_LEFT_ALT		]=kbLeftAlt;
		KeyConvert[GLFW_KEY_SPACE           ]=kbSpace;
		KeyConvert[GLFW_KEY_RIGHT_ALT		]=kbRightAlt;
		KeyConvert[GLFW_KEY_RIGHT_SUPER		]=kbRightOS;
        KeyConvert[GLFW_KEY_MENU            ]=kbRightMenu;
		KeyConvert[GLFW_KEY_RIGHT_CONTROL   ]=kbRightCtrl;
        
        KeyConvert[GLFW_KEY_PRINT_SCREEN    ]=kbPrintScreen;
		KeyConvert[GLFW_KEY_SCROLL_LOCK     ]=kbScrollLock;
		KeyConvert[GLFW_KEY_PAUSE           ]=kbPause;
        
		KeyConvert[GLFW_KEY_NUM_LOCK        ]=kbNumLock;

        for(i=0;i<=9;i++)KeyConvert[GLFW_KEY_KP_0+i]=(KeyboardButton)(kbNum0+i);

		KeyConvert[GLFW_KEY_KP_DECIMAL   ]=kbNumDecimal;
		KeyConvert[GLFW_KEY_KP_DIVIDE    ]=kbNumDivide;
		KeyConvert[GLFW_KEY_KP_MULTIPLY  ]=kbNumMultiply;
		KeyConvert[GLFW_KEY_KP_SUBTRACT  ]=kbNumSubtract;
		KeyConvert[GLFW_KEY_KP_ADD       ]=kbNumAdd;
        KeyConvert[GLFW_KEY_KP_ENTER     ]=kbNumEnter;

		KeyConvert[GLFW_KEY_UP          ]=kbUp;
		KeyConvert[GLFW_KEY_DOWN        ]=kbDown;
		KeyConvert[GLFW_KEY_LEFT        ]=kbLeft;
		KeyConvert[GLFW_KEY_RIGHT       ]=kbRight;

		KeyConvert[GLFW_KEY_INSERT      ]=kbInsert;
		KeyConvert[GLFW_KEY_DELETE      ]=kbDelete;
		KeyConvert[GLFW_KEY_HOME        ]=kbHome;
		KeyConvert[GLFW_KEY_END         ]=kbEnd;
		KeyConvert[GLFW_KEY_PAGE_UP     ]=kbPageUp;
		KeyConvert[GLFW_KEY_PAGE_DOWN   ]=kbPageDown;
	}

	KeyboardButton ConvertOSKey(uint key)
	{
		if(key<0||key>=512)return(kb_NONE);

#ifdef _DEBUG
		if(KeyConvert[key]==0)
		{
			LOG_INFO(OSString(L"Unknow Key: " )+OSString(key));
		}
#endif//_DEBUG

		return KeyConvert[key];
	}
}//namespace hgl
