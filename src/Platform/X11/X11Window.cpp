#include"X11Window.h"
#include<hgl/InputState.h>

namespace
{
    namespace platform
    {
        static KeyButon KeyConvert[256];

        void InitKeyConvert()
        {
            memset(KeyConvert,0,sizeof(KeyConvert));

            for(int i=0;i<12;i++)
                KeyConvert[XK_F1+i]=kbF1+i;
            for(int i=0;i<10;i++)
                KeyConvert[XK_0+i]=kb0+i;

            for(int i=0;i<26;i++)
            {
                KeyConvert[XK_A+i]=kbA+i;
                KeyConvert[XK_a+i]=kbA+i;
            }

            KeyConvert[XK_Sys_Req		]=kbPrintScreen;
            KeyConvert[XK_Break			]=kbBreak;
            KeyConvert[XK_Menu			]=kbMenu;
            KeyConvert[XK_KP_Enter		]=kbNumEnter;
            KeyConvert[XK_Pause			]=kbPause;
            KeyConvert[XK_Scroll_Lock	]=kbScrollLock;
            KeyConvert[XK_Print			]=kbPrint;
            KeyConvert[XK_Escape		]=kbEsc;
            KeyConvert[XK_BackSpace		]=kbBackSpace;
            KeyConvert[XK_Tab			]=kbTab;
            KeyConvert[XK_Return		]=kbReturn;
            KeyConvert[XK_Caps_Lock		]=kbCapsLock;
            KeyConvert[XK_Shift_L		]=kbLeftShift;
            KeyConvert[XK_Shift_R		]=kbRightShift;
            KeyConvert[XK_Control_L		]=kbLeftCtrl;
            KeyConvert[XK_Control_R		]=kbRightCtrl;
            KeyConvert[XK_Alt_L			]=kbLeftAlt;
            KeyConvert[XK_Alt_R			]=kbRightAlt;
    //		KeyConvert[XK_Hyper_L		]:
    //		KeyConvert[XK_Hyper_R		]=kbHYPER;
            KeyConvert[XK_Insert		]=kbInsert;
            KeyConvert[XK_Home			]=kbHome;
            KeyConvert[XK_Page_Up		]=kbPageUp;
            KeyConvert[XK_Delete		]=kbDelete;
            KeyConvert[XK_End			]=kbEnd;
            KeyConvert[XK_Page_Down		]=kbPageDown;
            KeyConvert[XK_Left			]=kbLeft;
            KeyConvert[XK_Right			]=kbRight;
            KeyConvert[XK_Up			]=kbUp;
            KeyConvert[XK_Down			]=kbDown;
            KeyConvert[XK_Num_Lock		]=kbNumLock;
            KeyConvert[XK_KP_Add		]=kbNumAdd;
            KeyConvert[XK_KP_Subtract	]=kbNumMinus;
            KeyConvert[XK_KP_Multiply	]=kbNumMultiply;
            KeyConvert[XK_KP_Divide		]=kbNumDivide;
            KeyConvert[XK_KP_Home		]=kbNum7;
            KeyConvert[XK_KP_Up			]=kbNum8;
            KeyConvert[XK_KP_Page_Up	]=kbNum9;
            KeyConvert[XK_KP_Left		]=kbNum4;
            KeyConvert[XK_KP_5			]=kbNum5;
            KeyConvert[XK_KP_Right		]=kbNum6;
            KeyConvert[XK_KP_End		]=kbNum1;
            KeyConvert[XK_KP_Down		]=kbNum2;
            KeyConvert[XK_KP_Page_Down	]=kbNum3;
            KeyConvert[XK_KP_Insert		]=kbNum0;
            KeyConvert[XK_KP_Delete		]=kbNumDecimal;

    //		KeyConvert[XK_KP_Space		]=
            KeyConvert[XK_space			]=kbSpace;
    //		KeyConvert[XK_parenright	]=kbRIGHT_PARENTHESIS;

    //		KeyConvert[XK_exclam		]=kbEXCLAM;
    //		KeyConvert[XK_at			]=kbAT;
    //		KeyConvert[XK_numbersign	]=kbNUMBER;
    //		KeyConvert[XK_dollar		]=kbDOLLAR;
    //		KeyConvert[XK_percent:
    //		KeyConvert[XK_asciicircumC	]=kbIRCUMFLEX;
    //		KeyConvert[KEY_PERCENT		]
    //		KeyConvert[XK_ampersand		]=kbAMPERSAND;
    //		KeyConvert[XK_asterisk		]=kbASTERISK;
    //		KeyConvert[XK_parenleft		]=kbLEFT_PARENTHESIS;
            KeyConvert[XK_equal			]=kbEqual;
    //		KeyConvert[XK_plus			]=kbPlus;
    //		KeyConvert[XK_less			]=kbLESS_THAN;
            KeyConvert[XK_comma			]=kbComma;
    //		KeyConvert[XK_underscore	]=kbUNDERSCORE;
            KeyConvert[XK_minus			]=kbMinus;
    //		KeyConvert[XK_greater		]=kbGREATER_THAN;
            KeyConvert[XK_period		]=kbPeriod;
    //		KeyConvert[XK_colon			]=kbCOLON;
            KeyConvert[XK_semicolon		]=kbSemicolon;
    //		KeyConvert[XK_question		]=kbQUESTION;
    //		KeyConvert[XK_slash			]=kbSLASH;
    //		KeyConvert[XK_grave			]=kbGRAVE;
    //		KeyConvert[XK_asciitilde	]=kbTILDE;
    //		KeyConvert[XK_braceleft		]=kbLeftBrace;
            KeyConvert[XK_bracketleft	]=kbLeftBracket;
    //		KeyConvert[XK_bar			]=kbBAR;
            KeyConvert[XK_backslash		]=kbBackSlash;
    //		KeyConvert[XK_braceright	]=kbRIGHT_BRACE;
            KeyConvert[XK_bracketright	]=kbRightBracket;
    //		KeyConvert[XK_quotedbl		]=kbQUOTE;
            KeyConvert[XK_apostrophe	]=kbApostrophe;
    //		KeyConvert[XK_EuroSign		]=kbEURO;
    //		KeyConvert[XK_sterling		]=kbPOUND;
    //		KeyConvert[XK_yen			]=kbYEN;
    //		KeyConvert[XK_periodcentered]=kbMIDDLE_DOT;
        }
    }//namespace platform

    namespace platform
    {
        X11Window::X11Window()
        {
            disp=nullptr;
            win=nullptr;
            ctx=nullptr;
        }

        X11Window::~X11Window()
        {
            Close();
        }

        bool X11Window::CreateWindow(const char *win_name,int w,int h,const OpenGLSetup *gs)
        {
            if(disp)
                return(false);

            disp=XOpenDisplay(nullptr);

            if(!disp)
            {
                LOG_ERROR(OS_TEXT("Failed to open X Display"));
                return(false);
            }

            int glx_major,glx_minor;

            glXQueryVersion(disp,&glx_major,&glx_minor);

            if(glx_major<1||
            (glx_major==1&&glx_minor<2))
            {
                LOG_ERROR(OS_TEXT("GLX 1.2 or greater is required."));
                return(false);
            }

            default_screen=DefaultScreen(disp);

            atomFullscreen	= XInternAtom(disp,"_NET_WM_STATE_FULLSCREEN",	True);
            atomState		= XInternAtom(disp,"_NET_WM_STATE",				True);
            atomDeleteWindow= XInternAtom(disp,"WM_DELETE_WINDOW",			True);

            static int visual_attribs[] =
            {
                GLX_X_RENDERABLE,	True,
                GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
                GLX_RENDER_TYPE,	GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
                GLX_RED_SIZE,		8,
                GLX_GREEN_SIZE,		8,
                GLX_BLUE_SIZE,		8,
                GLX_ALPHA_SIZE,		8,
                GLX_DEPTH_SIZE,		24,
                GLX_STENCIL_SIZE,	8,
                GLX_DOUBLEBUFFER,	True,
                //GLX_SAMPLE_BUFFERS  , 1,
                //GLX_SAMPLES         , 4,
                None
            };

            int fbcount;
            GLXFBConfig *fbc = glXChooseFBConfig(disp,default_screen,visual_attribs, &fbcount);

            if(!fbc)
            {
                LOG_ERROR(OS_TEXT("Failed to retrieve a framebuffer config"));
                return(false);
            }

            int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

            for(int i=0;i<fbcount;i++)
            {
                XVisualInfo *vi=glXGetVisualFromFBConfig(disp, fbc[i]);

                if (vi)
                {
                    int samp_buf, samples;
                    glXGetFBConfigAttrib(disp, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                    glXGetFBConfigAttrib(disp, fbc[i], GLX_SAMPLES, &samples);

                    if (best_fbc < 0 || (samp_buf && samples) > best_num_samp)
                        best_fbc = i, best_num_samp = samples;
                    if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                        worst_fbc = i, worst_num_samp = samples;
                }
                XFree(vi);
            }

            // Get a visual
            int fbc_id = best_fbc;
            //int fbc_id = worst_fbc;

            XVisualInfo *vi = glXGetVisualFromFBConfig(disp, fbc[ fbc_id ]);
            //printf("Chosen visual ID = 0x%x\n", vi->visualid);

            //printf("Creating colormap\n");
            XSetWindowAttributes swa;

            swa.colormap=XCreateColormap(	disp,
                                            RootWindow(disp,vi->screen),
                                            vi->visual,
                                            AllocNone);
            swa.background_pixmap	=None;
            swa.border_pixel		=0;
            swa.event_mask			=ExposureMask
                                    |VisibilityChangeMask
                                    |KeyPressMask
                                    |KeyReleaseMask
                                    |ButtonPressMask
                                    |ButtonReleaseMask
                                    |PointerMotionMask
                                    |StructureNotifyMask
                                    |SubstructureNotifyMask
                                    |FocusChangeMask;

            //printf("Creating window\n");
            win = XCreateWindow(disp,
                                RootWindow(disp,vi->screen),	//parent 父窗口
                                0, 0,			//x,y
                                w, h,
                                0,				//border_width
                                vi->depth,
                                InputOutput,
                                vi->visual,
                                CWBorderPixel|CWColormap|CWEventMask,
                                &swa);
            if (!win)
            {
                LOG_ERROR(OS_TEXT("Failed to create window."));
                return false;
            }

            XMapWindow(dis,win);
            XStoreName(dis,win,win_name);
            XSetWMProtocols(disp,win,&atomDeleteWindow,1);

            ctx=glXCreateContext(disp,vi,nullptr,true);
            if(!ctx)
            {
                LOG_ERROR(OS_TEXT("glXCreateContext return NULL"));
            }

            glXMakeCurrent(disp,win,ctx);

            XFree(vi);
            XFree(fbc);

            if(!glXIsDirect(disp,ctx))
            {
                LOG_ERROR(OS_TEXT("Indirect GLX rendering context obtained"));
                return false;
            }
        }

        bool X11Window::InitToFullScreen(int w, int h, int c,bool tm,const OpenGLSetup *gs)
        {
            if(!CreateWindow(hglGetString(hfsName),w,h))
                return(false);

            XClientMessageEvent message;

            message.type		= ClientMessage;
            message.serial		= 0;
            message.send_event	= True;
            message.window		= win;
            message.message_type= atomState;
            message.format		= 32;
            message.data.l[0]	= 1;
            message.data.l[1]	= atomFullscreen;
            message.data.l[2]	= 0;

            XSendEvent(	disp,
                        DefaultRootWindow(disp),
                        False,
                        SubstructureRedirectMask|SubstructureNotifyMask,
                        (XEvent*)&message);

            XFlush(disp);
        }

        bool X11Window::InitToWindow(int w,int h,const char *win_name,const OpenGLSetup *gs)
        {
            return CreateWindow(win_name,w,h);
        }

        void X11Window::Close()
        {
            if(ctx)
            {
                glXMakeCurrent(disp,0,0);
                glXDestroyContext(disp,ctx);
                ctx=nullptr;
            }

            if(win)
            {
                XDestroyWindow(disp,win);
                win=nullptr;
            }

            if(disp)
            {
                XCloseDisplay(disp);
                disp=nullptr;
            }
        }

        void X11Window::SetCaption(const UTF16String &cap)
        {
            const UTF8String win_name=to_u8(cap);

            XStoreName(dis,win,win_name);

            Window::SetCaption(win_name);
        }

        void X11Window::MakeToCurrent()
        {
            glXMakeCurrent(disp,win,ctx);
        }

        void X11Window::SwapBuffer()
        {
            glXSwapBuffers(disp,win);
        }

        bool X11Window::UpdateXEvent()
        {
            while(XPending(disp))
            {
                XNextEvent(disp,&event);

                switch(event.type)
                {
                    case DestroyNotify:		Close();exit(0);//关闭窗口

                    case Expose:			XGetWindowAttributes(disp,win,&win_attr);
                                            width=win_attr.width;
                                            height=win_attr.height;
                                            break;

                    case ConfigureNotify:	width=event.xconfigure.width;
                                            height=event.xconfigure.height;
                                            break;

                    case MotionNotify:		mouse_x=event.xbutton.x;
                                            mouse_y=event.xbutton.y;
                                            break;

                    case KeyPress:			sym=XLookupKeysym(&(event.xkey),0);
                                            //转换
                                            break;

                    case KeyRelease:		if(XPending(disp))
                                            {
                                                XPeekEvent(disp,&next_event);
                                                if(next_event.type==KeyPress
                                                    &&next_event.xkey.time==event.xkey.time
                                                    &&next_event.xkey.keycode==event.xkey.keycode)
                                                {
                                                    XNextEvent(disp,&next_event);
                                                    continue;
                                                }
                                            }

                                            sym=XLookupKeysym(&(event.xkey),0);
                                            break;

                    case ButtonPress:
                                            if(event.xbutton.button==1)		//MouseLeftPress
                                            if(event.xbutton.button==2)		//MouseMiddlePress
                                            if(event.xbutton.button==3)		//MouseRightPress
                                            if(event.xbutton.button==4)
                                            if(event.xbutton.button==5)		//Scroll
                    case ButtonRelease:
                }
            }
        }
    }//namespace platform

    namespace platform
    {
        Window *Create(int width,int height,const WindowSetup *ws,const OpenGLSetup *gs)
        {
            X11Window *win=new X11Window();

            if(win->InitToWindow(width,height,ws->Name,gs))
                return win;

            return RETURN_ERROR_NULL;
        }

        Window *Create(const Monitor *monitor,const VideoMode *vm,const OpenGLSetup *gs)
        {
            X11Window *win=new X11Window();

            if(win->InitToFullScreen(width,height,ws->Name,gs))
                return win;

            return RETURN_ERROR_NULL;
        }
    }//namespace platform
}//namespace hgl
