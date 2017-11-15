#ifndef HGL_TOKENDEF_INCLUDE
#define HGL_TOKENDEF_INCLUDE

/**
 * like C Token 定义，此模块源自Angel script
 */

namespace hgl
{
	enum TokenType
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
    };//enum TokenType

	struct TokenWord
	{
		const u16char *word;
		TokenType   type;
	};

	constexpr TokenWord TokenWordsList[] =
	{
		{U16_TEXT("+")        , ttPlus},
		{U16_TEXT("-")        , ttMinus},
		{U16_TEXT("*")        , ttStar},
		{U16_TEXT("/")        , ttSlash},
		{U16_TEXT("%")        , ttPercent},
		{U16_TEXT("=")        , ttAssignment},
		{U16_TEXT(".")        , ttDot},
		{U16_TEXT("+=")       , ttAddAssign},
		{U16_TEXT("-=")       , ttSubAssign},
		{U16_TEXT("*=")       , ttMulAssign},
		{U16_TEXT("/=")       , ttDivAssign},
		{U16_TEXT("%=")       , ttModAssign},
		{U16_TEXT("|=")       , ttOrAssign},
		{U16_TEXT("&=")       , ttAndAssign},
		{U16_TEXT("^=")       , ttXorAssign},
		{U16_TEXT("<<=")      , ttShiftLeftAssign},
		{U16_TEXT(">>=")      , ttShiftRightLAssign},
		{U16_TEXT(">>>=")     , ttShiftRightAAssign},
		{U16_TEXT("|")        , ttBitOr},
		{U16_TEXT("~")        , ttBitNot},
		{U16_TEXT("^")        , ttBitXor},
		{U16_TEXT("<<")       , ttBitShiftLeft},
		{U16_TEXT(">>")       , ttBitShiftRight},
		{U16_TEXT(">>>")      , ttBitShiftRightArith},
		{U16_TEXT(";")        , ttEndStatement},
		{U16_TEXT(",")        , ttListSeparator},
		{U16_TEXT("{")        , ttStartStatementBlock},
		{U16_TEXT("}")        , ttEndStatementBlock},
		{U16_TEXT("(")        , ttOpenParanthesis},
		{U16_TEXT(")")        , ttCloseParanthesis},
		{U16_TEXT("[")        , ttOpenBracket},
		{U16_TEXT("]")        , ttCloseBracket},
		{U16_TEXT("?")        , ttQuestion},
		{U16_TEXT(":")        , ttColon},
		{U16_TEXT("==")       , ttEqual},
		{U16_TEXT("!=")       , ttNotEqual},
		{U16_TEXT("<")        , ttLessThan},
		{U16_TEXT(">")        , ttGreaterThan},
		{U16_TEXT("<=")       , ttLessThanOrEqual},
		{U16_TEXT(">=")       , ttGreaterThanOrEqual},
		{U16_TEXT("++")       , ttInc},
		{U16_TEXT("--")       , ttDec},
		{U16_TEXT("&")        , ttAmp},
		{U16_TEXT("!")        , ttNot},
		{U16_TEXT("||")       , ttOr},
		{U16_TEXT("&&")       , ttAnd},
		{U16_TEXT("^^")       , ttXor},
		{U16_TEXT("@")        , ttHandle},
		{U16_TEXT("and")      , ttAnd},
		{U16_TEXT("bool")     , ttBool},
		{U16_TEXT("break")    , ttBreak},
		{U16_TEXT("cast")     , ttCast},
		{U16_TEXT("const")    , ttConst},
		{U16_TEXT("continue") , ttContinue},
		{U16_TEXT("do")       , ttDo},
		{U16_TEXT("double")   , ttDouble},
		{U16_TEXT("else")     , ttElse},
		{U16_TEXT("false")    , ttFalse},
		{U16_TEXT("float")    , ttFloat},
		{U16_TEXT("for")      , ttFor},
		{U16_TEXT("goto")	  , ttGoto},
		{U16_TEXT("GOTO")	  , ttGoto},
		{U16_TEXT("if")       , ttIf},
		{U16_TEXT("in")       , ttIn},
		{U16_TEXT("inout")    , ttInOut},
		{U16_TEXT("import")   , ttImport},

		{U16_TEXT("enum")	  , ttEnum},
		{U16_TEXT("func")	  , ttFunc},
		{U16_TEXT("string")	  , ttString},

		{U16_TEXT("int")      , ttInt},
		{U16_TEXT("int8")     , ttInt8},
		{U16_TEXT("int16")    , ttInt16},
		{U16_TEXT("int32")    , ttInt},
		{U16_TEXT("int64")    , ttInt64},
		{U16_TEXT("interface"), ttInterface},
		{U16_TEXT("not")      , ttNot},
		{U16_TEXT("null")     , ttNull},
		{U16_TEXT("or")       , ttOr},
		{U16_TEXT("out")      , ttOut},
		{U16_TEXT("return")   , ttReturn},
		{U16_TEXT("true")     , ttTrue},
		{U16_TEXT("void")     , ttVoid},
		{U16_TEXT("while")    , ttWhile},
		{U16_TEXT("uint")     , ttUInt},
		{U16_TEXT("uint8")    , ttUInt8},
		{U16_TEXT("uint16")   , ttUInt16},
		{U16_TEXT("uint32")   , ttUInt},
		{U16_TEXT("uint64")   , ttUInt64},
		{U16_TEXT("switch")   , ttSwitch},
		{U16_TEXT("class")    , ttClass},
		{U16_TEXT("case")     , ttCase},
		{U16_TEXT("CASE")	  , ttCase},
		{U16_TEXT("default")  , ttDefault},
		{U16_TEXT("xor")      , ttXor},
	};

    constexpr int       TokenWordsNumber=sizeof(TokenWordsList)/sizeof(TokenWord);

	constexpr u16char * WhiteSpace      =U16_TEXT(" \t\r\n\xFEFF");
    constexpr int       WhiteSpaceNumber=sizeof(WhiteSpace)/sizeof(u16char);
}//namespace hgl
#endif//HGL_TOKENDEF_INCLUDE
