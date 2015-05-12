#include<hgl/graph/Font.h>

namespace hgl
{
    Font *Font::DefaultFont=nullptr;

	void Font::InitPrivate()
	{
		name.Clear();
		width=0;
		height=0;
		bold=false;
		italic=false;

		hglSetProperty(Name,	this,Font::GetName,		Font::SetName	);

		hglSetProperty(Width,	this,Font::GetWidth,	Font::SetWidth	);
		hglSetProperty(Height,	this,Font::GetHeight,	Font::SetHeight	);

		hglSetProperty(Bold,	this,Font::GetBold,		Font::SetBold	);
		hglSetProperty(Italic,	this,Font::GetItalic,	Font::SetItalic	);

		OnChange=nullptr;
	}

	Font::Font()
	{
		InitPrivate();

		if(DefaultFont)
			operator = (*DefaultFont);
	}

	/**
	* 字体数据类构造函数，从一个现有的字体结构复制
	* @param fnt 现有的字体数据结构
	*/
    Font::Font(const Font &fnt)
	{
		InitPrivate();

		operator = (fnt);
    }

    /**
    * 字体数据类构造函数
    * @param n 字体名称
    * @param w 字体宽度,一般设为高度的一半，或是0
    * @param h 字体高度
    * @param b 是否使用粗体
	* @param i 是否使用斜体
	*/
	Font::Font(const UTF16String &n,int w,int h,bool b,bool i)
	{
		InitPrivate();

		Set(n,w,h,b,i);
	}

	/**
	* 设置字体数据
	* @param n 字体名称
	* @param w 字体宽度,一般设为高度的一半，或是0
	* @param h 字体高度
	* @param b 是否使用粗体
	* @param i 是否使用斜体
	*/
	void Font::Set(const UTF16String &n,int w,int h,bool b,bool i)
	{
		InitPrivate();

		Name		=n;

		Width		=w;
		Height		=h;

		Bold		=b;
		Italic		=i;

		SafeCallEvent(OnChange,(this));
    }

	#define FONTCOMP(func_name,parm_type,this_data,parm_data)	void Font::func_name(parm_type param_data)	\
																{	\
																	if(this_data==param_data)return;	\
																	\
																	this_data=param_data;	\
																	\
																	SafeCallEvent(OnChange,(this));	\
																}
	FONTCOMP(SetWidth		,int			,width		,w);
	FONTCOMP(SetHeight		,int			,height		,h);
	FONTCOMP(SetBold		,bool			,bold		,b);
	FONTCOMP(SetItalic		,bool			,italic		,i);

	#undef FONTCOMP

	void Font::SetName(UTF16String &ws)
	{
		UTF16String old_name=name;

		name=ws;

		SafeCallEvent(OnChange,(this));
	}

    /**
    * 设置缺省字体数据
    * @param n 字体名称
    * @param w 字体宽度,一般设为高度的一半，或是0
    * @param h 字体高度
    * @param b 是否使用粗体
    * @param i 是否使用斜体
    */
    void Font::SetDefaultFont(const UTF16String &n,int w,int h,bool b,bool i)
    {
		if(!DefaultFont)
			DefaultFont=new Font;

        DefaultFont->Name		=n;

        DefaultFont->Width		=w;
        DefaultFont->Height		=h;

        DefaultFont->Bold		=b;
        DefaultFont->Italic		=i;
    }

    void Font::SetDefaultFont(const Font &fnt)
	{
		if(!DefaultFont)
			DefaultFont=new Font;

    	DefaultFont->Name	=fnt.Name;

    	DefaultFont->Width	=fnt.Width;
    	DefaultFont->Height	=fnt.Height;

    	DefaultFont->Bold	=fnt.Bold;
    	DefaultFont->Italic	=fnt.Italic;
    }

	void Font::ClearDefaultFont()
	{
		SAFE_CLEAR(DefaultFont);
	}

    bool Font::operator == (const Font &fnt)
    {
        if(Name		!=fnt.Name	)return(false);
		if(Width	!=fnt.Width	)return(false);
		if(Height	!=fnt.Height)return(false);
		if(Bold		!=fnt.Bold	)return(false);
		if(Italic	!=fnt.Italic)return(false);

        return(true);
    }

    bool Font::operator != (const Font &fnt)
	{
		return(!(operator==(fnt)));
	}

	void Font::operator = (const Font &src)
	{
		if(this==&src)return;

		Name		=src.name;

		Width		=src.width;
		Height		=src.height;

		Bold		=src.bold;
		Italic		=src.italic;
    }
}//namespace hgl
