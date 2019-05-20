#include<hgl/db/TableDesc.h>

namespace hgl
{
    namespace db
    {
        bool TableDesc::AddField(FieldDesc *f)                                            ///<增加一个字段
        {
            if(!f)return(false);

            if(field_list.IsExist(f))return(false);

            if(field_by_name.KeyExist(f->name))return(false);

            if(f->primary)
            {
                if(primary_field)
                    return(false);

                primary_field=f;
            }

            f->serial=field_list.GetCount();
            field_list.Add(f);
            field_by_name.Add(f->name,f);
            return(true);
        }
    }//namespace db
}//namespace hgl
