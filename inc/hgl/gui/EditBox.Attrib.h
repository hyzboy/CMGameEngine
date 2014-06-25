protected:

    void InitPrivate();

protected:	//光标

	Renderable *cursor_obj;																	///<当前光标可渲染对象
	Material *cursor_mtl;																	///<当前光标可渲染对象的材质

    int cursor_col;                                                                         ///<当前光标位置
    int show_start;                                                                         ///<从第几个字符开始显示
    bool cursor_visible;                                                                    ///<是否显示光标
    double time;                                                                  			///<本次光标闪烁时间

protected:

	bool password_edit;																		///<是否密码输入状态
	char16_t password_char;																	///<密码字符

	UTF16String edit_text;																	///<编辑用文本
	UTF16String pw_text;																		///<密码用文本
	Makeup makeup;																			///<排版效果
	Matrix4f text_mat;																		///<文本变换矩阵

	bool Proc_MouseButton(int,bool);
	bool Proc_Key(int,bool);
	bool Proc_Char(char16_t);

	virtual void Change(bool=true);

protected:

	int GetCol()const{return cursor_col;}
	void SetCol(int); 
	
	void SetEditText();

	bool GetPassword()const{return password_edit;}
	void SetPassword(bool);

	char16_t GetPasswordChar()const{return password_char;}
	void SetPasswordChar(char16_t ch);
