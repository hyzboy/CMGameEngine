#include<hgl/ut/KeyMapping.h>
namespace hgl
{
	KeyMapping::KeyMapping()
	{
		Clear();
	}

	KeyMapping::~KeyMapping()
	{
	}
    
    int KeyMapping::Find(ObjectList<KeyMapping::KeyEvent> &list,uint key)
    {
        int n=list.Count;

        while(n--)
            if(list[n]->key==key)
                return(n);
                
        return(-1);
    }
	
	bool KeyMapping::GetKeyMap(uint key,ObjectList<KeyEvent> &list,KeyMapping::OnKeyEvent &func)
	{
		const int result=Find(list,key);

		if(result==-1)return(false);

		func=list[result]->func;
		return(true);
	}

	void KeyMapping::SetKeyMap(uint key,ObjectList<KeyEvent> &list,KeyMapping::OnKeyEvent func)
	{
		const int result=Find(list,key);

        KeyEvent *obj;
        
        if(result==-1)
            obj=list.Append();
        else
			obj=list[result];
            
        obj->func=func;
        obj->key=key;
	}

	bool KeyMapping::ProcKeyMap(uint key,ObjectList<KeyEvent> &list)
	{
		const int result=Find(list,key);
        
        if(result==-1)return(false);
        
		CallEvent(list[result]->func,());
        return(true);
	}

	void KeyMapping::Clear()
	{
        KeyDownList.Clear();
		KeyPressList.Clear();
        KeyUpList.Clear();
        CharList.Clear();
	}
}//namespace hgl
