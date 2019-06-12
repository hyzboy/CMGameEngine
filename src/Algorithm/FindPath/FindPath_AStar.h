#ifndef HGL_ASTAR_FINDPATH_INCLUDE
#define HGL_ASTAR_FINDPATH_INCLUDE

#include<hgl/type/List.h>
#include<hgl/algorithm/FindPath.h>
#include<math.h>
namespace hgl
{
    namespace algorithm
    {
        namespace findpath
        {
            struct FindItem
            {
                FindItem *prev;

                List<FindItem *> sub;

                int x, y;
                int step;

                float length;

                FindItem()
                {
                    prev = nullptr;
                    x = -1;
                    y = -1;
                    step = -1;
                    length = -1;
                }

                void Set(FindItem *p, int cx, int cy, float len)
                {
                    prev = p;

                    sub.ClearData();

                    x = cx;
                    y = cy;

                    if (p)
                        step = p->step + 1;
                    else
                        step = 0;

                    length = len;
                }
            };

            class AStarFindPath :public FindPath
            {
            private:

                FindItem ***find;               //标记于每一个格子上的查找项，用以标记已经查过了

                List<FindItem *> findlist;      //要查找的队列
                //findlist 是仍可优先的重点，由于在每次查找中都需要动态的调整大小，重分配内容，将会耗费大量CPU时间。
                //最为理想的方式是更换为链表

                FindItem *find_buffer;          //放入结果队列用的数据，提前分配以提升效能
                int find_buffer_index;          //缓冲索引

            private:

                __inline bool checkpoint(int x, int y, int px, int py)
                {
                    return CheckPoint(x, y, px, py);
                }

                __inline float comp_length(int x, int y)
                {
                    //return sqrt((EndX-x)*(EndX-x)+(EndY-y)*(EndY-y));
                    //return (EndX-x)*(EndX-x)+(EndY-y)*(EndY-y);
                    return fabs(float(EndX - x)) + fabs(float(EndY - y));
                }

                __inline FindItem *GetItem(FindItem *p, int cx, int cy, float len)
                {
                    FindItem *result = find_buffer + (find_buffer_index++);

                    result->Set(p, cx, cy, len);

                    return result;
                }

                void clear();

                FindItem *insert(FindItem *pfd, int x, int y);

                virtual void create_sub(int x, int y, int px, int py, StepList &) = 0;

                bool find_path();

            public:

                AStarFindPath(int, int);
                ~AStarFindPath();

                void Init();
            };
        }//namespace findpath
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ASTAR_FINDPATH_INCLUDE

