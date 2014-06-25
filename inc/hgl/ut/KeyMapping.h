#ifndef HGL_KEYMAPPING_INCLUDE
#define HGL_KEYMAPPING_INCLUDE

#include<hgl/object/EventObject.h>
#include<hgl/InputState.h>
#include<hgl/type/List.h>
namespace hgl
{
	/**
	* 按键映射可自动在产生相应按键事件时,调用指定的事件函数
	*/
	class KeyMapping:public EventObject                                                             ///<按键映射类
	{
	protected:

		typedef DefEvent(void,OnKeyEvent,());

		struct KeyEvent
		{
			uint key;

			OnKeyEvent func;
		};

		ObjectList<KeyEvent> KeyDownList,KeyPressList,KeyUpList,CharList;

		int Find(ObjectList<KeyEvent> &,uint);

		bool GetKeyMap(uint,ObjectList<KeyEvent> &,KeyMapping::OnKeyEvent &);
		void SetKeyMap(uint,ObjectList<KeyEvent> &,KeyMapping::OnKeyEvent);
		bool ProcKeyMap(uint key,ObjectList<KeyEvent> &list);

	protected:

		virtual bool Proc_KeyDown   (uint key){return ProcKeyMap(key,KeyDownList	);}
		virtual bool Proc_KeyPress  (uint key){return ProcKeyMap(key,KeyPressList	);}
		virtual bool Proc_KeyUp     (uint key){return ProcKeyMap(key,KeyUpList		);}
		virtual bool Proc_Char		(uint key){return ProcKeyMap(key,CharList		);}

	public:

		KeyMapping();
		virtual ~KeyMapping();

		virtual bool GetKeyDown	(uint key,OnKeyEvent &ke){return GetKeyMap(key,KeyDownList,	ke);}
		virtual bool GetKeyPress(uint key,OnKeyEvent &ke){return GetKeyMap(key,KeyPressList,ke);}
		virtual bool GetKeyUp	(uint key,OnKeyEvent &ke){return GetKeyMap(key,KeyUpList,	ke);}
		virtual bool GetChar	(uint key,OnKeyEvent &ke){return GetKeyMap(key,CharList,	ke);}

		virtual void SetKeyDown	(uint key,OnKeyEvent ke){SetKeyMap(key,KeyDownList,	ke);}
		virtual void SetKeyPress(uint key,OnKeyEvent ke){SetKeyMap(key,KeyPressList,ke);}
		virtual void SetKeyUp	(uint key,OnKeyEvent ke){SetKeyMap(key,KeyUpList,	ke);}
		virtual void SetChar	(uint key,OnKeyEvent ke){SetKeyMap(key,CharList,	ke);}

		void Clear();                                                                               ///<清除键盘映射
	};//class KeyMapping
}//namespace hgl
#endif//HGL_KEYMAPPING_INCLUDE
