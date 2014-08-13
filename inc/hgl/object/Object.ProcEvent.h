public:

	virtual bool Proc_CursorPos     (int,int);
	virtual bool Proc_Scroll    	(int,int);
	virtual bool Proc_MouseButton	(int,bool);
	virtual bool Proc_JoystickButton(int,bool);
	virtual bool Proc_Key			(int,bool);
	virtual bool Proc_Char          (u16char);
	virtual bool Proc_Event         (int,void *);
