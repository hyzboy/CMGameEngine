#include<hgl/type/StringList.h>
#include<hgl/File.h>
namespace hgl
{
	/**
	* 从指定文件中加载一个字符串列表<br>支持带BOM头的UTF8/UTF16LE/UTF16BE/UTF32LE/UTF32BE文本
	* @param sl 字符串列表
	* @param filename 文件名
	* @param cs 如果为ansi，将使用的转换代码页/字符集
	* @return 加载的字符串列表行数
	*/
	int LoadStringListBOM(UTF16StringList &sl,const OSString &filename,const CharSet &cs)
	{
		u16char *buf;
		const int64 size=LoadTxtToMemory(filename,&buf,cs);

		if(size<=0)
			return(0);

		const int line=SplitToStringList(sl,buf,size);

		delete[] buf;

		return(line);
	}

	/**
     * 加载一个原始文本文件到StringList，不支持BOM头
     */
    int LoadUTF8FileToStringList(UTF8StringList &sl,const OSString &filename)
    {
        char *data;

        const int size=LoadFileToMemory(filename,(void **)&data);

        if(size<=0)
            return size;

        const int line_count=SplitToStringList<char>(sl,data,size);

        delete[] data;
        return line_count;
    }

    int LoadUTF16FileToStringList(UTF16StringList &sl,const OSString &filename,bool need_swap)
    {
        u16char *data;

        const int size=LoadFileToMemory(filename,(void **)&data)/2;

        if(size<=0)
            return size;

        if(need_swap)
        {
            EndianSwap(data,size);
        }

        const int line_count=SplitToStringList<u16char>(sl,data,size);

        delete[] data;
        return line_count;
    }

//     int LoadUTF16LEFileToStringList (UTF16StringList  &sl,const OSString &filename){return LoadFileToStringList<u16char,bomUTF16LE  >(sl,filanem);} ///<从文件加载一个UTF16LE文本到UTF16LEStringList,不支持BOM头
//     int LoadUTF16BEFileToStringList (UTF16StringList  &sl,const OSString &filename){return LoadFileToStringList<u16char,bomUTF16BE  >(sl,filanem);} ///<从文件加载一个UTF16BE文本到UTF16BEStringList,不支持BOM头
}//namespace hgl
