#include<hgl/platform/GraphicsApplication.h>
#include<hgl/platform/GraphicsSystemInitInfo.h>
#include<hgl/object/FlowControl.h>
#include<hgl/graph/Render.h>
#include<hgl/graph/TileFont.h>
#include<glew/include/GL/glew.h>
#include<GLFW/glfw3.h>

extern "C"
{
	void InitOpenGLCoreExtensions();
	void ClearOpenGLCoreExtension();
}

namespace openal
{
//	bool InitOpenAL(const u16char *driver_name=0,const u16char *device_name=0,bool=false);     //初始化OpenAL,参数为设备名称,返回AL_TRUE或AL_FALSE
//	void CloseOpenAL();
}//namespace openal

namespace hgl
{
    namespace opengl_debug_message
    {
        const char *source_name[]=
        {
            "API",
            "Windows System",
            "Shader Compiler",
            "Third Party",
            "Application",
            "Other"
        };

        const char *type_name[]=
        {
            "error",
            "Deprecated Behavior",
            "Undefined Behavior",
            "Portability",
            "Performance",
            "Other",
            "Marker",
            "Push Group",
            "Pop Group",
        };

        struct severity_info
        {
            GLenum severity;
            char str[32];
        };

        const severity_info severity_name[]=
        {
            {GL_DEBUG_SEVERITY_NOTIFICATION ,"notification"},
            {GL_DEBUG_SEVERITY_HIGH         ,"high"},
            {GL_DEBUG_SEVERITY_MEDIUM       ,"medium"},
            {GL_DEBUG_SEVERITY_LOW          ,"low"},
            {0,"unkown"}
        };

        void GLAPIENTRY OpenGLDebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void * /*userParam*/)
        {
            UTF8String severity_string;

            for(int i=0;;i++)
            {
                if(severity_name[i].severity==severity
                 ||severity_name[i].severity==0)
                {
                    severity_string=severity_name[i].str;
                    break;
                }
            }

            LOG_INFO(U8_TEXT("OpenGL DebugMessage: source[")
                    +UTF8String(source_name[source-GL_DEBUG_SOURCE_API_ARB])
                    +U8_TEXT("] type[")
                    +UTF8String(type_name[type-GL_DEBUG_TYPE_ERROR_ARB])
                    +U8_TEXT("] severity[")
                    +severity_string
                    +U8_TEXT("] message:")+UTF8String(message,length));
        }
    }//opengl_debug_message

    namespace opengl_debug_message_amd
    {
        const char *category_name[]=
        {
            "API",
            "Windows System",
            "Deprecation",
            "Undefined Behavior",
            "Performance",
            "Shader Compiler",
            "Application",
            "Other"
        };

        const char *severity_name[]=
        {
            "high",
            "medium",
            "low"
        };

        void GLAPIENTRY OpenGLDebugProc(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void * /*userParam*/)
        {
            LOG_INFO(U8_TEXT("OpenGL DebugMessage: id[")
                    +UTF8String(id)
                    +U8_TEXT("] category[")
                    +UTF8String(category_name[category-GL_DEBUG_CATEGORY_API_ERROR_AMD])
                    +U8_TEXT("] severity[")
                    +UTF8String(severity_name[severity-GL_DEBUG_SEVERITY_HIGH_AMD])
                    +U8_TEXT("] message:")+UTF8String(message,length));
        }
    }//namespace opengl_debug_message_amd

// 	void InitKeyConvert();

	namespace graph
	{
		GraphicsApplication *graphics_application=nullptr;

		void InitRender();																			//初始化渲染器
		void CloseRender();																			//关闭渲染器

		void SetViewport(int,int,int,int);

	/*	namespace gui																					//初始化GUI
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

	// 		SetPointer(hfpRootFlowControl,flow);

			hglSetProperty(		FPS, this,GraphicsApplication::GetFPS,GraphicsApplication::SetFPS);
			cur_fps=0;

			glfw_win=nullptr;
			wait_active=true;

			default_font=nullptr;

			OnResize=nullptr;
			OnClose=nullptr;

			SetEventCall(flow->OnChange,this,GraphicsApplication,ProcActiveObject);

			graphics_application=this;
		}

		GraphicsApplication::~GraphicsApplication()
		{
			graphics_application=nullptr;

			SAFE_CLEAR(default_font);

			graph::CloseRender();

			ClearOpenGLCoreExtension();

			if(glfw_win)
				glfwDestroyWindow(glfw_win);
			glfwTerminate();

	//		openal::CloseOpenAL();

			SAFE_CLEAR(flow);

	/*		gui::CloseGUI();*/
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

	// 		hgl::SetPointer(hfpSII,sii);

			ProjectName=sii->info.ProjectName;
			ProjectCode=sii->info.ProjectCode;

			if(!(sii->CheckMultiStartup()))
				return(false);

// 			if(sii->CheckSystem)		CheckSystem(true);
	//		if(sii->DisableScreenSave)	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,false,nullptr,SPIF_SENDCHANGE);

	//		openal::InitOpenAL(nullptr,sii->audio.DeviceName,sii->audio.enum_device);

			if(!glfwInit())
			{
				LOG_ERROR(OS_TEXT("GLFW初始化失败！"));
				return(false);
			}

// 			InitKeyConvert();		//位于GLEWInterface.CPP中

			glfwWindowHint(GLFW_RESIZABLE,				_sii->win.Resize);
			glfwWindowHint(GLFW_REFRESH_RATE,			_sii->graphics.VSync);
			glfwWindowHint(GLFW_SAMPLES,				_sii->graphics.gl.MultiSample);

			glfwWindowHint(GLFW_CLIENT_API,				_sii->graphics.gl.opengl_es?GLFW_OPENGL_ES_API:GLFW_OPENGL_API);

			glfwWindowHint(GLFW_OPENGL_PROFILE,			GLFW_OPENGL_CORE_PROFILE);			//核心模式
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,	true);								//向前兼容模式(无旧特性)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,	_sii->graphics.gl.debug);			//调试模式
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,	_sii->graphics.gl.major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,	_sii->graphics.gl.minor);

			glfwWindowHint(GLFW_VISIBLE,				true);								//是否显示

			if(_sii->graphics.gl.AlphaBits	>0)glfwWindowHint(GLFW_ALPHA_BITS	,_sii->graphics.gl.AlphaBits	);
			if(_sii->graphics.gl.DepthBits	>0)glfwWindowHint(GLFW_DEPTH_BITS	,_sii->graphics.gl.DepthBits	);
			if(_sii->graphics.gl.StencilBits>0)glfwWindowHint(GLFW_STENCIL_BITS	,_sii->graphics.gl.StencilBits	);

			glfw_win=glfwCreateWindow(	_sii->graphics.Width,
										_sii->graphics.Height,
										_sii->win.Name,
										NULL,//glfwGetPrimaryMonitor(),
										NULL);

			if(!glfw_win)
			{
				LOG_ERROR(OS_TEXT("创建窗口失败！另一种可能是您的显卡或驱动不支持 OpenGL Core ")+OSString(_sii->graphics.gl.major)+OS_TEXT(".")+OSString(_sii->graphics.gl.minor));
				return(false);
			}

			wait_active=_sii->WaitActive;

			glfwMakeContextCurrent(glfw_win);

			InitOpenGLCoreExtensions();		//初始化OpenGL Core扩展管理
											//GLEW(至少1.11版本还是)在OpenGL Core模式下的扩展检测是不可用的，所以不要使用原始的glew代码，并一定在glew初始化之前调用InitOpenGLCoreExtensions
			if(glewInit()!=GLEW_OK)
			{
				LOG_ERROR(OS_TEXT("GLEW初始化失败！"));
				return(false);
			}

            if(_sii->graphics.gl.debug)     //如果是OpenGL调试模式
            {
                if(GLEW_VERSION_4_3||GLEW_KHR_debug)
                {
                    glDebugMessageCallback(opengl_debug_message::OpenGLDebugProc,nullptr);

                    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0,nullptr,GL_FALSE);
                }
                else
                if(GLEW_ARB_debug_output)
                {
                    glDebugMessageCallbackARB(opengl_debug_message::OpenGLDebugProc,nullptr);

                    glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DEBUG_SEVERITY_NOTIFICATION, 0,nullptr,GL_FALSE);
                }
                else
                if(GLEW_AMD_debug_output)
                {
                    glDebugMessageCallbackAMD(opengl_debug_message_amd::OpenGLDebugProc,nullptr);

                    glDebugMessageEnableAMD(GL_DEBUG_CATEGORY_API_ERROR_AMD, GL_DEBUG_LOGGED_MESSAGES_AMD, 0,nullptr,GL_FALSE);
                }
            }

			graph::InitRender();
			graph::SetViewport(0,0,_sii->graphics.Width,_sii->graphics.Height);
            graph::Ortho2DMatrix=ortho2d(_sii->graphics.Width,_sii->graphics.Height);

			FPS=_sii->graphics.FPS;

			if(_sii->graphics.fnt.use)
			{
// 				default_font=CreateTileFont(_sii->graphics.fnt.chs_fnt,
// 											_sii->graphics.fnt.eng_fnt,
// 											_sii->graphics.fnt.fnt_buf);

	// 			SetPointer(hfpFont,default_font);		//设置缺省字体
			}
			else
			{
				LOG_INFO(OS_TEXT("不创建缺省字体！"));
			}

	/*		if(sii->graphics.gui.use)
				gui::InitGUI(sii->graphics.gui.pack);
			else
				PutInfo(u"不使用GUI系统！");
	*/
			InitProcEvent();		//设置GLFW事件函数回调

			return(true);
		}

		void GraphicsApplication::SetStart(FlowObject *fo)
		{
			flow->SetStart(fo);

			ProcActiveObject(fo);
		}

		void GraphicsApplication::ProcActiveObject(FlowObject *fo)
		{
// 			SetPointer(hfpFlow,fo);
		}

        void GraphicsApplication::SwapBuffer()
        {
            glfwSwapBuffers(glfw_win);              //交换缓冲区
        }

		void GraphicsApplication::WaitActive()
		{
			if(wait_active)
				glfwWaitEvents();							//Windows版代码是先WaitMessage再glfwPollEvents
			else
				glfwPollEvents();
		}

		int GraphicsApplication::Run()
		{
			if(!flow)return(-1);

			double prev_time=0;

			const Matrix4f mv=identity();

			do
			{
				UpdateTime();				//更新时间

				flow->Update();             //流程刷新
				flow->ProcCurState();       //处理流程状态

				if(cur_time-prev_time>=interval_time)			//刷新时间到了
				{
					prev_time=cur_time;

					if(!glfwWindowShouldClose(glfw_win))		//如果窗口还开着
					{
						flow->Draw(&mv);						//调用流程绘制代码
						flow->ProcCurState();					//处理流程状态

                        SwapBuffer();                           //交换缓冲区
					}
				}
				else
				{
// 					glfwSleep(interval_time-(cur_time-prev_time));
					WaitTime(interval_time-(cur_time-prev_time));
				}

				WaitActive();
			}
			while(flow->ObjectState!=fosExitGame);

			return(0);
		}
	}//namespace graph

	namespace graph
	{
		bool GraphicsApplication::Proc_CursorPos(int x,int y)
		{
			return flow->Proc_CursorPos(x,y);
		}

		bool GraphicsApplication::Proc_Scroll(int x,int y)
		{
			return flow->Proc_Scroll(x,y);
		}

		#define PROC(func_name,type)	bool GraphicsApplication::func_name(type key,bool press)	\
		{	\
			return flow->func_name(key,press);	\
		}

		PROC(Proc_MouseButton	,int);
		PROC(Proc_JoystickButton,int);
		PROC(Proc_Key			,int);
		#undef PROC

		bool GraphicsApplication::Proc_Char				(u16char ch)
		{
			return flow->Proc_Char(ch);
		}

		bool GraphicsApplication::Proc_Event            (int id,void *event_id)
		{
			return flow->Proc_Event(id,event_id);
		}

		void GraphicsApplication::ProcResize(int w,int h)
		{
			graph::SetViewport(0,0,w,h);

			if(w>0&&h>0)
				graph::Ortho2DMatrix=ortho2d(w,h);

			SafeCallEvent(OnResize,(w,h));

			flow->Proc_Resize(w,h);
		}

		bool GraphicsApplication::ProcClose()
		{
			if(OnClose!=nullptr)
			{
				bool result=OnClose();

				if(!result)
					return(false);
			}

			flow->ExitGame();

			return(true);
		}
	}//namespace graph
}//namespace hgl
