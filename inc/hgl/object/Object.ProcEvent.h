public:

    virtual bool ProcMouseMove      (int,int);                                      ///<鼠标移动事件
    virtual bool ProcMouseButton    (int,bool);                                     ///<鼠标按键事件
    virtual bool ProcMouseWheel     (int,int);                                      ///<鼠标滚轮事件
    virtual bool ProcJoystickButton (int,bool);                                     ///<游戏杆按键事件
    virtual bool ProcKey            (int,bool);                                     ///<键盘按键事件
    virtual bool ProcChar           (os_char);                                      ///<字符事件
    virtual bool ProcTouch          (int,Vertex2i *);                               ///<触控点事件
    virtual bool ProcEvent          (int,void *);                                   ///<自定义事件
