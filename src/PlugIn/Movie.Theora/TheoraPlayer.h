#ifndef HGL_THEORA_PLAYER_INCLUDE
#define HGL_THEORA_PLAYER_INCLUDE

#include<theora/theoradec.h>

typedef void Stream;
typedef void (__cdecl *SeekFunc	)(Stream *,int);
typedef bool (__cdecl *EofFunc	)(Stream *);
typedef int	 (__cdecl *ReadFunc	)(Stream *,void *,int);
//--------------------------------------------------------------------------------------------------
class TheoraPlayer
{
	double GetDoubleTime();																		///<取得当前时间(双精度，单位秒)

private:

    ogg_sync_state   oy;
    ogg_page         og;
    ogg_stream_state vo;
    ogg_stream_state to;

	th_info			ti;
	th_comment		tc;
	th_setup_info	*tsi;
	th_dec_ctx		*tdc;

private:

	Stream *			in;

	SeekFunc			Seek;
	EofFunc				Eof;
	ReadFunc			Read;

private:

    int					theora_p;
    int					stateflag;

    ogg_int64_t			videobuf_granulepos;
    double				videobuf_time;
	double				one_frame_time;
	double				half_frame_time;
    int					frameNr;

    ogg_packet			op;

	th_ycbcr_buffer		yuv;

    int					fileVideoDataOffset;
    bool				theoraLoaded;
    double				startTime;
    unsigned int		decodedFrames;

	int					pp_level_max;
	int					pp_level;
	int					pp_inc;

	double				currentTime;

protected:

    int buffer_data(ogg_sync_state *);
    int queue_page(ogg_page *);

    void DecodeToRGB(th_ycbcr_buffer &,unsigned char *);
	void DecodeToBGR(th_ycbcr_buffer &,unsigned char *);

	void DecodeToRGBA(th_ycbcr_buffer &,unsigned char *);
	void DecodeToBGRA(th_ycbcr_buffer &,unsigned char *);

    void StopTheora();

public:
                          
	bool        	isOpen;
	bool			isLoop;
	bool           	isPlaying;
	unsigned int    videoWidth;
	unsigned int    videoHeight;
	float       	videoFpS;
	unsigned int	BufferSize;

public:

	TheoraPlayer();
	~TheoraPlayer();

	bool Open(Stream *,SeekFunc,EofFunc,ReadFunc);
    void Close();

	double			CheckUpdate();
    void			DecodeToRGB(unsigned char *);
	void			DecodeToBGR(unsigned char *);
	void			DecodeToRGBA(unsigned char *);
	void			DecodeToBGRA(unsigned char *);
	th_img_plane *	DecodeToYUV(int &,int &);
    void			Rewind();
};
//--------------------------------------------------------------------------------------------------
void InitTheoraDecode();
//--------------------------------------------------------------------------------------------------
#endif//HGL_THEORA_PLAYER_INCLUDE