#include<hgl/graph/TileFont.h>
#include<hgl/graph/Render.h>
#include<hgl/graph/Material.h>
#include<hgl/graph/Renderable.h>
#include<stdarg.h>

namespace hgl
{
	namespace graph
	{
		TileFont::TileFont(int fh,TileData *td,FontSource *cfs,FontSource *efs):fud(td->GetMaxCount())
		{
			font_height=fh;
			line_distance=font_height*0.15;

			tile_data=td;

			chs_source=cfs;
			eng_source=efs;

			if(!efs)
				eng_source=cfs;

			if(!cfs)
				chs_source=efs;

			const int max_count=td->GetMaxCount();

			vertex2d=new VB2f(6*max_count,0,HGL_DYNAMIC_DRAW);
			tex_coord=new VB2f(6*max_count,0,HGL_DYNAMIC_DRAW);

			fnt_mtl=CreateMaterial();

			fnt_mtl->SetColorMaterial(true);
			fnt_mtl->SetColor(1,1,1,1);

			fnt_draw=CreateRenderable();
			fnt_draw->SetPrimitive(HGL_PRIM_TRIANGLES);
			fnt_draw->SetMaterial(fnt_mtl,true);

			fnt_mtl->SetTexture(mtcAlpha,tile_data->GetTexture());

			fnt_mtl->SetBlend(true);
			fnt_mtl->SetBlend(HGL_BLEND_SRC_ALPHA,HGL_BLEND_ONE_MINUS_SRC_ALPHA);

			fnt_mtl->SetDepthMask(false);
			fnt_mtl->SetDepthTest(false);

			fnt_mtl->SetLight(false);

			fnt_draw->SetVertex(vertex2d);
			fnt_draw->SetTexCoord(mtcAlpha,vbtAlphaTexCoord,tex_coord);

			fnt_draw->AutoCreateShader(true
#ifdef _DEBUG
				,L"TileFont"
#endif//_DEBUG
				);

			tile_width=td->GetWidth();
			tile_height=td->GetHeight();

			char_bitmap_bytes=tile_width*tile_height;
			char_bitmap_buffer=new uint8[char_bitmap_bytes];
		}

		TileFont::~TileFont()
		{
			//FontSource不用释放

			delete[] char_bitmap_buffer;
			delete fnt_draw;

			delete tile_data;
		}

		FontSource::Bitmap *TileFont::MakeCharBitmap(const wchar_t &ch)
		{
			FontSource::Bitmap *bmp=GetCharBitmap(ch);

			if(!bmp)
				return(false);

			memset(char_bitmap_buffer,0,char_bitmap_bytes);

			int copy_rows=(bmp->h>tile_height?tile_height:bmp->h);
			int copy_cols=(bmp->w>tile_width?tile_width:bmp->w);

			uint8 *sp=bmp->data;
			uint8 *tp=char_bitmap_buffer;

			for(int row=0;row<copy_rows;row++)
			{
				memcpy(tp,sp,copy_cols);

				tp+=tile_width;
				sp+=bmp->w;
			}

			return(bmp);
		}

		TileData::Object *TileFont::GetCharData(const wchar_t &ch)
		{
			FudItem<wchar_t,TileData::Object *> *obj;

			TileData::Object *tile;

			if(fud.Find(ch,tile,true))		//查找并自动移到最前
				return(tile);

			FontSource::Bitmap *bmp=MakeCharBitmap(ch);

			if(!bmp)
				return(NULL);

			if(fud.GetFreeCount())			//有剩余空间
			{
				tile=tile_data->Add(char_bitmap_buffer,char_bitmap_bytes,HGL_SF_R8,bmp->w,bmp->h);

				obj=fud.Add(ch,tile);
			}
			else
			{
				obj=fud.GetEnd();

				obj->flag=ch;

				tile_data->Change(obj->data,char_bitmap_buffer,char_bitmap_bytes,HGL_SF_R8,bmp->w,bmp->h);
			}

			return(obj->data);
		}

		/**
		* 求字符宽度
		* @param ch 字符
		* @return 字符的象素级宽度
		*/
		float TileFont::CharWidth(wchar_t ch)
		{
			if(ch==0)return(0);

			const FontSource::Bitmap *bmp=GetCharBitmap(ch);

			return bmp?bmp->adv_x:0;
		}

		/**
		* 求字符串宽度
		* @param str 字符串
		* @param limit 限制的字符数
		* @return 字符串的象素级宽度
		*/
		float TileFont::GetStringWidth(const wchar_t *str,int limit)
		{
			if(!str||!(*str)||limit==0)return(0);

			const wchar_t *p=str;
			int w=0;

			while(*p&&limit--)
			{
				const FontSource::Bitmap *bmp=GetCharBitmap(*p++);

				if(bmp)
					w+=bmp->adv_x;
				else
				{
					if(*p==L'\t')w+=font_height*2;else
					if(*p>0xFF	)w+=font_height;else
								 w+=font_height/2;
				}
			}

			return(w);
		}

		/**
		* 排版字符串(无自动换行,无\n处理)
		* @param makeup 排版数据
		* @param x 起始X坐标
		* @param y 起始Y坐标
		* @param str 字符串
		* @param limit 处理的字符数量
		*/
		bool TileFont::MakeupText(Makeup &makeup,int x,int y,const wchar_t *str,int limit)
		{
			if(!str||!(*str)||limit==0)return(false);

			double w;
			Makeup::CharMakeup cm;

			while(*str&&limit--)
			{
				const FontSource::Bitmap *bmp=GetCharBitmap(*str);

				if(bmp)
				{
					w=bmp->adv_x;

					cm.left	=bmp->x;
					cm.top	=bmp->y;
				}
				else
				{
					if(*str==L'\t')w=font_height*2;else
					if(*str==L'\r')w=0;else
					if(*str>0xFF  )w=font_height;else
								   w=font_height/2;

					cm.left	=0;
					cm.top	=0;
				}

				cm.scope.Set(x,y,w,font_height);
				cm.ch=*str;

				x+=w;

				makeup+=cm;

				str++;
			}

			return(true);
		}

		/**
		* 排版字符串(无自动换行,仅认\n为换行)
		* @param makeup 记录排版信息的数据结构
		* @param str 要排版的字符串
		* @param limit 字符数量,-1表示绘到结束为止
		*/
		bool TileFont::MakeupText(Makeup &makeup,const wchar_t *str,int limit)
		{
			if(!str||!(*str)||limit==0)return(false);

			makeup.Clear();

			const wchar_t *p=str;
			int x=0,y=0,w;
			int max_w=0,max_h=0;
			Makeup::CharMakeup cm;

			while(*p&&limit)
			{
				if(*p==L'\n')
				{
					cm.scope.Set(x,y,0,0);

					if(x>max_w)
						max_w=x;

					x=0;
					y+=font_height+line_distance;

					if(y>max_h)
                		max_h=y;
				}
				else
				if(*p==L'\t')
				{
					cm.scope.Set(x,y,font_height*2,font_height);

					x+=font_height*2;	//4字个宽
				}
				else
				{
					const FontSource::Bitmap *bmp=GetCharBitmap(*p);

					if(bmp)
					{
						w=bmp->adv_x;

						cm.scope.Set(x,y,w,font_height);
						cm.left	=bmp->x;
						cm.top	=bmp->y;
					}
					else
					{
						if(*p<0xFF)
							w=font_height/2;
						else
							w=font_height;

						cm.scope.Set(x,y,w,0);
					}

					x+=w;
				}

				cm.ch=*p;
				makeup+=cm;

				p++;
				limit--;
			}

			if(x>max_w)
        		max_w=x;

			if(max_h)
			{
				if(x==0)					//最后一行是空的
					max_h-=line_distance;
				else						//不是空的
            		max_h+=font_height;
			}
			else	//只有一行
			{
				max_h=font_height;
			}

			makeup.Width =max_w;
			makeup.Height=max_h;

			makeup.ProcEnd(font_height/2);

			return(true);
		}

		const wchar_t 	BeginSymbol[]=	L"!),.:;?]}¨·ˇˉ―‖’”…∶、。〃々〉》」』】〕〗〙〛！＂＇），．：；？］｀｜｝～￠";
		const wchar_t 	EndSymbol[]=	L"([{·‘“〈《「『【〔〖〘〚（．［｛￡￥";

		const int 		BeginSymbolCount=sizeof(BeginSymbol)/sizeof(wchar_t);
		const int 		EndSymbolCount	=sizeof(EndSymbol)/sizeof(wchar_t);

		/**
		* 排版字符串(会自动换行,自动处理行首行尾标点；\n处理为强制换行，不处理标点)
		* @param makeup 记录排版信息的数据结构
		* @param str 要排版的字符串
		* @param max_width 最大宽度
		* @param align 对齐方式
		*/
		bool TileFont::MakeupText(Makeup &makeup,const wchar_t *str,int max_width,TextAlignment align)
		{
			if(!str||!(*str)||max_width<=font_height||align<=taNone||align>=taEnd)return(false);

			makeup.Clear();
			makeup.Width=max_width;

			const wchar_t *p,*sp;
			int row=0,w=0,tw=0,pos;
			Makeup::CharMakeup cm;

			p=sp=str;
			while(*p)
			{
				if(*p==L'\n')		//强制换行
				{
					if(align==taCenter	)pos=(max_width-tw)/2;else
					if(align==taRight	)pos=max_width-tw;else pos=0;

					MakeupText(makeup,pos,row,sp,p-sp);

					cm.scope.Width=0;
					cm.scope.Height=0;
					cm.ch=*p;

					makeup+=cm;	//空的也要加

					tw=0;
					row+=font_height+line_distance;

					p++;
					sp=p;
				}
				else
				{
					const FontSource::Bitmap *bmp=GetCharBitmap(*p);

					if(bmp)
					{
						w=bmp->adv_x;
					}
					else
					{
						if(*p==L'\t')w=font_height*2;else
						if(*p==L'\r')w=0;else
						if(*p>0xFF  )w=font_height;else
									 w=font_height/2;
					}

					if(tw+w>max_width)		//换行
					{
						while(p>sp)
						{
							bool result=false;

							if(strchr(EndSymbol,	*(p-1),	EndSymbolCount	))result=true;		//查找当前字符是否出现在行尾禁用符号
							if(strchr(BeginSymbol,	*p,		BeginSymbolCount))result=true;		//查找当前字符是否出现在行首禁用符号

							if(result)								//无论行首行尾出现，都是换行前移一个字符
							{
								const FontSource::Bitmap *prev_bmp=GetCharBitmap(*(p-1));

								tw-=prev_bmp->adv_x;				//删除字符宽度
								p--;								//p前移

								continue;
							}

							break;									//都没有查到，退出查找
						}

						if(align==taCenter	)pos=(max_width-tw)/2;else
						if(align==taRight	)pos=max_width-tw;else pos=0;

						MakeupText(makeup,pos,row,sp,p-sp);

						tw=0;
						row+=font_height+line_distance;

						sp=p;
					}
					else
					{
						tw+=w;

						p++;
					}
				}
			}

			if(align==taCenter	)pos=(max_width-tw)/2;else
			if(align==taRight	)pos=max_width-tw;else pos=0;

			MakeupText(makeup,pos,row,sp,p-sp);

			makeup.ProcEnd(font_height/2);

			return(true);
		}

		/**
		* 绘制一个排版好的文本
		* @param mat modelview变换矩阵
		* @param makeup 排版信息
		* @param limit 字数
		*/
		void TileFont::Draw(const Matrix4f *mat,const Makeup &makeup,int limit)
		{
			if(limit==0)return;

			const Makeup::CharMakeup *cm=makeup.MakeupString.Data;

			int n=makeup.MakeupString.Count;
			int i=0;
			int count=0;

			if(limit>0&&n>limit)
				n=limit;

			vertex2d->Begin();
			tex_coord->Begin();

			while(n--)
			{
				TileData::Object *to=GetCharData(cm->ch);

				if(!to||cm->scope.Width<=0||cm->scope.Height<=0)			//都不为0
				{
					cm++;
					continue;
				}

				RenderToVB2DRect(	vertex2d,
									tex_coord,
									to,
									cm->scope.Left+cm->left,
									cm->scope.Top +cm->top);
				count++;

				cm++;
			}

			tex_coord->End();
			vertex2d->End();

//#ifdef _DEBUG
//			Texture2D *tex=tile_data->GetTexture();
//
//			uint8 *buf;
//			int size;
//
//			size=tex->GetImage(HGL_SF_R8,NULL);
//
//			buf=new uint8[size];
//
//			tex->GetImage(HGL_SF_R8,buf);
//
//			SaveMemoryToFile(L"TileFont.RAW",buf,size);
//
//			delete[] buf;
//#endif//_DEBUG

			fnt_mtl->SetColor(Color);
			fnt_draw->SetDrawCount(0,count*6);
			DirectRender2D(fnt_draw,mat);
		}

		/**
		* 以格式化方式绘制字符串
		* @param l,t 绘制字符串的坐标
		* @param fmt 格式化字串
		* @return 字符串的象素级宽度
		*/
		float TileFont::DrawFormat(const Matrix4f *mat,const wchar_t *fmt,...)
		{
			va_list va;
			wchar_t str[1024];

			va_start(va,fmt);
			vswprintf_s(str,1023,fmt,va);
			va_end(va);

			return DrawString(mat,str);
		}

		/**
		* 以格式化方式绘制字符串
		* @param l,t 绘制字符串的坐标
		* @param fmt 格式化字串
		* @return 字符串的象素级宽度
		*/
		float TileFont::DrawFormat(float l,float t,const wchar_t *fmt,...)
		{
			va_list va;
			wchar_t str[1024];

			va_start(va,fmt);
			vswprintf_s(str,1023,fmt,va);
			va_end(va);

			const Matrix4f pos=translate(l,t,0);

			return DrawString(&pos,str);
		}
	}//namespace graph
}//namespace hgl
