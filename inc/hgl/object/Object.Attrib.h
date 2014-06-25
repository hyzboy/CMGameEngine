	friend class RadioItem;

public:

	/**
	* 对象属性位枚举
	*/
	enum ObjectAttrib
	{
		oaUpdate        = 1<<0,                     ///<对象需要刷新
		oaVisual        = 1<<1,                     ///<对象需要绘制
		oaProcEvent     = 1<<2,                     ///<对象可接收事件
		oaFlow          = 1<<3,                     ///<对象是流程对象

		oaControl       = 1<<4,                     ///<对象是控制器

		oaRes           = 1<<5,                     ///<对象是资源对象

		oaGUI           = 1<<6,                     ///<对象是否是GUI对象
		oaRadio   		= 1<<7,                     ///<对象是收音机对象

		oaKeyFocus		= 1<<8,						///<对象是否有可以键盘焦点
	};

	/**
	* 对象分类枚举
	*/
	enum ObjectClas
	{
		ocName=0,           ///<名字有效

		ocControl,          ///<控制器
		ocUpdate,           ///<需要刷新
		ocVisual,           ///<需要绘制
		ocProcEvent,        ///<需要处理事件

		ocRes,              ///<资源对象

		ocGUI,              ///<GUI对象
		ocAlign,            ///<需要做对齐运算
		ocRadio,      		///<收音机对象
		ocKeyFocus,			///<键盘焦点

		ocEnd               ///<结束定义，无意义
	};

protected:

	ObjectControl *super_object;

	uint object_attrib;                                                                     ///<当前对象属性

	bool destroy;

	bool enabled;

protected:

	ObjectControl *GetSuperObject(){return super_object;}

	virtual void SetSuperObject(ObjectControl *);
	virtual void SetEnabled(bool);

	const uint GetObjectAttrib	()const{return object_attrib;}
	const bool GetEnabled		()const{return enabled;}

	void InitPrivate();

public:

			const bool is_name		()const{return Name.Length();}
	virtual const bool is_update	()const{return(object_attrib&oaUpdate	);}
	virtual const bool is_visual	()const{return(object_attrib&oaVisual	);}
	virtual const bool is_proc_event()const{return(object_attrib&oaProcEvent);}
	virtual const bool is_flow		()const{return(object_attrib&oaFlow		);}
	virtual const bool is_control	()const{return(object_attrib&oaControl	);}

	virtual const bool is_res		()const{return(object_attrib&oaRes		);}

	virtual const bool is_gui		()const{return(object_attrib&oaGUI		);}
	virtual const bool is_align		()const{return(false);}
	virtual const bool is_radio		()const{return(object_attrib&oaRadio	);}

	virtual const bool is_key_focus	()const{return(object_attrib&oaKeyFocus	);}

	virtual const bool CheckObjectAttrib(int oc)const
	{
		if(oc<=ocName||oc>=ocEnd)return(false);

		if(oc==ocName		)return true;
		if(oc==ocControl	)return is_control();
		if(oc==ocUpdate		)return(is_update()&&enabled);
		if(oc==ocVisual		)return is_visual();
		if(oc==ocProcEvent	)return is_proc_event();
		if(oc==ocRes		)return is_res();
		if(oc==ocGUI		)return is_gui();
		if(oc==ocAlign		)return is_align();
		if(oc==ocRadio		)return is_radio();
		if(oc==ocKeyFocus	)return is_key_focus();

		return(false);
	}
