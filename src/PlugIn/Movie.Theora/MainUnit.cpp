#include<hgl/PlugIn.h>
#include<string.h>
#include"TheoraPlayer.h"

using namespace hgl;

const wchar_t plugin_intro[]=L"Theora影片解码器(LibTheora 1.1.1,LibOGG 1.3.2,2015-06-18)";

typedef TheoraPlayer MOVIE;
//--------------------------------------------------------------------------------------------------
MOVIE * __cdecl OpenMovie(Stream *file,SeekFunc sf,EofFunc ef,ReadFunc rf)
{
	TheoraPlayer *tp=new TheoraPlayer;

	if(tp->Open(file,sf,ef,rf))
		return tp;

	delete tp;
	return(NULL);
}

bool __cdecl GetMovieInfo(MOVIE *tp,int &width,int &height,int &fps)
{
	if(tp)
	{
		width=tp->videoWidth;
		height=tp->videoHeight;
		fps=tp->videoFpS;

		return(true);
	}
	else
		return(false);
}

void __cdecl SetBufferSize(MOVIE *tp,uint size)
{
	if(tp)
		tp->BufferSize=size;
}

double __cdecl CheckUpdate(MOVIE *tp)
{
	if(tp)
		return tp->CheckUpdate();
	else
		return(-1);
}

void __cdecl DecodeToRGB	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToRGB(buf);}
void __cdecl DecodeToBGR	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToBGR(buf);}
void __cdecl DecodeToRGBA	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToRGBA(buf);}
void __cdecl DecodeToBGRA	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToBGRA(buf);}

th_img_plane *__cdecl DecodeToYUV(MOVIE *tp,int &ox,int &oy)
{
	if(tp)
		return tp->DecodeToYUV(ox,oy);
	else
		return NULL;
}

bool __cdecl CheckMovieEnd(MOVIE *tp)
{
	if(!tp)
		return(true);

	return !(tp->isPlaying);
}

void __cdecl CloseMovie(MOVIE *tp)
{
	if(tp)
		delete tp;
}
//--------------------------------------------------------------------------------------------------
struct OutInterface
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

static OutInterface out_interface=
{
	OpenMovie,
	GetMovieInfo,
	SetBufferSize,
	CheckUpdate,
	DecodeToRGB,
	DecodeToBGR,
	DecodeToRGBA,
	DecodeToBGRA,
	DecodeToYUV,
	CheckMovieEnd,
	CloseMovie
};
//--------------------------------------------------------------------------------------------------
HGL_PLUGIN_FUNC void __cdecl InitPlugIn()
{
	InitTheoraDecode();
}

HGL_PLUGIN_FUNC uint32 __cdecl GetPlugInVersion()
{
	return(2);		//版本号
					//根据版本号取得不同的API
}

HGL_PLUGIN_FUNC wchar_t * __cdecl GetPlugInIntro()
{
	return((wchar_t *)plugin_intro);
}

HGL_PLUGIN_FUNC bool __cdecl GetPlugInInterface(uint32 ver,void *data)
{
	if(ver==2)
	{
		memcpy(data,&out_interface,sizeof(OutInterface));
		return(true);
	}
	else
	{
		return(false);
	}
}