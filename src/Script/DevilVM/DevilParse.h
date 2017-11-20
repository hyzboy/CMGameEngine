#ifndef DevilParseH
#define DevilParseH

#include"as_tokenizer.h"
#include"DevilFunc.h"
#include<hgl/type/BaseString.h>

namespace hgl
{
	class DevilParse
	{
		DevilScriptModule *	module;

		const u16char *		source_start;

		const u16char *		source_cur;
		uint 				source_length;

		CTokenizer          parse;

	private:

		bool					ParseCode(DevilFunc *);												//解析一段代码

        template<typename T>
        bool                    ParseNumber(T &,const UTF16String &);

		DevilValueInterface *	ParseValue();														//解析一个量(属性/数值/真实函数调用)
		void					ParseValue(DevilFunc *,TokenType,UTF16String &);
		void					ParseEnum();

		#ifdef _DEBUG
		DevilCommand *			ParseFuncCall(UTF16String &,DevilFuncMap *,UTF16String &);
		#else
		DevilCommand *			ParseFuncCall(DevilFuncMap *);
		#endif//
		bool 					ParseIf(DevilFunc *);

		DevilCompInterface *	ParseComp();
		TokenType 				ParseCompType();

	public:

		DevilParse(DevilScriptModule *,const u16char *,int=-1);

		TokenType GetToken(UTF16String &);		//取得一个token,自动跳过注释、换行、空格
		TokenType CheckToken(UTF16String &);	//检测下一个token,自动跳过注释、换行、空格,但不取出

		bool GetToken(TokenType,UTF16String &);	//找某一种Token为止

    	bool ParseFunc(DevilFunc *);		//解析一个函数
	};
}//namespace hgl
#endif
