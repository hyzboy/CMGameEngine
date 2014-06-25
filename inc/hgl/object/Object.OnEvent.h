public: //事件

	DefEvent(bool,OnCursorPos,		(int,int));									///<定位设备移动了
	DefEvent(bool,OnScroll,			(int,int));									///<滚轮转动了
	DefEvent(bool,OnMouseButton,	(int,bool));								///<鼠标按键按下或弹起了
	DefEvent(bool,OnJoystickButton,	(int,bool));								///<游戏杆按钮按下或弹起了
	DefEvent(bool,OnKey,			(int,bool));								///<有按键按下或弹起了
	DefEvent(bool,OnChar,			(char16_t));									///<有字符输入了
	DefEvent(bool,OnEvent,			(int,void *));								///<有自定义事件了
