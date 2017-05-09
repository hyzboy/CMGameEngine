#include<hgl/type/DataType.h>
#include<hgl/platform/InputDevice.h>
#include<Windows.h>
#include<GLFW/glfw3.h>

#ifdef _DEBUG
#include<hgl/LogInfo.h>
#endif//_DEBUG

namespace hgl
{
	static KeyboardButton KeyConvert[256];

	void InitKeyConvert()
	{
		int i;

		memset(KeyConvert,0,sizeof(KeyConvert));

		KeyConvert[VK_ESCAPE    ]=kbEsc;
		for(i=VK_F1;i<=VK_F12;i++)KeyConvert[i]=(KeyboardButton)(kbF1+i-VK_F1);

		KeyConvert['`'          ]=kbGrave;
		for(i='0';i<='9';i++)KeyConvert[i]=(KeyboardButton)(kb0+i-'0');
		KeyConvert['-'          ]=kbMinus;
		KeyConvert['='          ]=kbEquals;
		KeyConvert['\\'         ]=kbBackSlash;
		KeyConvert[VK_BACK      ]=kbBackSpace;

		KeyConvert[VK_TAB       ]=kbTab;
		KeyConvert['Q'          ]=kbQ;
		KeyConvert['W'          ]=kbW;
		KeyConvert['E'          ]=kbE;
		KeyConvert['R'          ]=kbR;
		KeyConvert['T'          ]=kbT;
		KeyConvert['Y'          ]=kbY;
		KeyConvert['U'          ]=kbU;
		KeyConvert['I'          ]=kbI;
		KeyConvert['O'          ]=kbO;
		KeyConvert['P'          ]=kbP;
		KeyConvert['['          ]=kbLeftBracket;
		KeyConvert[']'          ]=kbRightBracket;

		KeyConvert[VK_CAPITAL   ]=kbCapsLock;
		KeyConvert['A'          ]=kbA;
		KeyConvert['S'          ]=kbS;
		KeyConvert['D'          ]=kbD;
		KeyConvert['F'          ]=kbF;
		KeyConvert['G'          ]=kbG;
		KeyConvert['H'          ]=kbH;
		KeyConvert['J'          ]=kbJ;
		KeyConvert['K'          ]=kbK;
		KeyConvert['L'          ]=kbL;
		KeyConvert[';'          ]=kbSemicolon;
		KeyConvert['\''         ]=kbApostrophe;
		KeyConvert[VK_RETURN    ]=kbEnter;

		KeyConvert[VK_LSHIFT    ]=kbLeftShift;
		KeyConvert['Z'          ]=kbZ;
		KeyConvert['X'          ]=kbX;
		KeyConvert['C'          ]=kbC;
		KeyConvert['V'          ]=kbV;
		KeyConvert['B'          ]=kbB;
		KeyConvert['N'          ]=kbN;
		KeyConvert['M'          ]=kbM;
		KeyConvert[','          ]=kbComma;
		KeyConvert['.'          ]=kbPeriod;
		KeyConvert['/'          ]=kbSlash;
		KeyConvert[VK_RSHIFT    ]=kbRightShift;

		KeyConvert[VK_LCONTROL  ]=kbLeftCtrl;
		KeyConvert[VK_LWIN		]=kbLeftOS;
		KeyConvert[VK_LMENU		]=kbLeftAlt;
		KeyConvert[VK_SPACE     ]=kbSpace;
		KeyConvert[VK_RMENU		]=kbRightAlt;
		KeyConvert[VK_RWIN		]=kbRightOS;
		KeyConvert[VK_RCONTROL  ]=kbRightCtrl;

		KeyConvert[VK_PAUSE     ]=kbPause;
	//        KeyConvert[VK_CLEAR     ]=kbClear;

		KeyConvert[VK_NUMPAD0   ]=kbNum0;
		KeyConvert[VK_NUMPAD1   ]=kbNum1;
		KeyConvert[VK_NUMPAD2   ]=kbNum2;
		KeyConvert[VK_NUMPAD3   ]=kbNum3;
		KeyConvert[VK_NUMPAD4   ]=kbNum4;
		KeyConvert[VK_NUMPAD5   ]=kbNum5;
		KeyConvert[VK_NUMPAD6   ]=kbNum6;
		KeyConvert[VK_NUMPAD7   ]=kbNum7;
		KeyConvert[VK_NUMPAD8   ]=kbNum8;
		KeyConvert[VK_NUMPAD9   ]=kbNum9;

		KeyConvert[VK_DECIMAL   ]=kbNumDecimal;
		KeyConvert[VK_DIVIDE    ]=kbNumDivide;
		KeyConvert[VK_MULTIPLY  ]=kbNumMultiply;
		KeyConvert[VK_SUBTRACT  ]=kbNumSubtract;
		KeyConvert[VK_ADD       ]=kbNumAdd;

		KeyConvert[VK_UP        ]=kbUp;
		KeyConvert[VK_DOWN      ]=kbDown;
		KeyConvert[VK_LEFT      ]=kbLeft;
		KeyConvert[VK_RIGHT     ]=kbRight;

		KeyConvert[VK_INSERT    ]=kbInsert;
		KeyConvert[VK_DELETE    ]=kbDelete;
		KeyConvert[VK_HOME      ]=kbHome;
		KeyConvert[VK_END       ]=kbEnd;
		KeyConvert[VK_PRIOR     ]=kbPageUp;
		KeyConvert[VK_NEXT      ]=kbPageDown;

		KeyConvert[VK_NUMLOCK   ]=kbNumLock;
		KeyConvert[VK_SCROLL    ]=kbScrollLock;

		//KeyConvert[VK_SHIFT		]=kbLeftShift;
		//KeyConvert[VK_CONTROL	]=kbLeftCtrl;
		//KeyConvert[VK_MENU		]=kbLeftAlt;

		KeyConvert[VK_OEM_1		]=kbSemicolon;
		KeyConvert[VK_OEM_PLUS	]=kbEquals;
		KeyConvert[VK_OEM_COMMA	]=kbComma;
		KeyConvert[VK_OEM_MINUS	]=kbMinus;
		KeyConvert[VK_OEM_PERIOD]=kbPeriod;
		KeyConvert[VK_OEM_2		]=kbSlash;
		KeyConvert[VK_OEM_3		]=kbGrave;
		KeyConvert[VK_OEM_4		]=kbLeftBracket;
		KeyConvert[VK_OEM_5		]=kbBackSlash;
		KeyConvert[VK_OEM_6		]=kbRightBracket;
		KeyConvert[VK_OEM_7		]=kbApostrophe;
	}

	KeyboardButton ConvertOSKey(uint key)
	{
		if(key<0||key>=256)return(kb_NONE);

		if(key==VK_SHIFT)
		{
			if((GetAsyncKeyState(VK_LSHIFT)>>15)&1)
				return kbLeftShift;
			else
				return kbRightShift;
		}
		else
		if(key==VK_CONTROL)
		{
			if((GetAsyncKeyState(VK_LCONTROL)>>15)&1)
				return kbLeftCtrl;
			else
				return kbRightCtrl;
		}
		if(key==VK_MENU)
		{
			if((GetAsyncKeyState(VK_LMENU)>>15)&1)
				return kbLeftAlt;
			else
				return kbRightAlt;
		}

#ifdef _DEBUG
		if(KeyConvert[key]==0)
		{
			wchar_t name[64];

			::GetKeyNameTextW(key,name,64);

			LOG_INFO(WideString(L"Unknow Key: " )+WideString(key)
                    +WideString(L" ,name: "     )+WideString(name));
		}
#endif _DEBUG

		return KeyConvert[key];
	}
}//namespace hgl
