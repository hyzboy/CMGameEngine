#include<hgl/HAC.h>
#include<hgl/gl/PBO.h>
#include<hgl/PlugIn.h>
#include<hgl/stream/FileStream.h>
#include<hgl/gl/Texture2D.h>
#include<hgl/gl/shader/GLSL.h>
#include<hgl/audio/AudioPlayer.h>
#include<hgl/movie/TheoraMovie.h>

namespace hgl
{
	const char glsl_yuv2rgb[]=
	{
		"uniform sampler2D tex_y;\n"
		"uniform sampler2D tex_u;\n"
		"uniform sampler2D tex_v;\n"

		"void main()\n"
		"{\n"
		"	float y = texture2D( tex_y, gl_TexCoord[0].xy ).r;\n"
		"	float u = texture2D( tex_u, gl_TexCoord[1].xy ).r;\n"
		"	float v = texture2D( tex_v, gl_TexCoord[2].xy ).r;\n"

		"	if(u == 0.0 && v == 0.0)\n"
		"		y = 0.0;\n"

		"	y=1.1643*(y-0.0625);\n"
		"	u=u-0.5;\n"
		"	v=v-0.5;\n"

		"	gl_FragColor=vec4(	y+1.5958*v,\n"
		"						y-0.39173*u-0.81290*v,\n"
		"						y+2.017*u,\n"
		"						1.0);\n"
		"}\n"
	};

	typedef void MOVIE;
	typedef void (__cdecl *SeekFunc	)(Stream *,int);
	typedef bool (__cdecl *EofFunc	)(Stream *);
	typedef int	 (__cdecl *ReadFunc	)(Stream *,void *,int);

    typedef struct
    {
      int            width;
      int            height;
      int            stride;
      unsigned char *data;
    }th_img_plane;

	struct TheoraMovieInterface
	{
        MOVIE *			(__cdecl *Open			)(Stream *,SeekFunc,EofFunc,ReadFunc);
        bool			(__cdecl *GetInfo		)(MOVIE *,int &,int &,int &);
        void			(__cdecl *SetBuffer		)(MOVIE *,uint);
        double			(__cdecl *CheckUpdate	)(MOVIE *);
        void			(__cdecl *DecodeToRGB	)(MOVIE *,uchar *);
        void			(__cdecl *DecodeToBGR	)(MOVIE *,uchar *);
		void			(__cdecl *DecodeToRGBA	)(MOVIE *,uchar *);
		void			(__cdecl *DecodeToBGRA	)(MOVIE *,uchar *);
        th_img_plane *	(__cdecl *DecodeToYUV	)(MOVIE *,int &,int &);
        bool			(__cdecl *CheckEnd		)(MOVIE *);
        void			(__cdecl *Close			)(MOVIE *);
	};

	void __cdecl Seek(Stream *str,int pos)
	{
        str->Position=pos;
	}

	bool __cdecl Eof(Stream *str)
	{
		return str->Eof();
	}

	int __cdecl Read(Stream *str,void *buf,int size)
	{
		return str->Read(buf,size);
	}

	class MovieDecode:public Thread
	{
		PlugIn *movie_plug_in;
		TheoraMovieInterface *movie_interface;
		void *movie;

	public:

		int width,height,fps;
		int y_width,y_height;
		int uv_width,uv_height;
		PBO *pbo_rgb;
		PBO *pbo_y,*pbo_u,*pbo_v;

		volatile bool play;
		volatile bool frame_update;

		Texture2D *image_rgb;
		uchar *rgb_buffer;

		GLSL *glsl;
		Texture2D *image_y,*image_u,*image_v;
		th_img_plane *yuv;
		int yuv_x,yuv_y;

		float left,top;
		float draw_left,draw_top;
		float draw_width,draw_height;
		float scale;
		float movie_width,movie_height;

	public:

		MovieDecode()
		{
			movie_plug_in	=LoadPlugIn(u"Movie.Theora");
			movie_interface	=nullptr;
            play            =false;
			frame_update	=false;

			pbo_rgb			=nullptr;
			image_rgb		=nullptr;
			rgb_buffer		=nullptr;

			glsl			=nullptr;
			pbo_y			=nullptr;
			pbo_u			=nullptr;
			pbo_v			=nullptr;
			image_y			=nullptr;
			image_u			=nullptr;
			image_v			=nullptr;

			yuv				=nullptr;

			if(!movie_plug_in)
            {
                PutError(u"加载影片解码插件失败！");
                return;
            }

			movie_interface=new TheoraMovieInterface;

			if(!movie_plug_in->GetInterface(2,movie_interface))
			{
				delete movie_interface;
				movie_interface=nullptr;
			}
		}

		~MovieDecode()
		{
        	if(movie_interface)
				movie_interface->Close(movie);

			SAFE_CLEAR(image_rgb);
			SAFE_CLEAR(image_y);
			SAFE_CLEAR(image_u);
			SAFE_CLEAR(image_v);
			SAFE_CLEAR_ARRAY(rgb_buffer);
			SAFE_CLEAR(movie_interface);
			SAFE_CLEAR(pbo_rgb);
			SAFE_CLEAR(pbo_y);
			SAFE_CLEAR(pbo_u);
			SAFE_CLEAR(pbo_v);
			SAFE_CLEAR(glsl);
			UnloadPlugIn(movie_plug_in);
		}

		bool Open(Stream *file)
		{
			if(!movie_interface)
            {
                PutError(u"未找到影片解码插件");
                return(false);
            }

			movie=movie_interface->Open(file,Seek,Eof,Read);

			if(!movie_interface->GetInfo(movie,width,height,fps))
            {
                PutError(u"取得影片基本信息失败");
                return(false);
            }

			movie_interface->SetBuffer(movie,64*1024);

			if(OHS.glsl&&OHS.fs&&OHS.multi_texture>=3)
			{
                glsl=new GLSL;

                if(!glsl->CompileFragment(glsl_yuv2rgb))
                {
                    delete glsl;
                    glsl=nullptr;
                }
			}

            if(!glsl)
			{
				rgb_buffer=new uchar[width*height*4];
			}

			scale=float(GetCanvasWidth())/float(width);

			if(scale*height>GetCanvasHeight())
				scale=float(GetCanvasHeight())/float(height);

			movie_width=scale*width;
			movie_height=scale*height;

			left=(GetCanvasWidth()-movie_width)/2;
			top=(GetCanvasHeight()-movie_height)/2;

			draw_left=0;
			draw_top=0;

			return(true);
		}

		bool Execute()
		{
			double time;

			while(play)
			{
				time=movie_interface->CheckUpdate(movie);

				if(time<0)  //错误或影片结束
                {
                    play=false;
                    break;
                }
                #ifdef _DEBUG
                else
                    PutInfo(u"movie time=%lf",time);
                #endif//_DEBUG

				if(!TryLock())
                {
                    //不等主线程，但也不能一直调check update，不然会占太多cpu

                    time/=2.0f;

                    if(time>0.01)
                        WaitTime(time);		//延时等待

                    continue;
                }

                if(image_rgb)
                    movie_interface->DecodeToRGBA(movie,rgb_buffer);
                else
                    yuv=movie_interface->DecodeToYUV(movie,yuv_x,yuv_y);

                frame_update=true;

                Unlock();

                if(time>0.01)
                    WaitTime(time-0.01);		//延时等待
			}

			play=false;
			Unlock();
			return(false);
		}

        void StartMovie()
        {
            play=true;
			frame_update=false;

            Thread::Start();
        }

		void StopMovie()
		{
        	play=false;
			Thread::Wait();
		}

		bool Update()
		{
            if(!play)
            {
                PutInfo(u"人为中断播放或解码完成");
                return(false);
            }

			if(!TryLock())return(true);

			bool result=true;

			if(frame_update)
			{
				if(rgb_buffer)		//RGB播放
				{
                    DebugLog(u"frame update start,rgb_buffer=%p,image_rgb=%p",rgb_buffer,image_rgb);

					if(!image_rgb)	//第一次
					{
                        DebugLog(u"create texture start!");

						if(OHS.texture_non_power_of_two){DebugLog(u"create npot texture !");image_rgb=new Texture2D  (width,height,GL_RGBA,GL_RGBA8,1);draw_width=1;draw_height=1;}else
														{DebugLog(u"create  pot texture !");image_rgb=new Texture2D(PowerTo2(width),PowerTo2(height),GL_RGBA,GL_RGBA8,1);
															draw_width	=float(width)/float(image_rgb->Width);
															draw_height	=float(height)/float(image_rgb->Height);}
                        DebugLog(u"create texture ok!");

						image_rgb->Filter=tfLinear;
						image_rgb->Blend.Enable=false;

						draw_left	=0;
						draw_top	=0;

						draw_width	=float(width)/float(image_rgb->Width);
						draw_height	=float(height)/float(image_rgb->Height);

						if(OHS.pbo)
						{
							pbo_rgb=new PBO;

							//第一次同时也用一下普通的更改，以免画面错误
							image_rgb->Change(rgb_buffer,0,0,width,height);

							//第一次提交数据
							pbo_rgb->Put(rgb_buffer,width*height*4);
						}
					}
					else
					{
						if(pbo_rgb)
						{
							image_rgb->PBOChange(pbo_rgb,0,0,width,height);

							pbo_rgb->Put(rgb_buffer,width*height*4);
						}
					}

					if(!pbo_rgb)
					{
                        DebugLog(u"change image_rgb start!");
						image_rgb->Change(rgb_buffer,0,0,width,height);
                        DebugLog(u"change image_rgb end!");
					}

                    DebugLog(u"rgb frame update end!");
				}
				else				//YUV播放
				{
                    DebugLog(u"frame update start,y=%p,u=%p,v=%p",yuv[0].data,yuv[1].data,yuv[2].data);
                    DebugLog(u"buf y=%p,u=%p,v=%p",image_y,image_u,image_v);

					if(yuv)
					{
						if(!image_y)	//只有取得了yuv_buffer *yuv才可能知道需要的缓冲区大小，所以放在这里创建贴图
						{
							if(OHS.texture_non_power_of_two)
							{
								image_y=new Texture2D  (yuv[0].stride,yuv[0].height,GL_LUMINANCE,GL_LUMINANCE8,1);
								image_u=new Texture2D  (yuv[1].stride,yuv[1].height,GL_LUMINANCE,GL_LUMINANCE8,1);
								image_v=new Texture2D  (yuv[2].stride,yuv[2].height,GL_LUMINANCE,GL_LUMINANCE8,1);
							}
							else		//不使用矩形贴图
							{
								image_y=new Texture2D(PowerTo2(yuv[0].stride),PowerTo2(yuv[0].height),GL_LUMINANCE,GL_LUMINANCE8,1);
								image_u=new Texture2D(PowerTo2(yuv[1].stride),PowerTo2(yuv[1].height),GL_LUMINANCE,GL_LUMINANCE8,1);
								image_v=new Texture2D(PowerTo2(yuv[2].stride),PowerTo2(yuv[2].height),GL_LUMINANCE,GL_LUMINANCE8,1);
							}

							draw_left	=float(yuv_x)/float(image_y->Width);
							draw_top	=float(yuv_y)/float(image_y->Height);

							draw_width	=float(width)/float(image_y->Width);
							draw_height	=float(height)/float(image_y->Height);

							image_y->Wrap=twClampToEdge;
							image_y->Filter=tfLinear;
							image_y->Blend.Enable=false;

							image_y->Wrap=twClampToEdge;
							image_u->Filter=tfLinear;
							image_u->Blend.Enable=false;

							image_y->Wrap=twClampToEdge;
							image_v->Filter=tfLinear;
							image_v->Blend.Enable=false;

							if(OHS.pbo)
							{
								pbo_y=new PBO;
								pbo_u=new PBO;
								pbo_v=new PBO;
							
								//第一次同时也用一下普通的更改，以免画面错误
                    			image_y->Change(yuv[0].data,0,0,yuv[0].stride,yuv[0].height,GL_LUMINANCE);
								image_u->Change(yuv[1].data,0,0,yuv[1].stride,yuv[1].height,GL_LUMINANCE);
								image_v->Change(yuv[2].data,0,0,yuv[2].stride,yuv[2].height,GL_LUMINANCE);

                                pbo_y->Put(yuv[0].data,yuv[0].stride*yuv[0].height);
                                pbo_u->Put(yuv[1].data,yuv[1].stride*yuv[1].height);
                                pbo_v->Put(yuv[2].data,yuv[2].stride*yuv[2].height);
							}
						}

                        if(pbo_y)
                        {
                            image_y->PBOChange(pbo_y,0,0,yuv[0].stride,yuv[0].height,GL_LUMINANCE);
                            image_u->PBOChange(pbo_u,0,0,yuv[1].stride,yuv[1].height,GL_LUMINANCE);
                            image_v->PBOChange(pbo_v,0,0,yuv[2].stride,yuv[2].height,GL_LUMINANCE);

                            pbo_y->Put(yuv[0].data,yuv[0].stride*yuv[0].height);
                            pbo_u->Put(yuv[1].data,yuv[1].stride*yuv[1].height);
                            pbo_v->Put(yuv[2].data,yuv[2].stride*yuv[2].height);
                        }
                        else
						{
                    		image_y->Change(yuv[0].data,0,0,yuv[0].stride,yuv[0].height,GL_LUMINANCE);
							image_u->Change(yuv[1].data,0,0,yuv[1].stride,yuv[1].height,GL_LUMINANCE);
							image_v->Change(yuv[2].data,0,0,yuv[2].stride,yuv[2].height,GL_LUMINANCE);
						}//if pbo_rgb
					}//if yuv
				}//if image_rgb

                DebugLog(u"frame update end");

				frame_update=false;
			}
            #ifdef _DEBUG
            else
                PutInfo(u"no frame update");
            #endif//

			if(movie_interface->CheckEnd(movie))
            {
                PutInfo(u"影片解码结束!");

				result=false;
            }
            #ifdef _DEBUG
            else
                PutInfo(u"frame check,movie continue;");
            #endif//

			Unlock();

			return(result);
		}

		void Draw()
		{
			if(image_rgb)
			{
                DebugLog(u"draw image_rgb start!");
				image_rgb->DrawPart(left,top,draw_left,draw_top,draw_width,draw_height,scale,scale);
                DebugLog(u"draw image_rgb end!");
				return;
			}

			if(image_y)
			{
				CloseTexture();

				ChangeTexture(2,image_v);
				ChangeTexture(1,image_u);
				ChangeTexture(0,image_y);

                glsl->Begin();

                glsl->SetUniform1i("tex_y",0);
                glsl->SetUniform1i("tex_u",1);
                glsl->SetUniform1i("tex_v",2);

				glBegin(GL_QUADS);
					glMultiTexCoord2f(GL_TEXTURE0,draw_left,			draw_top);
					glMultiTexCoord2f(GL_TEXTURE1,draw_left,			draw_top);
					glMultiTexCoord2f(GL_TEXTURE2,draw_left,			draw_top);					glVertex2f(left,			top);

					glMultiTexCoord2f(GL_TEXTURE0,draw_left+draw_width,	draw_top);
					glMultiTexCoord2f(GL_TEXTURE1,draw_left+draw_width,	draw_top);
					glMultiTexCoord2f(GL_TEXTURE2,draw_left+draw_width,	draw_top);					glVertex2f(left+movie_width,top);

					glMultiTexCoord2f(GL_TEXTURE0,draw_left+draw_width,	draw_top+draw_height);
					glMultiTexCoord2f(GL_TEXTURE1,draw_left+draw_width,	draw_top+draw_height);
					glMultiTexCoord2f(GL_TEXTURE2,draw_left+draw_width,	draw_top+draw_height);		glVertex2f(left+movie_width,top+movie_height);

					glMultiTexCoord2f(GL_TEXTURE0,draw_left,			draw_top+draw_height);
					glMultiTexCoord2f(GL_TEXTURE1,draw_left,			draw_top+draw_height);
					glMultiTexCoord2f(GL_TEXTURE2,draw_left,			draw_top+draw_height);		glVertex2f(left,			top+movie_height);
				glEnd();

			    glsl->End();

				CloseTexture();
			}//image
		}//Draw()
	};
}

namespace hgl
{
	void TheoraMovie::InitPrivate(bool key,bool mouse)
	{
		end_fos=fosReturn;

		SetEventCall(OnKeyDown,			this,TheoraMovie,KeyProc);
		SetEventCall(OnMouseLeftDown,	this,TheoraMovie,MouseProc);
		SetEventCall(OnMouseRightDown,	this,TheoraMovie,MouseProc);

		key_break=key;
		mouse_break=mouse;

		file=nullptr;
		audio=nullptr;
		movie=nullptr;
	}

	TheoraMovie::TheoraMovie()
	{
		InitPrivate(true,true);
	}

	bool TheoraMovie::OpenMovie()
	{
		if(movie)
		{
			delete movie;
			movie=nullptr;
		}

		if(file)
		{
			movie=new MovieDecode;

			if(movie->Open(file))
			{
				start=false;

				return(true);
			}
		}

		SAFE_CLEAR(movie);
		
        PutError(u"打开影片失败");
		return(false);
	}

	/**
	* 打开一个影片文件
	* @param filename 影片文件名称(注:不带扩展名)
	* @param key 是否键盘可以中断播放
	* @param mouse 是否鼠标可以中断播放
	* @return 打开影片是否成功
	*/
	bool TheoraMovie::Open(const UTF16String &filename,bool key,bool mouse)
	{
		InitPrivate(key,mouse);

		if(FileConfirm(filename+u".ogv"))
			file=OpenFileStream(filename+u".ogv");
		else
			file=nullptr;

        if(FileConfirm(filename+u".ogg"))
			audio=new AudioPlayer(filename+u".ogg");
		else
			audio=nullptr;

		return OpenMovie();
	}

	/**
	* 打开一个影片文件
	* @param hac HAC包指针
	* @param filename 影片文件名称(注:不带扩展名)
	* @param key 是否键盘可以中断播放
	* @param mouse 是否鼠标可以中断播放
	* @return 打开影片是否成功
	*/
	bool TheoraMovie::Open(HAC *hac,const UTF16String &filename,bool key,bool mouse)
	{
		InitPrivate(key,mouse);

		file=hac->LoadFile(filename+u".ogv");

		if(hac->FindFile(filename+u".ogg"))
			audio=new AudioPlayer(hac,filename+u".ogg");
		else
			audio=nullptr;

		return OpenMovie();
	}

	/**
	* 本类构造函数
	* @param ogv_filename 视频ogv文件名称(注:带扩展名.ogv)
	* @oaram ogg_filename 音频ogg文件名称(注:带扩展名.ogg)
	* @param key 是否键盘可以中断播放
	* @param mouse 是否鼠标可以中断播放
	* @return 打开影片是否成功
	*/
	bool TheoraMovie::Open(const UTF16String &ogv_filename,const UTF16String &ogg_filename,bool key,bool mouse)
	{
		InitPrivate(key,mouse);

		file=OpenFileStream(ogv_filename);

		audio=new AudioPlayer(ogg_filename);

		return OpenMovie();
	}

	/**
	* 本类构造函数
	* @param hac HAC包指针
	* @param ogv_filename 视频ogv文件名称(注:带扩展名.ogv)
	* @oaram ogg_filename 音频ogg文件名称(注:带扩展名.ogg)
	* @param key 是否键盘可以中断播放
	* @param mouse 是否鼠标可以中断播放
	* @return 打开影片是否成功
	*/
	bool TheoraMovie::Open(HAC *hac,const UTF16String &ogv_filename,const UTF16String &ogg_filename,bool key,bool mouse)
	{
		InitPrivate(key,mouse);

		file=hac->LoadFile(ogv_filename);

		audio=new AudioPlayer(hac,ogg_filename);

		return OpenMovie();
	}

	TheoraMovie::~TheoraMovie()
	{
		if(movie)
		{
			movie->StopMovie();
			delete movie;
		}

		SAFE_CLEAR(audio);
		SAFE_CLEAR(file);
	}

	void TheoraMovie::SetNextObject(FlowObject *fo)
	{
		NextObject=fo;
		end_fos=fosExit;
	}

	void TheoraMovie::Update()
	{
		if(!movie)
		{
			fos=end_fos;
			return;
		}

		if(!start)
		{
			glClearColor(0,0,0,1);
			ClearScreen();

			if(audio)
				audio->Play(false);

			movie->StartMovie();

			start=true;
		}
	}

	void TheoraMovie::Draw()
	{
		ClearScreen();
		CloseBlend();

        if(!start)return;               //影片没有开始

        if(movie->Update())
        {
            movie->Draw();
            return;
        }
        else
        {
            fos=end_fos;
        }
	}

	bool TheoraMovie::KeyProc(uint)
	{
		if(key_break)
		{
			fos=end_fos;
			return(true);
		}
		else
        	return(false);
	}

	bool TheoraMovie::MouseProc(int,int,uint)
	{
		if(mouse_break)
		{
			fos=end_fos;
			return(true);
		}
		else
			return(false);
	}
}//namespace hgl
