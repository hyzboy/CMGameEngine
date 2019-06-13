    void SetSuperObject(Object *);

protected:

    GuiAlign align;                             //对齐属性
    bool move;                                  //是否可移动
    RectScope2i view_scope;                     //控件显示范围
    Vertex2i menu_coord;                        //屏幕绝对坐标
    int scissor[4];                             //裁剪坐标

    bool mouse_focus;                           //鼠标是否在内部

    MouseCoord mouse_coord;                     //鼠标最近一次坐标
    bool mouse_down[mbEnd];                     //鼠标是否按下
    Vector2f mouse_down_point[mbEnd];           //鼠标按下时的坐标点

    UTF16String hint;                           //提示字串

protected:

//          UTF16String &GetHint        (){return hint;}

            GuiAlign    GetAlign    ()const{return align;}

            bool        GetMove     ()const{return move;}

            int         GetLeft     ()const{return view_scope.Left;}
            int         GetTop      ()const{return view_scope.Top;}
    virtual int         GetWidth    ()const{return view_scope.Width;}
    virtual int         GetHeight   ()const{return view_scope.Height;}

            int         GetRight    ()const{return view_scope.GetRight();}
            int         GetBottom   ()const{return view_scope.GetBottom();}

            int         GetCenterX  ()const{return view_scope.GetCenterX();}
            int         GetCenterY  ()const{return view_scope.GetCenterY();}

    const Vertex2i &    GetMenuCoord()const{return menu_coord;}

            bool        GetFocus    ()const{return mouse_focus;}

protected:

            void SetCenterX(int cx){SetLeft(cx-view_scope.Width/2.0);}
            void SetCenterY(int cy){SetTop(cy-view_scope.Height/2.0);}

            void SetMove(bool m){move=m;}

//          void SetHint(const UTF16String &str){hint=str;}

    virtual void SetAlign(GuiAlign);

    virtual void CallRefresh();                                                                 ///<设定刷新

            void InitPrivate(const RectScope2i &rs);

public:

    virtual bool Proc_CursorPos(int,int);
    virtual bool Proc_MouseButton(int,bool);
