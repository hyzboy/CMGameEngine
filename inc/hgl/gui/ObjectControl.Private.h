private:

    bool CheckObjectAttrib(Object *,int);

    void InitPrivate();

protected:

    EventObject *active_obj;
    GUIObject *keyfocus_obj;

    Stack<EventObject *> stack_obj;                                                                 //活动对象堆栈

    bool RefreshFlag;
    int inside_scissor[4];

    virtual void AlignObject();
    virtual void RefreshScissor();
    virtual void SetActive(bool);

    virtual GUIObject *FindKeyFocus(GUIObject *,int);

protected:

    ObjectList<Object> destroy_obj;                                                                 //要销毁的对象

public: //设置对象属性(内部方法，请不要调用)

    virtual void SetEnabledAttrib(Object *,bool);
    virtual void SetUpdateAttrib(EnumObject *,bool);

    virtual void SetVisibleAttrib(VisualObject *,bool);
    virtual void SetProcEventAttrib(EventObject *,bool);
    virtual void SetActiveAttrib(EventObject *,bool);
    virtual void SetAlignAttrib(GUIObject *,GuiAlign);
    virtual void SetRadioAttrib(GUIObject *);

    virtual void SetDestroyAttrib(Object *);

    virtual void SetRefresh();
    virtual void Refresh();
