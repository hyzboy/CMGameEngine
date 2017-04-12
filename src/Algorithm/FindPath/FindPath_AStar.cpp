#include"FindPath_AStar.h"

namespace hgl
{
    namespace algorithm
    {
        namespace findpath
        {
            AStarFindPath::AStarFindPath(int w, int h) :FindPath(w, h)
            {
                int n = h;

                find = new FindItem **[h];

                while (n--)
                {
                    find[n] = new FindItem *[w];
                    memset(find[n], 0, w * sizeof(FindItem *));
                }

                find_buffer = new FindItem[w*h];
                find_buffer_index = 0;
            }

            AStarFindPath::~AStarFindPath()
            {
                int n = Height;

                while (n--)
                    delete[] find[n];

                delete[] find;
                delete[] find_buffer;

                findlist.Clear();
            }

            void AStarFindPath::clear()
            {
                int n = Height;

                while (n--)
                    memset(find[n], 0, Width * sizeof(FindItem *));

                findlist.ClearData();
                find_buffer_index = 0;
            }

            void AStarFindPath::Init()
            {
                FindItem *fd = GetItem(nullptr, StartX, StartY, comp_length(StartX, StartY));

                findlist.Add(fd);

                find[StartY][StartX] = fd;
            }

            FindItem *AStarFindPath::insert(FindItem *pfd, int x, int y)
            {
                FindItem *fd = GetItem(pfd, x, y, comp_length(x, y));

                int n = findlist.GetCount();

                while (n--)
                {
                    if (fd->length <= findlist[n]->length)                    //按从远到近排序
                    {
                        findlist.Insert(n, fd);
                        return(fd);
                    }
                }

                findlist.Insert(0, fd);

                return fd;
            }

            bool AStarFindPath::find_path()
            {
                clear();
                Init();

                int max_find = MaxFind;
                FindItem *cur, *prev;
                int n, sn;
                float base;
                int min_step;

                List<FindItem *> cur_find;        //当前查找列表
                StepList sub_list;        //子列表

                while (findlist.GetCount() > 0)
                {
                    n = findlist.GetCount() - 1;

                    if (n < 0)return(false);

                    base = findlist[n]->length;            //以最近点为必要的查找点
                    min_step = findlist[n]->step;

                    cur_find.ClearData();                    //当前查找列表
                    sub_list.ClearData();                    //子列表

                    n = findlist.GetCount();
                    while (n--)
                    {
                        FindItem *fi = findlist[n];

                        if (fi->length <= base)            //取和最近点一样距离的所有点放入当前查找列表
                        {
                            findlist.Delete(n);

                            cur_find += fi;
                        }

                        if (fi->step < min_step)
                            min_step = fi->step;            //记录所有点中的最近步数
                    }

                    if (max_find < min_step)return(false);    //最短步数都太远了，直接退出不找了

                    n = cur_find.GetCount();

                    while (n--)
                    {
                        cur = cur_find[n];

                        sub_list.ClearData();

                        if (cur->prev)prev = cur->prev;
                        else prev = cur;

                        create_sub(cur->x, cur->y, prev->x, prev->y, sub_list);        //创建子查找节点

                        sn = sub_list.GetCount();
                        if (sn <= 0)continue;

                        while (sn--)
                        {
                            int r = sub_list[sn].y;
                            int c = sub_list[sn].x;

                            if (!checkpoint(c, r, cur->x, cur->y))continue;            //这个点能不能走

                            if (find[r][c])                                        //这个位置已经查过了
                            {
                                if (cur->prev)
                                    if (find[r][c]->step < cur->prev->step)        //这个点的步数比之前的短
                                    {
                                        cur->prev = find[r][c];

                                        cur->step = cur->prev->step + 1;
                                    }

                                continue;
                            }

                            if (r == EndY&&c == EndX)
                            {
                                Steps.Add(EndX, EndY);

                                while (cur)
                                {
                                    Steps.Add(cur->x, cur->y);

                                    cur = cur->prev;
                                }

                                findlist.ClearData();

                                return(true);
                            }
                            else
                            {
                                find[r][c] = insert(cur, c, r);

                                cur->sub += find[r][c];
                            }
                        }

                        {
                            sn = cur->sub.GetCount();
                            findpath::FindItem **fi = cur->sub.GetData();

                            while (sn--)
                            {
                                (*fi)->step = cur->step + 1;
                                ++fi;
                            }
                        }
                    }
                }//while(findlist.GetCount()>0)

                return(false);
            }//bool AStarFindPath::find_path()

            class AStar4FindPath :public AStarFindPath
            {
                bool hv;

            public:

                void create_sub(int x, int y, int px, int py, StepList &l)
                {
                    //确定查找顺序
                    if (x == px)        //优先横向
                    {
                        if (EndX > x)
                        {
                            l.Add(x + 1, y);

                            if (EndY > y) { l.Add(x, y + 1); l.Add(x, y - 1); l.Add(x - 1, y); }
                            else
                            {
                                l.Add(x, y - 1); l.Add(x, y + 1); l.Add(x - 1, y);
                            }
                        }
                        else
                        {
                            l.Add(x - 1, y);

                            if (EndY > y) { l.Add(x, y + 1); l.Add(x, y - 1); l.Add(x + 1, y); }
                            else
                            {
                                l.Add(x, y - 1); l.Add(x, y + 1); l.Add(x + 1, y);
                            }
                        }
                    }
                    else        //优先纵向
                    {
                        if (EndY > y)
                        {
                            l.Add(x, y + 1);

                            if (EndX > x) { l.Add(x + 1, y); l.Add(x - 1, y); l.Add(x, y - 1); }
                            else
                            {
                                l.Add(x - 1, y); l.Add(x + 1, y); l.Add(x, y - 1);
                            }
                        }
                        else
                        {
                            l.Add(x, y - 1);

                            if (EndX > x) { l.Add(x + 1, y); l.Add(x - 1, y); l.Add(x, y + 1); }
                            else
                            {
                                l.Add(x - 1, y); l.Add(x + 1, y); l.Add(x, y + 1);
                            }
                        }
                    }
                }

            public:

                using AStarFindPath::AStarFindPath;
            };//class AStar4FindPath

            class AStar8FindPath :public AStarFindPath
            {
            public:

                void create_sub(int x, int y, int, int, StepList &l)
                {
                    int r, c;

                    for (r = y - 1; r <= y + 1; r++)
                        for (c = x - 1; c <= x + 1; c++)
                        {
                            if (r == y&&c == x)continue;    //原点不放入

                            if (r == y || c == x)//优先搜索 横向 和纵向
                                l.Add(c, r);
                            else
                                l.Insert(0, c, r);
                        }
                }

            public:

                using AStarFindPath::AStarFindPath;
            };//class AStar8FindPath

            FindPath *CreateAStar4(int w, int h)
            {
                return(new AStar4FindPath(w, h));
            }

            FindPath *CreateAStar8(int w, int h)
            {
                return(new AStar8FindPath(w, h));
            }
        }//namespace findpath
    }//namespace algorithm
}//namespace hgl

