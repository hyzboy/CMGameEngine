	friend class ObjectControl;

public:

	virtual bool Proc_CursorPos     (int	,int)	{return(false);}
	virtual bool Proc_Scroll    	(int	,int)	{return(false);}
	virtual bool Proc_MouseButton	(int	,bool)	{return(false);}
	virtual bool Proc_JoystickButton(int	,bool)	{return(false);}
	virtual bool Proc_Key			(int	,bool)	{return(false);}
	virtual bool Proc_Char          (char16_t)		{return(false);}
	virtual bool Proc_Event         (int	,void *);

protected:

	bool active;

protected:

	bool GetActive(){return active;}

	virtual void SetActive(bool);
