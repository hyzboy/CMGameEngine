#include"AudioDecode.h"
#include<hgl/PlugIn.h>

using namespace openal;
namespace hgl
{
    /*PlugInManage(    AudioInterface,            //管理器名称，仅此一次，所以随便命名
                    AudioPlugInInterface,    //接口结构名称,来自外部
                    AudioInterfaceCheck,    //检查接口函数名称,要对外用
                    u"Audio",                //插件前缀
                    2);                        //当前所用版本
                    */

    PlugInManage(Audio,"Audio",2);
    PlugInManage(AudioFloat,"Audio",3);
}//namespace hgl
