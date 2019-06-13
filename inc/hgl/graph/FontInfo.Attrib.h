    static FontInfo *DefaultFont;

private:

    UTF16String name;

    int width,height;
    bool bold,italic;

protected:

    UTF16String &GetName(){return name;}
    void SetName(UTF16String &);

    int GetWidth(){return width;}
    int GetHeight(){return height;}
    bool GetBold(){return bold;}
    bool GetItalic(){return italic;}

    void SetWidth(int);
    void SetHeight(int);
    void SetBold(bool);
    void SetItalic(bool);

    void InitPrivate();
