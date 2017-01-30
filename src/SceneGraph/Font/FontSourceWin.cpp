#include"FontSourceWin.H"

namespace hgl
{
	namespace graph
	{
		namespace
		{
			void Convert8BitGrey(unsigned char *dst,unsigned char *src,int w,int h,int line_bytes)
			{
				int pos;
				unsigned char *sp=src,*p;

				while(h--)
				{
					pos=w;
					p=sp;

					while(pos--)
					{
						if(*p==64)
							*dst=255;
						else
							*dst=(*p)<<2;

						dst++;
						p++;
					}

					sp+=line_bytes;
				}
			}

			void ConvertBitmap(unsigned char *dst,unsigned char *src,int w,int h,int line_bytes)
			{
				unsigned char *sp=src,*p;
				unsigned char bit;

				while(h--)
				{
					p=sp;

					bit=1<<7;

					for(int i=0;i<w;i++)
					{
						*dst++=(*p&bit)?255:0;

						if(bit==0x01)
						{
							bit=1<<7;
							p++;
						}
						else bit>>=1;
					}

					sp+=line_bytes;
				}//while(h--)
			}
		}//namespace

		FontSource *CreateWinBitmapFont(const FontInfo &f)
		{
			return(new WinBitmapFont(f));
		}

		WinBitmapFont::WinBitmapFont(const FontInfo &f):FontSource(f)
		{
			hdc=CreateCompatibleDC(0);

			LineHeight=(fnt.height*GetDeviceCaps(hdc,LOGPIXELSY))/72;
			LineDistance=(LineHeight-fnt.height)/2;

			hfont=CreateFont(	-fnt.height,
								fnt.width,
								0,
								0,
								fnt.bold?FW_BOLD:FW_NORMAL,
								fnt.italic,
								false,
								0,
								DEFAULT_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								ANTIALIASED_QUALITY,
								FF_DONTCARE,
								fnt.name);

			SelectObject(hdc,hfont);

			if(!fnt.anti||fnt.height<=10)		//<=10象素强制用无抗矩齿字体
				ggo=GGO_BITMAP;
			else
				ggo=GGO_GRAY8_BITMAP;

			buffer.SetLength(fnt.width*fnt.height*4);
		}

		WinBitmapFont::~WinBitmapFont()
		{
			//delete[] buffer;

    		DeleteObject(hfont);
    		DeleteDC(hdc);
		}

		bool WinBitmapFont::MakeCharBitmap(wchar_t ch)
		{
			memset(&gm,0,sizeof(GLYPHMETRICS));
			memset(&mat,0,sizeof(MAT2));

			mat.eM11.value = 1;
			mat.eM22.value = 1;

			const int size=GetGlyphOutline(hdc,ch,ggo,&gm,0,0,&mat);

			if(size<=0)return(false);

			buffer.SetLength(size);

			GetGlyphOutline(hdc,ch,ggo,&gm,size,buffer,&mat);

			FontSource::Bitmap *p=char_bitmap+ch;

			p->w=gm.gmBlackBoxX;
			p->h=gm.gmBlackBoxY;

			p->x=gm.gmptGlyphOrigin.x;
			p->y=fnt.height-gm.gmptGlyphOrigin.y-LineDistance;

			p->adv_x=gm.gmCellIncX;
			p->adv_y=gm.gmCellIncY;

			p->data=new unsigned char[p->w*p->h];

			if(ggo==GGO_GRAY8_BITMAP)
				Convert8BitGrey(p->data,buffer.GetData(),gm.gmBlackBoxX,gm.gmBlackBoxY,size/p->h);
			else
				ConvertBitmap(p->data,buffer.GetData(),gm.gmBlackBoxX,gm.gmBlackBoxY,size/p->h);

			return(true);
		}
	}//namespace graph
}//namespace hgl
