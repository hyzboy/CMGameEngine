#ifndef HGL__OBJECT_INCLUDE
#define HGL__OBJECT_INCLUDE

namespace hgl
{
    /**
    * 最终基类,无任何作用
    */
    class _Object                                                                                       ///最终基类
    {
    public:

        virtual ~_Object()=default;                                                                     ///<本类析构函数
    };//class _Object

    typedef void (_Object::*ObjectMemberFunc)();

    //此代码取自AngelScript，感谢
    #define GetMemberFuncPointer(c,m) MethodPtr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))

    template <int> struct MethodPtr
    {
        template<class M>
        static void *Convert(M Mthd)
        {
            union u
            {
                M m;
                void *v;
            };

            return ((u *)(&Mthd))->v;
        }
    };//struct MethodPtr
}//namespace hgl
#endif//HGL__OBJECT_INCLUDE
