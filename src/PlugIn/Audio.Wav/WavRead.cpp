#define __MAKE_PLUGIN__

#include<hgl/PlugIn.h>
#include<string.h>
#include<malloc.h>
#include<hgl/al/al.h>

using namespace hgl;
using namespace openal;

typedef struct                                  /* WAV File-header */
{
	ALubyte  Id[4]			;
	ALsizei  Size				;
	ALubyte  Type[4]			;
}
WAVFileHdr_Struct;

typedef struct                                  /* WAV Fmt-header */
{
	ALushort Format			;
	ALushort Channels			;
	ALuint   SamplesPerSec	;
	ALuint   BytesPerSec		;
	ALushort BlockAlign		;
	ALushort BitsPerSample	;
}
WAVFmtHdr_Struct;

typedef struct									/* WAV FmtEx-header */
{
	ALushort Size				;
	ALushort SamplesPerBlock	;
}
WAVFmtExHdr_Struct;

typedef struct                                  /* WAV Smpl-header */
{
	ALuint   Manufacturer		;
	ALuint   Product			;
	ALuint   SamplePeriod		;
	ALuint   Note				;
	ALuint   FineTune			;
	ALuint   SMPTEFormat		;
	ALuint   SMPTEOffest		;
	ALuint   Loops			;
	ALuint   SamplerData		;

	struct
	{
		ALuint Identifier		;
		ALuint Type				;
		ALuint Start			;
		ALuint End				;
		ALuint Fraction			;
		ALuint Count			;
	}
	Loop[1]			;
}
WAVSmplHdr_Struct;

typedef struct                                  /* WAV Chunk-header */
{
	ALubyte  Id[4]			;
	ALuint   Size				;
}
WAVChunkHdr_Struct;

ALvoid alutLoadWAVMemory(ALbyte *memory, ALsizei memory_size,ALenum *format, ALvoid **data, ALsizei *size, ALsizei *freq, ALboolean *loop)
{
	WAVChunkHdr_Struct ChunkHdr;
	WAVFmtExHdr_Struct FmtExHdr;
	WAVFileHdr_Struct FileHdr;
	WAVSmplHdr_Struct SmplHdr;
	WAVFmtHdr_Struct FmtHdr;
	ALbyte *Stream;

	*format=0;
	*data=NULL;
	*size=0;
	*freq=0;
	*loop=AL_FALSE;

	if (memory)
	{
		Stream=memory;

		if (Stream)
		{
			memcpy(&FileHdr, Stream, sizeof(WAVFileHdr_Struct));
			Stream+=sizeof(WAVFileHdr_Struct);
			FileHdr.Size=((FileHdr.Size+1)&~1)-4;
			while ((FileHdr.Size!=0)&&(memcpy(&ChunkHdr, Stream, sizeof(WAVChunkHdr_Struct))))
			{
				Stream+=sizeof(WAVChunkHdr_Struct);
				if (!memcmp(ChunkHdr.Id, "fmt ", 4))
				{
					memcpy(&FmtHdr, Stream, sizeof(WAVFmtHdr_Struct));
					if (FmtHdr.Format==0x0001)
					{
						*format=(FmtHdr.Channels==1? (FmtHdr.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16):
						     (FmtHdr.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
						*freq=FmtHdr.SamplesPerSec;
						Stream+=ChunkHdr.Size;
					}
					else
					{
						memcpy(&FmtExHdr, Stream, sizeof(WAVFmtExHdr_Struct));
						Stream+=ChunkHdr.Size;
					}
				}
				else if (!memcmp(ChunkHdr.Id, "data", 4))
				{
					if (FmtHdr.Format==0x0001)
					{
						*size=ChunkHdr.Size;
						*data=malloc(ChunkHdr.Size+31);
						if (*data) memcpy(*data, Stream, ChunkHdr.Size);
						memset(((char *)*data)+ChunkHdr.Size, 0, 31);
						Stream+=ChunkHdr.Size;
					}
					else if (FmtHdr.Format==0x0011)
					{
						//IMA ADPCM
					}
					else if (FmtHdr.Format==0x0055)
					{
						//MP3 WAVE
					}
				}
				else if (!memcmp(ChunkHdr.Id, "smpl", 4))
				{
					memcpy(&SmplHdr, Stream, sizeof(WAVSmplHdr_Struct));
					*loop = (SmplHdr.Loops ? AL_TRUE : AL_FALSE);
					Stream+=ChunkHdr.Size;
				}
				else
					Stream+=ChunkHdr.Size;
				Stream+=ChunkHdr.Size&1;
				FileHdr.Size-=(((ChunkHdr.Size+1)&~1)+8);
			}
		}
	}
}

ALvoid alutUnloadWAV(ALenum, ALvoid *data, ALsizei, ALsizei)
{
	if (data)
		free(data);
}
//--------------------------------------------------------------------------------------------------
struct OutInterface
{
	void (*Load)(ALbyte *,ALsizei,ALenum *,ALvoid **,ALsizei *,ALsizei *,ALboolean *);
	void (*Clear)(ALenum,ALvoid *,ALsizei,ALsizei);

	void *(*Open)(ALbyte *,ALsizei,ALenum *,ALsizei *,double *);
	void  (*Close)(void *);
	uint  (*Read)(void *,char *,uint);
	void  (*Restart)(void *);
};

static OutInterface out_interface=
{
	alutLoadWAVMemory,
	alutUnloadWAV,

	NULL,
	NULL,
	NULL,
	NULL
};
//--------------------------------------------------------------------------------------------------
const u16char plugin_intro[]=u"WAV音频文件解码(2014-04-09,代码源自ALUT)";

HGL_PLUGIN_FUNC uint32 GetPlugInVersion()
{
	return(2);		//版本号
					//根据版本号取得不同的API
}

HGL_PLUGIN_FUNC u16char * GetPlugInIntro()
{
	return((u16char *)plugin_intro);
}

HGL_PLUGIN_FUNC bool GetPlugInInterface(uint32 ver,void *data)
{
	if(ver==2)
	{
		memcpy(data,&out_interface,sizeof(OutInterface));
	}
	else
		return(false);

	return(true);
}

HGL_PLUGIN_FUNC void SetPlugInInterface(uint32,void *)
{
}
//--------------------------------------------------------------------------------------------------
