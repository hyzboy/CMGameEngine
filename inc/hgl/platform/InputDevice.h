#ifndef HGL_INPUT_DEVICE_INCLUDE
#define HGL_INPUT_DEVICE_INCLUDE
namespace hgl
{
	/**
	* 鼠标按钮枚举
	*/
	enum MouseButton:int
	{
		mbLeft=0,
		mbRight,
		mbMiddle,

		mbX1,
		mbX2,

		mbEnd
	};//enum MouseButton

	/**
	* 按键枚举定义
	*/
	enum KeyboardButton
	{
		kb_NONE=0,			///<起始定义，无意义

		//主键盘区
		kbEsc,              ///<ESC

		//F功能键
		kbF1,               ///<F1
		kbF2,               ///<F2
		kbF3,               ///<F3
		kbF4,               ///<F4
		kbF5,               ///<F5
		kbF6,               ///<F6
		kbF7,               ///<F7
		kbF8,               ///<F8
		kbF9,               ///<F9
		kbF10,              ///<F10
		kbF11,              ///<F11
		kbF12,              ///<F12

		kbGrave,            //<`号(主键盘数字键1左边的按钮)

		//10个数字
		kb0,                ///<数字键0
		kb1,                ///<数字键1
		kb2,                ///<数字键2
		kb3,                ///<数字键3
		kb4,                ///<数字键4
		kb5,                ///<数字键5
		kb6,                ///<数字键6
		kb7,                ///<数字键7
		kb8,                ///<数字键8
		kb9,                ///<数字键9

		kbMinus,            ///< - (减号)
		kbEquals,           ///< = (等号)
		kbBackSlash,        ///< \ (反斜杠)
		kbBackSpace,        ///< 退格键

		kbTab,              ///<Tab键
		kbQ,                ///<Q
		kbW,                ///<W
		kbE,                ///<E
		kbR,                ///<R
		kbT,                ///<T
		kbY,                ///<Y
		kbU,                ///<U
		kbI,                ///<I
		kbO,                ///<O
		kbP,                ///<P

		kbLeftBracket,      ///<[
		kbRightBracket,     ///<]

		kbCapsLock,         ///<大写锁定键
		kbA,                ///<A
		kbS,                ///<S
		kbD,                ///<D
		kbF,                ///<F
		kbG,                ///<G
		kbH,                ///<H
		kbJ,                ///<J
		kbK,                ///<K
		kbL,                ///<L

		kbSemicolon,        ///<; (分号)
		kbApostrophe,       ///<' (单引号)
		kbEnter,            ///<回车键

		kbLeftShift,        ///<左边的Shift键
		kbZ,                ///<Z
		kbX,                ///<X
		kbC,                ///<C
		kbV,                ///<V
		kbB,                ///<B
		kbN,                ///<N
		kbM,                ///<M

		kbComma,            ///<, (逗号)
		kbPeriod,           ///<. (句号)
		kbSlash,            ///</ (除号)
		kbRightShift,       ///<右边的Shift键

		kbLeftCtrl,         ///<左边的Ctrl键
		kbLeftOS,			///<左边的OS键(Win/Apple键)
		kbLeftAlt,          ///<左边的Alt键
		kbSpace,            ///<空格键
		kbRightAlt,         ///<右边的Alt键
		kbRightOS,			///<右边的OS键(Win/Apple键)
		kbRightCtrl,        ///<右边的Ctrl键

		//中键盘区
		kbPrintScreen,      ///<打印屏幕键
		kbScrollLock,       ///<滚动锁定键
		kbPause,            ///<暂停键

		kbInsert,           ///<插入键
		kbDelete,           ///<删除键
		kbHome,             ///<行首键
		kbEnd,              ///<行尾键
		kbPageUp,           ///<向前翻页键
		kbPageDown,         ///<向后翻页键

		kbUp,               ///<↑光标键
		kbDown,             ///<↓光标键
		kbLeft,             ///<←光标键
		kbRight,            ///<→光标键

		//小键盘区
		kbNumLock,          ///<小键盘 数字锁定键
		kbNumDivide,        ///<小键盘 /
		kbNumMultiply,      ///<小键盘 *
		kbNumSubtract,      ///<小键盘 -

		kbNum7,             ///<小键盘 7
		kbNum8,             ///<小键盘 8
		kbNum9,             ///<小键盘 9
		kbNum4,             ///<小键盘 4
		kbNum5,             ///<小键盘 5
		kbNum6,             ///<小键盘 6

		kbNumAdd,           ///<小键盘 +

		kbNum1,             ///<小键盘 1
		kbNum2,             ///<小键盘 2
		kbNum3,             ///<小键盘 3
		kbNum0,             ///<小键盘 0
		kbNumDecimal,       ///<小键盘 . (小数点/删除键)
		kbNumEnter,         ///<小键盘 回车键

		kb_END				///<结束定义，无意义
	};//enum KeyboardButton

	/**
	* 手柄按键枚举
	*/
	enum JoystickButton
	{
		jbNone=0,

		jbUp,
		jbDown,
		jbLeft,
		jbRight,

		jb0,		jb1,		jb2,		jb3,		jb4,		jb5,		jb6,		jb7,
		jb8,		jb9,		jb10,		jb11,		jb12,		jb13,		jb14,		jb15,
		jb16,		jb17,		jb18,		jb19,		jb20,		jb21,		jb22,		jb23,
		jb24,		jb25,		jb26,		jb27,		jb28,		jb29,		jb30,		jb31,

		jbEnd,

		//DreamCast/XBOX/XBOX360
/*		jbX=jb2,
		jbY=jb3,
		jbA=jb0,
		jbB=jb1,
		jbL=jb4,
		jbR=jb5,*/

		//PlayStation
		jbTriangle	=jb0,			// 三角
		jbCircle	=jb1,			// 圆
		jbFork		=jb2,			// 叉
		jbRectangle	=jb3,			// 方
		jbL1		=jb6,		jbL2		=jb4,
		jbR1		=jb7,		jbR2		=jb5,
		jbSelect	=jb8,		jbStart		=jb9,

		//XBOX/XBOX360
//		jbXBOX,						//西瓜键
	};

	/**
	* 取得键盘按键状态
	* @param key 按键
	* @return true 按下状态
	* @return false 弹起状态
	*/
	bool GetKeyState(KeyboardButton key);

	/**
	* 取得游戏杆按键状态
	* @param key 按键
	* @return true 按下状态
	* @return false 弹起状态
	*/
//	bool GetJotState(JoystickButton);
}//namespace hgl
#endif//HGL_INPUT_DEVICE_INCLUDE
