#ifndef HGL_IO_TEXT_OUTPUT_STREAM_INCLUDE
#define HGL_IO_TEXT_OUTPUT_STREAM_INCLUDE

#include<hgl/endian/Endian.h>
#include<hgl/type/StringList.h>
namespace hgl
{
	namespace io
	{
		/**
		* 文本输出流<br>
		* 用于将数据输出成文件，用于日志、配置等文本信息处理。
		*/
		template<ByteOrderMask BOM> class TextOutputStream								///文本输出流
		{
		protected:

			ByteOrderMask bom;																		///<文本编码
			DataOutputStream *out;																	///<数据输出流

		public:

			TextOutputStream(OutputStream *);

			virtual ~TextOutputStream()
			{
				delete out;
			}

			const ByteOrderMask GetBOM()const
			{
				return BOM;
			}

			DataOutputStream *GetDataOutputStream()
			{
				return out;
			}

			operator DataOutputStream *()
			{
				return out;
			}

			bool WriteBOM()																			///<写入文本文件bom头
			{
				if(!out)return(false);

				const BOMFileHeader *bom_item=BOMData+BOM;

				return(out->Write(bom_item->data,bom_item->size)==bom_item->size);
			}

			bool WriteChars(const char *,int64);													///<写入一个字符串
			bool WriteChars(const char16_t *,int64);													///<写入一个字符串

			template<typename N>
			bool WriteString(const BaseString<N> &str)												///<写入一个字符串
			{
				return WriteChars(str.c_str(),str.Length);
			}

			bool WriteLineEnd()																		///<写入一个换行符
			{
				return WriteChars(HGL_LINE_END,HGL_LINE_END_SIZE);
			}

			template<typename N>
			bool WriteLine(const N *str,int64 size)
			{
				if(!WriteChars(str,size))
					return(false);

				return WriteLineEnd();
			}

			template<typename N>
			bool WriteLine(const BaseString<N> &str)
			{
				return WriteLine(str.c_str(),str.Length());
			}

			/**
			 * 向流中写入一个文本(注：整体按标准文本文件模式)
			 */
			template<typename N>
			bool WriteText(const StringList<N> &sl)
			{
				const int count=sl.Count;

				for(int i=0;i<count;i++)
				{
					const BaseString<N> &str=sl[i];

					if(!WriteLine(str.c_str(),str.Length()))
						return(false);
				}

				return(true);
			}

			/**
			* 向流中写入一个字符串列表(注：整体为2进制流，不可用于文本文件)
			* @return 是否成功
			*/
			template<typename T>
			bool Write(const StringList<T> &sl)
			{
				const int count=sl.Count;

				if(!out->WriteInt32(count))					//写入行数
					return(false);

				if(count<=0)
					return(true);

				SharedArray<int32> line_length=new int32[count];
				SharedArray<T *> str_list=new T *[count];
				int32 *line_p=line_length;
				T **str_p=str_list;

				int32 total=0;

				for(int i=0;i<count;i++)
				{
					const BaseString<T> &str=sl[i];

					const int len=str.Length;

					total+=len;

					*line_p++=len;

					*str_list++=str.c_str();
				}

				if(out->WriteInt32(line_length,count)!=count)	//写入每行长度
					return(false);

				line_p=line_length;
				str_p=str_list;

				SharedArray<T> text=new T[total];
				T *text_p=text;

				for(int i=0;i<count;i++)
				{
					const int len=*line_p++;

					memcpy(text_p,*str_p++,len*sizeof(T));

					text_p+=len;
				}

				return WriteChars(text,total);
			}
		};//template<ByteOrderMask BOM> class TextOutputStream

        typedef TextOutputStream<bomUTF8   > UTF8TextOutputStream;
        typedef TextOutputStream<bomUTF16LE> UTF16LETextOutputStream;
        typedef TextOutputStream<bomUTF16BE> UTF16BETextOutputStream;
	}//namespace io
}//namespace hgl
#endif//HGL_IO_TEXT_OUTPUT_STREAM_INCLUDE
