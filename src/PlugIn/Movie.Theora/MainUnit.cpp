#include<hgl/PlugIn.h>
#include<string.h>
#include"TheoraPlayer.h"

using namespace hgl;

const wchar_t plugin_intro[]=L"Theora影片解码器(LibTheora 1.1.1,LibOGG 1.3.2,2015-06-18)";

typedef TheoraPlayer MOVIE;
//--------------------------------------------------------------------------------------------------
MOVIE * OpenMovie(Stream *file,SeekFunc sf,EofFunc ef,ReadFunc rf)
{
	TheoraPlayer *tp=new TheoraPlayer;

	if(tp->Open(file,sf,ef,rf))
		return tp;

	delete tp;
	return(NULL);
}

bool GetMovieInfo(MOVIE *tp,int &width,int &height,int &fps)
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

void SetBufferSize(MOVIE *tp,uint size)
{
	if(tp)
		tp->BufferSize=size;
}

double CheckUpdate(MOVIE *tp)
{
	if(tp)
		return tp->CheckUpdate();
	else
		return(-1);
}

void DecodeToRGB	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToRGB(buf);}
void DecodeToBGR	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToBGR(buf);}
void DecodeToRGBA	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToRGBA(buf);}
void DecodeToBGRA	(MOVIE *tp,uchar *buf){	if(tp)		tp->DecodeToBGRA(buf);}

th_img_plane *DecodeToYUV(MOVIE *tp,int &ox,int &oy)
{
	if(tp)
		return tp->DecodeToYUV(ox,oy);
	else
		return NULL;
}

bool CheckMovieEnd(MOVIE *tp)
{
	if(!tp)
		return(true);

	return !(tp->isPlaying);
}

void CloseMovie(MOVIE *tp)
{
	if(tp)
		delete tp;
}
//--------------------------------------------------------------------------------------------------
struct OutInterface
{
	MOVIE *			(*Open			)(Stream *,SeekFunc,EofFunc,ReadFunc);
	bool			(*GetInfo		)(MOVIE *,int &,int &,int &);
	void			(*SetBuffer		)(MOVIE *,uint);
	double			(*CheckUpdate	)(MOVIE *);
	void			(*DecodeToRGB	)(MOVIE *,uchar *);
	void			(*DecodeToBGR	)(MOVIE *,uchar *);
	void			(*DecodeToRGBA	)(MOVIE *,uchar *);
	void			(*DecodeToBGRA	)(MOVIE *,uchar *);
	th_img_plane *	(*DecodeToYUV	)(MOVIE *,int &,int &);
	bool			(*CheckEnd		)(MOVIE *);
	void			(*Close			)(MOVIE *);
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
HGL_PLUGIN_FUNC void InitPlugIn()
{
	InitTheoraDecode();
}

HGL_PLUGIN_FUNC uint32 GetPlugInVersion()
{
	return(2);		//版本号
					//根据版本号取得不同的API
}

HGL_PLUGIN_FUNC wchar_t * GetPlugInIntro()
{
	return((wchar_t *)plugin_intro);
}

HGL_PLUGIN_FUNC bool GetPlugInInterface(uint32 ver,void *data)
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
