#include<hgl/algorithm/affect/Affector.h>

    ///////////////////
/*    影响器是一种关键设计，其本质就是通过一个树形的列表，管理一堆可刷新的处理。
        诸如物理上的重力、旋转力、离心力
        色彩上的淡入淡出、色调色相变换
        几何上的缩放、移动、旋转

        等等均可算为影响器对象的一种

        在此建立影响器设计，即是为对应各种复杂变换而设计，同时也可为粒子系统铺路

    //新设计，。。。。。。。
    //计时器和时间进度器也算做是一种影响器
    //所有影响器有自己的名称注册进控制器
    //需要计时器和时间进度器的，自己通过名称向控制器请求取得对应的影响器。
*/

namespace hgl
{
    namespace algorithm
    {
        namespace affect
        {
            /**
            * 控制器构造函数
            */
            AffectorControl::AffectorControl()
            {
                pause = true;
                stop = true;

                hglSetPropertyRead(IsPause, this, AffectorControl::GetPause);
                hglSetPropertyRead(IsStop, this, AffectorControl::GetStop);
                hglSetPropertyRead(IsRun, this, AffectorControl::GetRun);
            }

            /**
            * 添加一个影响器到控制器中
            * @param name 影响器名称(不可重复)
            * @param ao 影响器指针
            * @return 是否添加成功
            */
            bool AffectorControl::Add(const UTF8String &name, AffectorObject *ao)
            {
                if (!ao)
                {
                    LOG_ERROR(U8_TEXT("AffectorControl 添加的新对象指针为空,name:") + name);
                    return(false);
                }

                if (name.IsEmpty())
                {
                    LOG_ERROR(U8_TEXT("AffectorControl 添加的新对象没有名称,addr:") + PointerToHexUTF8String(ao));
                    return(false);
                }

                if (affector_list.Find(name) != -1)
                {
                    LOG_ERROR(U8_TEXT("AffectorControl重复添加的对象,name:\"") + name + U8_TEXT("\",addr:") + PointerToHexUTF8String(ao));
                    return(false);
                }

                ao->SetControl(this);                //关联控制器

                return affector_list.Add(name, ao);    //添加到列表
            }

            /**
            * 从控制器中移除一个影响器
            * @param name 要移除的影响器名称
            * @return 是否移除成功
            */
            bool AffectorControl::Unlink(const UTF8String &name)
            {
                if (name.IsEmpty())
                {
                    LOG_ERROR(OS_TEXT("要解除关联的影响器名称为空"));
                    return(false);
                }

                return affector_list.UnlinkByKey(name);
            }

            /**
            * 从控制器中移除一个影响器
            * @param ao 要移除的影响器指针
            * @return 是否移除成功
            */
            bool AffectorControl::Unlink(AffectorObject *ao)
            {
                if (!ao)
                {
                    LOG_ERROR(OS_TEXT("要移除的影响器指针为空"));
                    return(false);
                }

                return affector_list.UnlinkByValue(ao);
            }

            /**
            * 从控制器中山删除一个影响器
            * @param name 要删除的影响器名称
            * @return 是否删除成功
            */
            bool AffectorControl::Delete(const UTF8String &name)
            {
                if (name.IsEmpty())
                {
                    LOG_ERROR(OS_TEXT("要删除的影响器名称为空"));
                    return(false);
                }

                return affector_list.DeleteByKey(name);
            }

            /**
            * 从控制器中移删除一个影响器
            * @param ao 要删除的影响器指针
            * @return 是否删除成功
            */
            bool AffectorControl::Delete(AffectorObject *ao)
            {
                if (!ao)
                {
                    LOG_ERROR(OS_TEXT("要删除的影响器指针为空"));
                    return(false);
                }

                return affector_list.UnlinkByValue(ao);
            }

            /**
            * 解除所有影响器关联
            */
            void AffectorControl::UnlinkAll()
            {
                affector_list.UnlinkAll();
            }

            /**
            * 删除所有影响器
            */
            void AffectorControl::DeleteAll()
            {
                affector_list.DeleteAll();
                delete_list.DeleteAll();
            }

            /**
            * 销毁一个影响器,添加到待删除列表
            * @param ao 要销毁的影响器对象
            */
            void AffectorControl::Destroy(AffectorObject *ao)
            {
                if (!ao)return;

                if (affector_list.UnlinkByValue(ao))
                    delete_list.Add(ao);
            }

            /**
            * 根据名称取得一个控制器
            * @param name 要取得的影响器名称
            * @return 影响器指针
            * @return NULL 失败
            */
            AffectorObject *AffectorControl::Get(const UTF8String &name)
            {
                AffectorObject *ao;

                if (affector_list.Get(name, ao))
                    return ao;
                else
                    return(nullptr);
            }

#undef AFFECTOR_ENUM_FUNC
#define AFFECTOR_ENUM_FUNC(func_name)    \
            bool AffectorControl::_ ##func_name()    \
            {    \
                affector_list.EnumValue([](AffectorObject *ao){ao->func_name();});   \
                \
                return(true);    \
            }

            AFFECTOR_ENUM_FUNC(Start);
            AFFECTOR_ENUM_FUNC(Pause);
            AFFECTOR_ENUM_FUNC(Resume);
            AFFECTOR_ENUM_FUNC(Stop);
            AFFECTOR_ENUM_FUNC(Update);
#undef AFFECTOR_ENUM_FUNC

            bool AffectorControl::Start()
            {
                pause = false;
                stop = false;

                return _Start();
            }

            bool AffectorControl::Pause()
            {
                pause = true;

                return _Pause();
            }

            bool AffectorControl::Resume()
            {
                pause = false;

                return _Resume();
            }

            bool AffectorControl::Stop()
            {
                pause = true;
                stop = true;

                return _Stop();
            }

            /**
            * 刷新处理，按顺序调用所有的影响器
            */
            void AffectorControl::Update()
            {
                if (pause || stop)return;

                _Update();

                delete_list.DeleteAll();
            }
        }//namespace affect
    }//namespace algorithm
}//namespace hgl
