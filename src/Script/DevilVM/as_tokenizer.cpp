#include "as_tokenizer.h"
#include "as_tokendef.h"

#include <hgl/Str.h> // strcmp()

namespace hgl
{
	CTokenizer::CTokenizer()
	{
	}

	CTokenizer::~CTokenizer()
	{
	}

	const u16char *GetTokenDefinition(int type)
	{
		if( type == ttUnrecognizedToken			) return U16_TEXT("<unrecognized token>");
		if( type == ttEnd							) return U16_TEXT("<end of file>");
		if( type == ttWhiteSpace					) return U16_TEXT("<white space>");
		if( type == ttOnelineComment				) return U16_TEXT("<one line comment>");
		if( type == ttMultilineComment				) return U16_TEXT("<multiple lines comment>");
		if( type == ttIdentifier					) return U16_TEXT("<identifier>");
		if( type == ttIntConstant					) return U16_TEXT("<integer constant>");
		if( type == ttFloatConstant				) return U16_TEXT("<float constant>");
		if( type == ttDoubleConstant				) return U16_TEXT("<double constant>");
		if( type == ttStringConstant				) return U16_TEXT("<string constant>");
		if( type == ttNonTerminatedStringConstant	) return U16_TEXT("<unterminated string constant>");
		if( type == ttBitsConstant					) return U16_TEXT("<bits constant>");
		if( type == ttHeredocStringConstant		) return U16_TEXT("<heredoc string constant>");

		for( uint n = 0; n < TokenWordsNumber; n++ )
			if( TokenWordsList[n].type == type )
				return TokenWordsList[n].word;

		return 0;
	}

	const u16char *GetTokenName(TokenType type)
	{
		for(uint i=0;i<TokenWordsNumber;i++)
			if(TokenWordsList[i].type==type)
				return(TokenWordsList[i].word);

		return(nullptr);
	}

	TokenType CTokenizer::GetToken(const u16char *source, uint sourceLength, uint *tokenLength)
	{
// 		assert(source != 0);
// 		assert(tokenLength != 0);

		this->source = source;
		this->sourceLength = sourceLength;

		ParseToken();

		// Copy the output to the token
		*tokenLength = this->tokenLength;

		return tokenType;
	}

	int CTokenizer::ParseToken()
	{
		if( IsWhiteSpace() ) return 0;
		if( IsComment()    ) return 0;
		if( IsConstant()   ) return 0;
		if( IsIdentifier() ) return 0;
		if( IsKeyWord()    ) return 0;

		// If none of the above this is an unrecognized token
		// We can find the length of the token by advancing
		// one step and trying to identify a token there
		tokenType = ttUnrecognizedToken;
		tokenLength = 1;

		return -1;
	}

	bool CTokenizer::IsWhiteSpace()
	{
		uint n;

		const uint numWhiteSpace = WhiteSpaceNumber;

		for( n = 0; n < sourceLength; n++ )
		{
			bool isWhiteSpace = false;
			for( uint w = 0; w < numWhiteSpace; w++ )
			{
				if( source[n] == WhiteSpace[w] )
				{
					isWhiteSpace = true;
					break;
				}
			}
			if( !isWhiteSpace )	break;
		}

		if( n > 0 )
		{
			tokenType = ttWhiteSpace;
			tokenLength = n;
			return true;
		}

		return false;
	}

	bool CTokenizer::IsComment()
	{
		if( sourceLength < 2 )
			return false;

		if( source[0] != '/' )
			return false;

		if( source[1] == '/' )
		{
			// One-line comment

			// Find the length
			uint n;
			for( n = 2; n < sourceLength; n++ )
			{
				if( source[n] == '\n' )
					break;
			}

			tokenType = ttOnelineComment;
			tokenLength = n+1;

			return true;
		}

		if( source[1] == '*' )
		{
			// Multi-line comment

			// Find the length
			uint n;
			for( n = 2; n < sourceLength-1; )
			{
				if( source[n++] == '*' && source[n] == '/' )
					break;
			}

			tokenType = ttMultilineComment;
			tokenLength = n+1;

			return true;
		}

		return false;
	}

	bool CTokenizer::IsConstant()
	{
		// Starting with number
		if( source[0] >= '0' && source[0] <= '9' )
		{
			// Is it a hexadecimal number?
			if( sourceLength >= 1 && (source[1] == 'x' || source[1] == 'X') )
			{
				uint n;
				for( n = 2; n < sourceLength; n++ )
				{
					if( !(source[n] >= '0' && source[n] <= '9') &&
						!(source[n] >= 'a' && source[n] <= 'f') &&
						!(source[n] >= 'A' && source[n] <= 'F') )
						break;
				}

				tokenType = ttBitsConstant;
				tokenLength = n;
				return true;
			}

			uint n;
			for( n = 1; n < sourceLength; n++ )
			{
				if( source[n] < '0' || source[n] > '9' )
					break;
			}

			if( n < sourceLength && source[n] == '.' )
			{
				n++;
				for( ; n < sourceLength; n++ )
				{
					if( source[n] < '0' || source[n] > '9' )
						break;
				}

				if( n < sourceLength && (source[n] == 'e' || source[n] == 'E') )
				{
					n++;
					if( n < sourceLength && (source[n] == '-' || source[n] == '+') )
						n++;

					for( ; n < sourceLength; n++ )
					{
						if( source[n] < '0' || source[n] > '9' )
							break;
					}
				}

				if( n < sourceLength && (source[n] == 'f' || source[n] == 'F') )
				{
					tokenType = ttFloatConstant;
					tokenLength = n + 1;
				}
				else
				{
					tokenType = ttDoubleConstant;
					tokenLength = n;
				}
				return true;
			}

			tokenType = ttIntConstant;
			tokenLength = n;
			return true;
		}

		// Character literal between single-quotes
		if( source[0] == '\'' )
		{
			bool evenSlashes = true;
			uint n;
			for( n = 1; n < sourceLength; n++ )
			{
				if( source[n] == '\n' ) break;
				if( source[n] == '\'' && evenSlashes )
				{
					tokenType = ttIntConstant;
					tokenLength = n+1;
					return true;
				}
				if( source[n] == '\\' ) evenSlashes = !evenSlashes; else evenSlashes = true;
			}

			tokenType = ttNonTerminatedStringConstant;
			tokenLength = n-1;

			return true;
		}

		// UTF16String constant between double-quotes
		if( source[0] == '"' )
		{
			// Is it a normal string constant or a heredoc string constant?
			if( sourceLength >= 6 && source[1] == '"' && source[2] == '"' )
			{
				// Heredoc string constant (spans multiple lines, no escape sequences)

				// Find the length
				uint n;
				for( n = 3; n < sourceLength-2; n++ )
				{
					if( source[n] == '"' && source[n+1] == '"' && source[n+2] == '"' )
						break;
				}

				tokenType = ttHeredocStringConstant;
				tokenLength = n+3;
			}
			else
			{
				// Normal string constant
				bool evenSlashes = true;
				uint n;
				for( n = 1; n < sourceLength; n++ )
				{
					if( source[n] == '\n' ) break;
					if( source[n] == '"' && evenSlashes )
					{
						tokenType = ttStringConstant;
						tokenLength = n+1;
						return true;
					}
					if( source[n] == '\\' ) evenSlashes = !evenSlashes; else evenSlashes = true;
				}

				tokenType = ttNonTerminatedStringConstant;
				tokenLength = n;
			}

			return true;
		}

		return false;
	}

	bool CTokenizer::IsIdentifier()
	{
		// Starting with letter or underscore
		if( (source[0] >= 'a' && source[0] <= 'z') ||
			(source[0] >= 'A' && source[0] <= 'Z') ||
			(source[0] == '_') )
		{
			tokenType = ttIdentifier;
			tokenLength = 1;

			for( uint n = 1; n < sourceLength; n++ )
			{
				if( (source[n] >= 'a' && source[n] <= 'z') ||
					(source[n] >= 'A' && source[n] <= 'Z') ||
					(source[n] >= '0' && source[n] <= '9') ||
					(source[n] == '_') )
					tokenLength++;
				else
					break;
			}

			// Make sure the identifier isn't a reserved keyword
			if( tokenLength > 50 ) return true;

			u16char test[51];
			memcpy(test, source, tokenLength*sizeof(u16char));
			test[tokenLength] = 0;

			for( uint i = 0; i < TokenWordsNumber; i++ )
			{
				//if( strcmp(test, TokenWordsList[i].word) == 0 )
				if( strcmp(test, TokenWordsList[i].word) == 0 )
					return false;
			}

			return true;
		}

		return false;
	}

	bool CTokenizer::IsKeyWord()
	{
		// Fill the list with all possible keywords
		// Check each character against all the keywords in the list,
		// remove keywords that don't match. When only one remains and
		// it matches the source completely we have found a match.
		int words[TokenWordsNumber];
		uint n;
		for( n = 0; n < TokenWordsNumber; n++ )
			words[n] = n;

		int numWords = TokenWordsNumber;
		int lastPossible = -1;

		n = 0;
		while( n < sourceLength && numWords >= 0 )
		{
			for( int i = 0; i < numWords; i++ )
			{
				if( TokenWordsList[words[i]].word[n] == '\0' )
				{
					if( numWords > 1 )
					{
						lastPossible = words[i];
						words[i--] = words[--numWords];
						continue;
					}
					else
					{
						tokenType = TokenWordsList[words[i]].type;
						tokenLength = n;
						return true;
					}
				}

				if( TokenWordsList[words[i]].word[n] != source[n] )
				{
					words[i--] = words[--numWords];
				}
			}
			n++;
		}

		// The source length ended or there where no more matchable words
		if( numWords )
		{
			// If any of the TokenWordsList also end at this
			// position then we have found the matching token
			for( int i = 0; i < numWords; i++ )
			{
				if( TokenWordsList[words[i]].word[n] == '\0' )
				{
					tokenType = TokenWordsList[words[i]].type;
					tokenLength = n;
					return true;
				}
			}
		}

		// It is still possible that a shorter token was found
		if( lastPossible > -1 )
		{
			tokenType = TokenWordsList[lastPossible].type;
			//tokenLength = strlen(TokenWordsList[lastPossible].word);
			tokenLength = strlen(TokenWordsList[lastPossible].word);
			return true;
		}

		return false;
	}
}//namespace hgl
