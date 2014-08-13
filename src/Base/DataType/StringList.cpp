#include<hgl/type/StringList.h>
#include<hgl/File.h>
namespace hgl
{
	/**
	* 从指定文件中加载一个字符串列表
	* @param sl 字符串列表
	* @param filename 文件名
	* @param cs 如果为ansi，将使用的转换代码页/字符集
	* @return 加载的字符串列表行数
	*/
	int LoadStringList(UTF16StringList &sl,const OSString &filename,const CharSet &cs)
	{
		u16char *buf;
		const int64 size=LoadTxtToMemory(filename,&buf,cs);

		if(size<=0)
			return(0);

		const int line=SplitToStringList(sl,buf,size);

		delete[] buf;

		return(line);
	}
}//namespace hgl
