#ifndef HGL_DATA_OBJECT_INCLUDE
#define HGL_DATA_OBJECT_INCLUDE

#include<hgl/object/Object.h>
namespace hgl
{
	class Stream;

	class DataObject:public Object
	{
	public:

		virtual ~DataObject()=default;

		virtual bool SaveToStream(Stream *){return(false);}
		virtual bool LoadFromStream(Stream *){return(false);}
	};//class DataObject
}//namespace hgl
#endif//HGL_DATA_OBJECT_INCLUDE
