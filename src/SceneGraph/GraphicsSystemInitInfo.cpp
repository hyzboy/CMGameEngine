#include<hgl/platform/GraphicsSystemInitInfo.h>
#include<hgl/proc/ProcMutex.h>
#include<hgl/Other.h>
#include<hgl/File.h>
#include<hgl/LogInfo.h>
#include<glfw3.h>

namespace hgl
{
	/**
	* 系统初始化信息类构造函数
	*/
	GraphicsSystemInitInfo::GraphicsSystemInitInfo()
	{
		WaitActive=false;
		BackNotDraw=true;
		DisableScreenSave=true;

		UseJoystick=false;

		KeyPressTime=100;

		{
			graphics.FPS=60;

			graphics.Width=640;
			graphics.Height=480;

			graphics.Bit=32;
			graphics.VSync=75;

			graphics.FullScreen=false;

			{
				graphics.safe.Width=640;
				graphics.safe.Height=480;
			}

			{
				graphics.gl.AlphaBits=0;
				graphics.gl.DepthBits=0;
				graphics.gl.StencilBits=0;
				graphics.gl.AccumBits=0;
				graphics.gl.MultiSample=0;

				graphics.gl.NicestTextureCompress=true;

				graphics.gl.texture_rectangle=true;
				graphics.gl.texture_non_power_of_two=true;
				graphics.gl.vbo=true;
				graphics.gl.pbo=true;
				graphics.gl.fbo=true;
				graphics.gl.glsl=true;
				graphics.gl.multi_texture=true;

#ifdef HGL_OPENGL_USE_DSA
				graphics.gl.major=4;
				graphics.gl.minor=5;
#else
				graphics.gl.major=4;
				graphics.gl.minor=5;
#endif//HGL_OPENGL_USE_DSA

				graphics.gl.debug=false;
				graphics.gl.opengl_es=false;
			}

			{
        		graphics.gui.use=false;

				graphics.gui.pack=U16_TEXT("AquaGUI.HAC");
			}

			{

				{
					graphics.fnt.use=false;

					graphics.fnt.eng_fnt.Name=U16_TEXT("Tahoma");

					graphics.fnt.eng_fnt.Width=0;
					graphics.fnt.eng_fnt.Height=14;

					graphics.fnt.eng_fnt.Bold=true;
					graphics.fnt.eng_fnt.Italic=false;
				}

				{
					graphics.fnt.chs_fnt.Name=U16_TEXT("黑体");

					graphics.fnt.chs_fnt.Width=0;
					graphics.fnt.chs_fnt.Height=14;

					graphics.fnt.chs_fnt.Bold=false;
					graphics.fnt.chs_fnt.Italic=false;
				}

				graphics.fnt.fnt_buf=1000;
			}
		}

		{
			audio.enum_device=false;
			audio.DeviceName.Clear();
		}

		{
        	win.Edge=true;

			win.SysMenu=true;
			win.Right=false;

			win.Resize=false;
			win.Minimize=true;
			win.Maximize=false;

			win.TopMost=false;
			win.AppTaskBar=true;
		}
	}

	/**
	* 传入命令行参数
	* @param args 命令行参数
	*/
	void GraphicsSystemInitInfo::Init(const OSStringList &args)
	{
		ConsoleSystemInitInfo::Init(args);

		if(args.GetCount()<1)return;

		int pos;

		if(args.Find(OS_TEXT("-joystick"))!=-1)
			UseJoystick=true;

		if(args.Find(OS_TEXT("-no_enum_audio"))!=-1)
			audio.enum_device=false;

// 		pos=args.Find(OS_TEXT("-audio_device"));
// 		if(pos!=-1)
// 			audio.DeviceName=args[pos+1];

		if(args.Find(OS_TEXT("-top_most"))!=-1)
			win.TopMost=true;

		if(args.Find(OS_TEXT("-no_app_task_bar"))!=-1)
			win.AppTaskBar=false;

// 		if(args.Find(OS_TEXT("-fullscreen"))!=-1)
// 		{
// 			int w,h,b,v;
//
// 			graphics.FullScreen=true;
//
// 			if(GetDesktopDisplayMode(w,h,b,v))
// 			{
// 				graphics.Width	=w;
// 				graphics.Height	=h;
// 				graphics.Bit	=b;
// 				graphics.VSync	=v;
// 			}
// 		}

		pos=args.Find(OS_TEXT("-res"));
		if(pos!=-1)
		{
			stoi(args[pos+1].c_str(),graphics.Width);
			stoi(args[pos+2].c_str(),graphics.Height);
		}

		pos=args.Find(OS_TEXT("-bit"));
		if(pos!=-1)
			stoi(args[pos+1].c_str(),graphics.Bit);

		pos=args.Find(OS_TEXT("-vsync"));
		if(pos!=-1)
			stoi(args[pos+1].c_str(),graphics.VSync);

// 		pos=args.Find(OS_TEXT("-chsfnt"));
// 		if(pos!=-1)
// 			graphics.fnt.chs_fnt.Name=args[pos+1];
//
// 		pos=args.Find(OS_TEXT("-engfnt"));
// 		if(pos!=-1)
// 			graphics.fnt.eng_fnt.Name=args[pos+1];

		pos=args.Find(OS_TEXT("-no_vbo"));
		if(pos!=-1)
			graphics.gl.vbo=false;

		pos=args.Find(OS_TEXT("-no_fbo"));
		if(pos!=-1)
			graphics.gl.fbo=false;

		pos=args.Find(OS_TEXT("-no_pbo"));
		if(pos!=-1)
			graphics.gl.pbo=false;

		pos=args.Find(OS_TEXT("-no_glsl"));
		if(pos!=-1)
			graphics.gl.glsl=false;

		pos=args.Find(OS_TEXT("-no_rect"));
		if(pos!=-1)
			graphics.gl.texture_rectangle=false;

		pos=args.Find(OS_TEXT("-no_npot"));
		if(pos!=-1)
			graphics.gl.texture_non_power_of_two=false;
	}

	/**
	* 处理未填写的值
	*/
	void GraphicsSystemInitInfo::ProcBlankValue()
	{
		ConsoleSystemInitInfo::ProcBlankValue();

		if(graphics.Width==0&&graphics.Height==0)
		{
			graphics.Width=640;
			graphics.Height=480;
		}

#if HGL_OS == HGL_OS_Windows
		if(win.ClassName.IsEmpty())
			win.ClassName=L"CMGDK Window Class";
#endif//HGL_OS == HGL_OS_Windows

		if(win.Name.IsEmpty())
			win.Name=info.ProjectName;
	}
}//namespace hgl

