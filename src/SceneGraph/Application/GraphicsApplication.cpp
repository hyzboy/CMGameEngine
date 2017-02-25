#include<hgl/platform/GraphicsApplication.h>
#include<hgl/platform/Window.h>
#include<hgl/object/FlowControl.h>
#include<hgl/graph/Render.h>
#include<hgl/graph/TileFont.h>
#include<glew/include/GL/glew.h>
#include <hgl/platform/compiler/EventFunc.h>

extern "C"
{
    void InitOpenGLCoreExtensions();
    void ClearOpenGLCoreExtension();
}

namespace openal
{
//    bool InitOpenAL(const u16char *driver_name=0,const u16char *device_name=0,bool=false);     //初始化OpenAL,参数为设备名称,返回AL_TRUE或AL_FALSE
//    void CloseOpenAL();
}//namespace openal

namespace hgl
{
//     void InitKeyConvert();

    namespace graph
    {
        void InitOpenGLDebug();                                                                     //初始化OpenGL调试输出
        void InitRender();                                                                            //初始化渲染器
        void InitSamplerObject(float,float);                                                        //初始化SamplerObject
        void CloseRender();                                                                            //关闭渲染器

        void SetViewport(int,int,int,int);

    /*    namespace gui                                                                                    //初始化GUI
        {
            bool InitGUI(const u16char *);
            void CloseGUI();
        }

        void ClearFont();*/

        /**
        * 图形应用程序基类构造函数
        * @param fc 自定义流程控制器
        */
        GraphicsApplication::GraphicsApplication(FlowControl *fc):BaseApplication()
        {
            if(fc)
                flow=fc;
            else
                flow=new FlowControl;

    //         SetPointer(hfpRootFlowControl,flow);

            hglSetProperty(        FPS, this,GraphicsApplication::GetFPS,GraphicsApplication::SetFPS);
            cur_fps=0;

            win=nullptr;
            wait_active=true;
            visible = true;

            default_font=nullptr;

            app_event_base=new AppEventBase;

            SetEventCall(app_event_base->Resize.OnProc,this,GraphicsApplication,ProcResize);
            SetEventCall(app_event_base->Close.OnProc,this,GraphicsApplication,ProcClose);

            OnResize=nullptr;
            OnClose=nullptr;
        }

        GraphicsApplication::~GraphicsApplication()
        {
            SAFE_CLEAR(default_font);

            graph::CloseRender();

            ClearOpenGLCoreExtension();

            SAFE_CLEAR(win);
            platform::CloseGraphicsPlatform();

    //        openal::CloseOpenAL();

            SAFE_CLEAR(flow);

    /*        gui::CloseGUI();*/

            delete app_event_base;
        }

        void GraphicsApplication::SetFPS(uint fps)
        {
            if(fps==cur_fps)return;

            cur_fps=fps;

            interval_time=double(1.0f)/double(fps);
        }

        bool GraphicsApplication::Init(GraphicsSystemInitInfo *_sii)
        {
            if(!flow)
            {
                LOG_ERROR(OS_TEXT("BaseApplication,流程控制器没有被构造!"));
                return(false);
            }

            if(!BaseApplication::Init(_sii))
                return(false);

    //         hgl::SetPointer(hfpSII,sii);

            ProjectName=sii->info.ProjectName;
            ProjectCode=sii->info.ProjectCode;

            if(!(sii->CheckMultiStartup()))
                return(false);

//             if(sii->CheckSystem)        CheckSystem(true);
    //        if(sii->DisableScreenSave)    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,false,nullptr,SPIF_SENDCHANGE);

    //        openal::InitOpenAL(nullptr,sii->audio.DeviceName,sii->audio.enum_device);

            if(!platform::InitGraphicsPlatform())
            {
                LOG_ERROR(OS_TEXT("Init Platform failed！"));
                return(false);
            }

//             InitKeyConvert();        //位于GLEWInterface.CPP中

            if(_sii->graphics.FullScreen)
            {
                platform::VideoMode vm;

                vm.width=_sii->graphics.Width;
                vm.height=_sii->graphics.Height;

                win=platform::Create(nullptr,&vm,&(_sii->graphics.gl));
            }
            else
            {
                win=platform::Create(_sii->graphics.Width,_sii->graphics.Height,&(_sii->win),&(_sii->graphics.gl));
            }

            if(!win)
            {
                LOG_ERROR(OS_TEXT("创建窗口失败！另一种可能是您的显卡或驱动不支持 OpenGL ")+OSString(_sii->graphics.gl.es?OS_TEXT("ES "):OS_TEXT("Core "))+OSString(_sii->graphics.gl.major)+OS_TEXT(".")+OSString(_sii->graphics.gl.minor));
                return(false);
            }

            wait_active=_sii->WaitActive;

            win->MakeToCurrent();

            InitOpenGLCoreExtensions();        //初始化OpenGL Core扩展管理
                                            //GLEW(至少1.11版本还是)在OpenGL Core模式下的扩展检测是不可用的，所以不要使用原始的glew代码，并一定在glew初始化之前调用InitOpenGLCoreExtensions
            if(glewInit()!=GLEW_OK)
            {
                LOG_ERROR(OS_TEXT("GLEW初始化失败！"));
                return(false);
            }

            if(_sii->graphics.gl.debug)     //如果是OpenGL调试模式
                graph::InitOpenGLDebug();

            graph::InitRender();
            graph::InitSamplerObject(_sii->graphics.gl.texture_max_anistropy,_sii->graphics.gl.texture_lod_bias);
            graph::SetViewport(0,0,_sii->graphics.Width,_sii->graphics.Height);
            graph::Ortho2DMatrix=ortho2d(_sii->graphics.Width,_sii->graphics.Height);

            FPS=_sii->graphics.FPS;

            if(_sii->graphics.fnt.use)
            {
//                 default_font=CreateTileFont(_sii->graphics.fnt.chs_fnt,
//                                             _sii->graphics.fnt.eng_fnt,
//                                             _sii->graphics.fnt.fnt_buf);

    //             SetPointer(hfpFont,default_font);        //设置缺省字体
            }
            else
            {
                LOG_INFO(OS_TEXT("不创建缺省字体！"));
            }

    /*        if(sii->graphics.gui.use)
                gui::InitGUI(sii->graphics.gui.pack);
            else
                PutInfo(u"不使用GUI系统！");
    */
            win->InitProcEvent(app_event_base);        //设置GLFW事件函数回调
            app_event_base->Join(flow->GetEventBase()); //绑定应用事件收发

            return(true);
        }

        void GraphicsApplication::SetStart(FlowObject *fo)
        {
            flow->SetStart(fo);

            ProcActiveObject(fo);
        }

        void GraphicsApplication::ProcActiveObject(FlowObject *fo)
        {
//             SetPointer(hfpFlow,fo);
        }

        void GraphicsApplication::SwapBuffer()
        {
            win->SwapBuffer();                              //交换缓冲区
        }

        void GraphicsApplication::WaitActive()
        {
            if(wait_active)
                win->WaitEvent();                            //Windows版代码是先WaitMessage再glfwPollEvents

            win->PollEvent();
        }

        int GraphicsApplication::Run()
        {
            if(!flow)return(-1);

            double prev_time=0;

            const Matrix4f mv=identity();

            do
            {
                UpdateTime();                //更新时间

                flow->Update();             //流程刷新
                flow->ProcCurState();       //处理流程状态

                if(visible                                      //显示画面
                 &&cur_time-prev_time>=interval_time)           //刷新时间到了
                {
                    prev_time=cur_time;

                    if(win->IsOpen())                           //如果窗口还开着
                    {
                        flow->Draw(&mv);                        //调用流程绘制代码
                        flow->ProcCurState();                   //处理流程状态

                        SwapBuffer();                           //交换缓冲区
                    }
                }
                else
                {
//                     glfwSleep(interval_time-(cur_time-prev_time));
                    WaitTime(interval_time-(cur_time-prev_time));
                }

                WaitActive();
            }
            while(flow->GetState()!=fosExitApp);

            return(0);
        }
    }//namespace graph

    namespace graph
    {
        bool GraphicsApplication::ProcResize(int w,int h)
        {
            graph::SetViewport(0,0,w,h);

            if (w > 0 && h > 0)
            {
                visible = true;
                graph::Ortho2DMatrix = ortho2d(w, h);
            }
            else
            {
                visible = false;
                return(false);
            }

            SafeCallEvent(OnResize,(w,h));

            flow->OnResize(w,h);

            return(true);
        }

        bool GraphicsApplication::ProcClose()
        {
            if(OnClose!=nullptr)
            {
                bool result=OnClose();

                if(!result)
                    return(false);
            }

            flow->ExitApp();

            return(true);
        }
    }//namespace graph
}//namespace hgl
