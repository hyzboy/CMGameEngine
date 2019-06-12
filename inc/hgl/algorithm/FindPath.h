#ifndef HGL_ALGORITHM_ASTAR_FIND_PATH_INCLUDE
#define HGL_ALGORITHM_ASTAR_FIND_PATH_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
    namespace algorithm
    {
        /**
        * 寻路模块基类描述
        */
        class FindPath                                                                                  ///寻路模块基类
        {
        protected:

            int Left, Top;
            int StartX, StartY;
            int EndX, EndY;
            int Width, Height;

            virtual bool find_path() = 0;

        public: //事件

            DefEvent(bool, CheckPoint, (int new_x, int new_y, int old_x, int old_y));                       ///<检查是否可以从old_x,old_y走到new_x,new_y

        public: //属性

            struct StepCoord
            {
                int x, y;

                StepCoord() { x = y = 0; }
                StepCoord(int _x, int _y) { x = _x; y = _y; }
            };

            class StepList :public List<StepCoord>
            {
            public:

                void Add(int x, int y)
                {
                    List<StepCoord>::Add(StepCoord(x, y));
                }

                void Insert(int index, int x, int y)
                {
                    List<StepCoord>::Insert(index, StepCoord(x, y));
                }
            };

            StepList Steps;                                                                             ///<查找的步子结果

            int MaxFind;                                                                                ///<最大查找节点数,默认128

        public: //方法

            FindPath(int w, int h);
            virtual ~FindPath() { Steps.Clear(); }

            void SetStart(int x, int y);                                                                    ///<设置起始点
            void SetEnd(int x, int y);                                                                  ///<设置目标点

            bool Find();                                                                                ///<开始查找
            bool Find(int l, int t, int sx, int sy, int ex, int ey);                                            ///<开始查找
            bool Find(int sx, int sy, int ex, int ey);                                                      ///<开始查找
        };//class FindPath

        /**
        * 寻路算法枚举
        */
        enum FindPathAlgorithml             ///寻路算法枚举
        {
            fpaNone = 0,

            fpaAStar,                       ///<A*四方向
            fpaAStar8,                      ///<A*八方向
            fpaLine,                        ///<直线

            fpaEnd
        };

        typedef FindPathAlgorithml FPA;

        FindPath *CreateFindPath(FPA, int w, int h);                ///<创建寻路模块
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_ASTAR_FIND_PATH_INCLUDE
