#ifndef HGL_TOKENIZER_INCLUDE
#define HGL_TOKENIZER_INCLUDE

#include <hgl/type/DataType.h>
#include "as_tokendef.h"

/**
 * like C Token 解晰，此模块源自Angel script
 */

namespace hgl
{
	const u16char *GetTokenDefinition(int type);
	const u16char *GetTokenName(TokenType);

	class CTokenizer
	{
	public:

		CTokenizer();
		~CTokenizer();

		TokenType GetToken(const u16char *source, hgl::uint sourceLength, hgl::uint *tokenLength);

	protected:

		int  ParseToken();
		bool IsWhiteSpace();
		bool IsComment();
		bool IsConstant();
		bool IsKeyWord();
		bool IsIdentifier();

		const u16char *source;
		hgl::uint sourceLength;

		TokenType tokenType;
		hgl::uint tokenLength;
	};//class CTokenizer
}//namespace hgl
#endif//HGL_TOKENIZER_INCLUDE
