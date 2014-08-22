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
		const u16char *word;
		eTokenType   tokenType;
	};

	sTokenWord const tokenWords[] =
	{
		{u"+"         , ttPlus},
		{u"-"         , ttMinus},
		{u"*"         , ttStar},
		{u"/"         , ttSlash},
		{u"%"         , ttPercent},
		{u"="         , ttAssignment},
		{u"."         , ttDot},
		{u"+="        , ttAddAssign},
		{u"-="        , ttSubAssign},
		{u"*="        , ttMulAssign},
		{u"/="        , ttDivAssign},
		{u"%="        , ttModAssign},
		{u"|="        , ttOrAssign},
		{u"&="        , ttAndAssign},
		{u"^="        , ttXorAssign},
		{u"<<="       , ttShiftLeftAssign},
		{u">>="       , ttShiftRightLAssign},
		{u">>>="      , ttShiftRightAAssign},
		{u"|"         , ttBitOr},
		{u"~"         , ttBitNot},
		{u"^"         , ttBitXor},
		{u"<<"        , ttBitShiftLeft},
		{u">>"        , ttBitShiftRight},
		{u">>>"       , ttBitShiftRightArith},
		{u";"         , ttEndStatement},
		{u","         , ttListSeparator},
		{u"{"         , ttStartStatementBlock},
		{u"}"         , ttEndStatementBlock},
		{u"("         , ttOpenParanthesis},
		{u")"         , ttCloseParanthesis},
		{u"["         , ttOpenBracket},
		{u"]"         , ttCloseBracket},
		{u"?"         , ttQuestion},
		{u":"         , ttColon},
		{u"=="        , ttEqual},
		{u"!="        , ttNotEqual},
		{u"<"         , ttLessThan},
		{u">"         , ttGreaterThan},
		{u"<="        , ttLessThanOrEqual},
		{u">="        , ttGreaterThanOrEqual},
		{u"++"        , ttInc},
		{u"--"        , ttDec},
		{u"&"         , ttAmp},
		{u"!"         , ttNot},
		{u"||"        , ttOr},
		{u"&&"        , ttAnd},
		{u"^^"        , ttXor},
		{u"@"         , ttHandle},
		{u"and"       , ttAnd},
	#ifdef AS_DEPRECATED
		{u"bits"      , ttUInt},
		{u"bits8"     , ttUInt8},
		{u"bits16"    , ttUInt16},
		{u"bits32"    , ttUInt},
	#endif
		{u"bool"      , ttBool},
		{u"break"     , ttBreak},
		{u"cast"      , ttCast},
		{u"const"     , ttConst},
		{u"continue"  , ttContinue},
		{u"do"        , ttDo},
	#ifdef  AS_USE_DOUBLE_AS_FLOAT
		{u"double"    , ttFloat},
	#else
		{u"double"    , ttDouble},
	#endif
		{u"else"      , ttElse},
		{u"false"     , ttFalse},
		{u"float"     , ttFloat},
		{u"for"       , ttFor},
		{u"goto"	  , ttGoto},
		{u"GOTO"	  , ttGoto},
		{u"if"        , ttIf},
		{u"in"        , ttIn},
		{u"inout"     , ttInOut},
		{u"import"    , ttImport},

		{u"enum"	  , ttEnum},
		{u"func"	  , ttFunc},
		{u"string"	  , ttString},

		{u"int"       , ttInt},
		{u"int8"      , ttInt8},
		{u"int16"     , ttInt16},
		{u"int32"     , ttInt},
		{u"int64"     , ttInt64},
		{u"interface" , ttInterface},
		{u"not"       , ttNot},
		{u"null"      , ttNull},
		{u"or"        , ttOr},
		{u"out"       , ttOut},
		{u"return"    , ttReturn},
		{u"true"      , ttTrue},
		{u"void"      , ttVoid},
		{u"while"     , ttWhile},
		{u"uint"      , ttUInt},
		{u"uint8"     , ttUInt8},
		{u"uint16"    , ttUInt16},
		{u"uint32"    , ttUInt},
		{u"uint64"    , ttUInt64},
		{u"switch"    , ttSwitch},
		{u"class"     , ttClass},
		{u"case"      , ttCase},
		{u"CASE"	  , ttCase},
		{u"default"   , ttDefault},
		{u"xor"       , ttXor},
	};

	const unsigned int numTokenWords = sizeof(tokenWords)/sizeof(sTokenWord);

	const u16char * const whiteSpace =u" \t\r\n\xFEFF";
	const int whiteSpaceNumber=5;
}//namespace angle_script
#endif
