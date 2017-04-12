#ifndef HGL_LINE_FINDPATH_INCLUDE
#define HGL_LINE_FINDPATH_INCLUDE

#include<hgl/ut/FindPath.h>
#include<hgl/type/Stack.h>
namespace hgl
{
	namespace findpath
	{
		class LineFindPath:public FindPath
		{
			int px,py;

		private:

			bool PutPixel(int,int);
			bool Line(int,int,int,int);

			bool find_path();

		public:

			LineFindPath(int,int);
		};
	}//namespace findpath
}//namespace hgl
#endif//HGL_LINE_FINDPATH_INCLUDE
