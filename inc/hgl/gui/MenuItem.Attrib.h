	friend class MenuPage;

private:

	void InitPrivate();

	void SetRadioAttrib(MenuItem *);

	void SetCheck(bool);

protected:

	UTF16String  caption;                                                                        ///<标题

	MenuItem *SuperMenu;																	///<上级菜单

	MenuPage *menupage;                                                                    	///<弹出的菜单页

protected:

			void CloseMenu();																		///<关闭菜单

			int GetIndex();																			///<取当前菜单在上级菜单中的索引编号
			MenuPage *GetSubMenuPage(){return menupage;}

			void SetCaption();

			int GetWidth();
			int GetHeight();

	virtual bool Proc_Scroll            	(int,int){return(true);}
	virtual bool Proc_MouseButton			(int,bool);
	virtual bool Proc_JoystickButton		(int,bool){return(true);}
	virtual bool Proc_Key         			(int,bool){return(true);}
	virtual bool Proc_Char          		(char16_t){return(true);}
	virtual bool Proc_Event         		(int,void *){return(true);}

			void DefaultDraw(GUIObject *,const Matrix4f *);
			void DefaultEnter(GUIObject *);
			void DefaultLeave(GUIObject *);

			void DefaultMeasureItem(GUIObject *,float &,float &);

			void DefaultCheckDraw(GUIObject *,const Matrix4f *);
			void DefaultCheck(GUIObject *);
