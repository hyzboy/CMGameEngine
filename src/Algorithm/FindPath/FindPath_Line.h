#ifndef HGL_ALGORITHM_LINE_FINDPATH_INCLUDE
#define HGL_ALGORITHM_LINE_FINDPATH_INCLUDE

#include<hgl/algorithm/FindPath.h>
#include<hgl/type/Stack.h>
namespace hgl
{
    namespace algorithm
    {
        namespace findpath
        {
            class LineFindPath :public FindPath
            {
                int px, py;

            private:

                bool PutPixel(int, int);
                bool Line(int, int, int, int);

                bool find_path();

            public:

                LineFindPath(int, int);
            };
        }//namespace findpath
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_LINE_FINDPATH_INCLUDE
