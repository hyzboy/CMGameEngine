/*
   AngelCode Scripting Library
   Copyright (c) 2003-2007 Andreas Jonsson

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
      distribution.

   The original version of this library can be located at:
   http://www.angelcode.com/angelscript/

   Andreas Jonsson
   andreas@angelcode.com
*/


//
// as_tokendef.h
//
// Definitions for tokens identifiable by the tokenizer
//


#ifndef AS_TOKENDEF_H
#define AS_TOKENDEF_H

namespace angle_script
{
	enum eTokenType
	{
		ttUnrecognizedToken,

		ttEnd,				   // End of file

		// White space and comments
		ttWhiteSpace,          // ' ', '\t', '\r', '\n'
		ttOnelineComment,      // // \n
		ttMultilineComment,    // /* */

		// Atoms
		ttIdentifier,            // abc123
		ttIntConstant,           // 1234
		ttFloatConstant,         // 12.34e56f
		ttDoubleConstant,        // 12.34e56
		ttStringConstant,        // "123"
		ttHeredocStringConstant, // """text"""
		ttNonTerminatedStringConstant, // "123
		ttBitsConstant,          // 0xFFFF

		// Math operators
		ttPlus,                // +
		ttMinus,               // -
		ttStar,                // *
		ttSlash,               // /
		ttPercent,             // %

		ttHandle,              // #

		ttAddAssign,           // +=
		ttSubAssign,           // -=
		ttMulAssign,           // *=
		ttDivAssign,           // /=
		ttModAssign,           // %=

		ttOrAssign,            // |=
		ttAndAssign,           // &=
		ttXorAssign,           // ^=
		ttShiftLeftAssign,     // <<=
		ttShiftRightLAssign,   // >>=
		ttShiftRightAAssign,   // >>>=

		ttInc,                 // ++
		ttDec,                 // --

		ttDot,                 // .

		// Statement tokens
		ttAssignment,          // =
		ttEndStatement,        // ;
		ttListSeparator,       // ,
		ttStartStatementBlock, // {
		ttEndStatementBlock,   // }
		ttOpenParanthesis,     // (
		ttCloseParanthesis,    // )
		ttOpenBracket,         // [
		ttCloseBracket,        // ]
		ttAmp,                 // &

		// Bitwise operators
		ttBitOr,               // |
		ttBitNot,              // ~
		ttBitXor,              // ^
		ttBitShiftLeft,        // <<
		ttBitShiftRight,       // >>
		ttBitShiftRightArith,  // >>>

		// Compare operators
		ttEqual,               // ==
		ttNotEqual,            // !=
		ttLessThan,            // <
		ttGreaterThan,         // >
		ttLessThanOrEqual,     // <=
		ttGreaterThanOrEqual,  // >=

		ttQuestion,            // ?
		ttColon,               // :

		// Reserved keywords
		ttIf,                  // if
		ttElse,                // else
		ttFor,				   // for
		ttWhile,               // while
		ttBool,                // bool
		ttImport,              // import

		ttGoto,				   // goto

		ttEnum,				   // enum
		ttFunc,				   // function
		ttString,			   // string

		ttInt,                 // int
		ttInt8,                // int8
		ttInt16,               // int16
		ttInt64,               // int64
		ttInterface,           // interface
		ttUInt,                // uint
		ttUInt8,               // uint8
		ttUInt16,              // uint16
		ttUInt64,              // uint64
		ttFloat,               // float
		ttVoid,                // void
		ttTrue,                // true
		ttFalse,               // false
		ttReturn,              // return
		ttNot,                 // not
		ttAnd,				   // and
		ttOr,				   // or
		ttXor,                 // xor
		ttBreak,               // break
		ttContinue,            // continue
		ttConst,			   // const
		ttDo,                  // do
		ttDouble,              // double
		ttSwitch,              // switch
		ttCase,                // case
		ttDefault,             // default
		ttIn,                  // in
		ttOut,                 // out
		ttInOut,               // inout
		ttNull,                // null
		ttClass,               // class
		ttCast                 // cast
	};

	struct sTokenWord
	{
		const char16_t *word;
		eTokenType   tokenType;
	};

	sTokenWord const tokenWords[] =
	{
		{L"+"         , ttPlus},
		{L"-"         , ttMinus},
		{L"*"         , ttStar},
		{L"/"         , ttSlash},
		{L"%"         , ttPercent},
		{L"="         , ttAssignment},
		{L"."         , ttDot},
		{L"+="        , ttAddAssign},
		{L"-="        , ttSubAssign},
		{L"*="        , ttMulAssign},
		{L"/="        , ttDivAssign},
		{L"%="        , ttModAssign},
		{L"|="        , ttOrAssign},
		{L"&="        , ttAndAssign},
		{L"^="        , ttXorAssign},
		{L"<<="       , ttShiftLeftAssign},
		{L">>="       , ttShiftRightLAssign},
		{L">>>="      , ttShiftRightAAssign},
		{L"|"         , ttBitOr},
		{L"~"         , ttBitNot},
		{L"^"         , ttBitXor},
		{L"<<"        , ttBitShiftLeft},
		{L">>"        , ttBitShiftRight},
		{L">>>"       , ttBitShiftRightArith},
		{L";"         , ttEndStatement},
		{L","         , ttListSeparator},
		{L"{"         , ttStartStatementBlock},
		{L"}"         , ttEndStatementBlock},
		{L"("         , ttOpenParanthesis},
		{L")"         , ttCloseParanthesis},
		{L"["         , ttOpenBracket},
		{L"]"         , ttCloseBracket},
		{L"?"         , ttQuestion},
		{L":"         , ttColon},
		{L"=="        , ttEqual},
		{L"!="        , ttNotEqual},
		{L"<"         , ttLessThan},
		{L">"         , ttGreaterThan},
		{L"<="        , ttLessThanOrEqual},
		{L">="        , ttGreaterThanOrEqual},
		{L"++"        , ttInc},
		{L"--"        , ttDec},
		{L"&"         , ttAmp},
		{L"!"         , ttNot},
		{L"||"        , ttOr},
		{L"&&"        , ttAnd},
		{L"^^"        , ttXor},
		{L"@"         , ttHandle},
		{L"and"       , ttAnd},
	#ifdef AS_DEPRECATED
		{L"bits"      , ttUInt},
		{L"bits8"     , ttUInt8},
		{L"bits16"    , ttUInt16},
		{L"bits32"    , ttUInt},
	#endif
		{L"bool"      , ttBool},
		{L"break"     , ttBreak},
		{L"cast"      , ttCast},
		{L"const"     , ttConst},
		{L"continue"  , ttContinue},
		{L"do"        , ttDo},
	#ifdef  AS_USE_DOUBLE_AS_FLOAT
		{L"double"    , ttFloat},
	#else
		{L"double"    , ttDouble},
	#endif
		{L"else"      , ttElse},
		{L"false"     , ttFalse},
		{L"float"     , ttFloat},
		{L"for"       , ttFor},
		{L"goto"	  , ttGoto},
		{L"GOTO"	  , ttGoto},
		{L"if"        , ttIf},
		{L"in"        , ttIn},
		{L"inout"     , ttInOut},
		{L"import"    , ttImport},

		{L"enum"	  , ttEnum},
		{L"func"	  , ttFunc},
		{L"string"	  , ttString},

		{L"int"       , ttInt},
		{L"int8"      , ttInt8},
		{L"int16"     , ttInt16},
		{L"int32"     , ttInt},
		{L"int64"     , ttInt64},
		{L"interface" , ttInterface},
		{L"not"       , ttNot},
		{L"null"      , ttNull},
		{L"or"        , ttOr},
		{L"out"       , ttOut},
		{L"return"    , ttReturn},
		{L"true"      , ttTrue},
		{L"void"      , ttVoid},
		{L"while"     , ttWhile},
		{L"uint"      , ttUInt},
		{L"uint8"     , ttUInt8},
		{L"uint16"    , ttUInt16},
		{L"uint32"    , ttUInt},
		{L"uint64"    , ttUInt64},
		{L"switch"    , ttSwitch},
		{L"class"     , ttClass},
		{L"case"      , ttCase},
		{L"CASE"	  , ttCase},
		{L"default"   , ttDefault},
		{L"xor"       , ttXor},
	};

	const unsigned int numTokenWords = sizeof(tokenWords)/sizeof(sTokenWord);

	const char16_t * const whiteSpace =u" \t\r\n\xFEFF";
	const int whiteSpaceNumber=5;
}//namespace angle_script
#endif
