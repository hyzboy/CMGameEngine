#ifndef HGL_WORKTEAM_WORK_OBJECT_INCLUDE
#define HGL_WORKTEAM_WORK_OBJECT_INCLUDE

namespace hgl
{
    /**
     * 多线程工作组名字空间
     */
    namespace workteam
    {
        /**
         * 具体工具对象<br>
         * 可以被添加到WorkThread中，也可以添加到WorkTeam中
         */
        class WorkObject
        {
        public:

            WorkObject()=default;
            virtual ~WorkObject()=default;

            virtual void Update()=0;
        };//class WorkObject
    }//namespace workteam
}//namespace hgl
#endif//HGL_WORKTEAM_WORK_OBJECT_INCLUDE
