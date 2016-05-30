#include<hgl/script/AngelVM.h>
#include<angelscript.h>
#include<hgl/type/BaseString.h>
#include<assert.h>
#include<new>

namespace hgl
{
	using namespace AngelScript;

	static UTF8String StringFactory(asUINT length,const char *str)
	{
		return UTF8String(str,length);
	}

	static void ConstructString(UTF8String *thisPointer)
	{
		new(thisPointer) UTF8String();
	}

	static void CopyConstructString(const UTF8String &other, UTF8String *thisPointer)
	{
		new(thisPointer) UTF8String(other);
	}

	static void DestructString(UTF8String *thisPointer)
	{
		thisPointer->~UTF8String();
	}

	static UTF8String &AddAssignStringToString(const UTF8String &str, UTF8String &dest)
	{
		// We don't register the method directly because some compilers
		// and standard libraries inline the definition, resulting in the
		// linker being unable to find the declaration.
		// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
		dest += str;
		return dest;
	}

	// bool string::isEmpty()
	// bool string::empty() // if AS_USE_STLNAMES == 1
	static bool StringIsEmpty(const UTF8String &str)
	{
		// We don't register the method directly because some compilers
		// and standard libraries inline the definition, resulting in the
		// linker being unable to find the declaration
		// Example: CLang/LLVM with XCode 4.3 on OSX 10.7
		return str.IsEmpty();
	}

	static UTF8String &AssignUIntToString(unsigned int i, UTF8String &dest)
	{
		dest=UTF8String(i);
		return dest;
	}

	static UTF8String &AddAssignUIntToString(unsigned int i, UTF8String &dest)
	{
		dest+=UTF8String(i);
		return dest;
	}

	static UTF8String AddStringUInt(const UTF8String &str, unsigned int i)
	{
		return str + UTF8String(i);
	}

	static UTF8String AddIntString(int i, const UTF8String &str)
	{
		return UTF8String(i) + str;
	}

	static UTF8String &AssignIntToString(int i, UTF8String &dest)
	{
		dest=UTF8String(i);
		return dest;
	}

	static UTF8String &AddAssignIntToString(int i, UTF8String &dest)
	{
		dest+=UTF8String(i);
		return dest;
	}

	static UTF8String AddStringInt(const UTF8String &str, int i)
	{
		return str + UTF8String(i);
	}

	static UTF8String AddUIntString(unsigned int i, const UTF8String &str)
	{
		return UTF8String(i) + str;
	}

	static UTF8String &AssignDoubleToString(double f, UTF8String &dest)
	{
		dest=UTF8String(f);
		return dest;
	}

	static UTF8String &AddAssignDoubleToString(double f, UTF8String &dest)
	{
		dest+=UTF8String(f);
		return dest;
	}

	static UTF8String &AssignBoolToString(bool b, UTF8String &dest)
	{
		dest=b?"true":"false";
		return dest;
	}

	static UTF8String &AddAssignBoolToString(bool b, UTF8String &dest)
	{
		dest+=b?"true":"false";
		return dest;
	}

	static UTF8String AddStringDouble(const UTF8String &str, double f)
	{
		return str + UTF8String(f);
	}

	static UTF8String AddDoubleString(double f, const UTF8String &str)
	{
		return UTF8String(f) + str;
	}

	static UTF8String AddStringBool(const UTF8String &str, bool b)
	{
		return str + (b?"true":"false");
	}

	static UTF8String AddBoolString(bool b, const UTF8String &str)
	{
		return (b?"true":"false") + str;
	}

	static char *StringCharAt(unsigned int i, UTF8String &str)
	{
		if( i >= str.Length() )
		{
			// Set a script exception
			asIScriptContext *ctx = asGetActiveContext();
			ctx->SetException("Out of range");

			// Return a null pointer
			return 0;
		}

		return str.c_str()+i;
	}

	// AngelScript signature:
	// int string::opCmp(const string &in) const
	static int StringCmp(const UTF8String &a, const UTF8String &b)
	{
		return a.Comp(b);
	}

	// This function returns the index of the first position where the substring
	// exists in the input string. If the substring doesn't exist in the input
	// string -1 is returned.
	//
	// AngelScript signature:
	// int string::findFirst(const string &in sub, uint start = 0) const
	static int StringFindFirst(const UTF8String &sub, asUINT start, const UTF8String &str)
	{
		// We don't register the method directly because the argument types change between 32bit and 64bit platforms
		return str.FindString(sub,start);
	}

	// This function returns the index of the last position where the substring
	// exists in the input string. If the substring doesn't exist in the input
	// string -1 is returned.
	//
	// AngelScript signature:
	// int string::findLast(const string &in sub, int start = -1) const
// 	static int StringFindLast(const string &sub, int start, const string &str)
// 	{
// 		// We don't register the method directly because the argument types change between 32bit and 64bit platforms
// 		return (int)str.rfind(sub, (size_t)start);
// 	}

	// AngelScript signature:
	// uint string::length() const
	static asUINT StringLength(const UTF8String &str)
	{
		// We don't register the method directly because the return type changes between 32bit and 64bit platforms
		return (asUINT)str.Length();
	}


	// AngelScript signature:
	// void string::resize(uint l)
	static void StringResize(asUINT l, UTF8String &str)
	{
		// We don't register the method directly because the argument types change between 32bit and 64bit platforms
		str.Resize(l);
	}

	// AngelScript signature:
	// string formatInt(int64 val, const string &in options, uint width)
// 	static string formatInt(asINT64 value, const string &options, asUINT width)
// 	{
// 		bool leftJustify = options.find("l") != -1;
// 		bool padWithZero = options.find("0") != -1;
// 		bool alwaysSign  = options.find("+") != -1;
// 		bool spaceOnSign = options.find(" ") != -1;
// 		bool hexSmall    = options.find("h") != -1;
// 		bool hexLarge    = options.find("H") != -1;
//
// 		string fmt = "%";
// 		if( leftJustify ) fmt += "-";
// 		if( alwaysSign ) fmt += "+";
// 		if( spaceOnSign ) fmt += " ";
// 		if( padWithZero ) fmt += "0";
//
// 	#ifdef __GNUC__
// 	#ifdef _LP64
// 		fmt += "*l";
// 	#else
// 		fmt += "*ll";
// 	#endif
// 	#else
// 		fmt += "*I64";
// 	#endif
//
// 		if( hexSmall ) fmt += "x";
// 		else if( hexLarge ) fmt += "X";
// 		else fmt += "d";
//
// 		string buf;
// 		buf.resize(width+20);
// 	#if _MSC_VER >= 1400 && !defined(AS_MARMALADE)// MSVC 8.0 / 2005
// 		sprintf_s(&buf[0], buf.size(), fmt.c_str(), width, value);
// 	#else
// 		sprintf(&buf[0], fmt.c_str(), width, value);
// 	#endif
// 		buf.resize(strlen(&buf[0]));
//
// 		return buf;
// 	}
//
// 	// AngelScript signature:
// 	// string formatFloat(double val, const string &in options, uint width, uint precision)
// 	static string formatFloat(double value, const string &options, asUINT width, asUINT precision)
// 	{
// 		bool leftJustify = options.find("l") != -1;
// 		bool padWithZero = options.find("0") != -1;
// 		bool alwaysSign  = options.find("+") != -1;
// 		bool spaceOnSign = options.find(" ") != -1;
// 		bool expSmall    = options.find("e") != -1;
// 		bool expLarge    = options.find("E") != -1;
//
// 		string fmt = "%";
// 		if( leftJustify ) fmt += "-";
// 		if( alwaysSign ) fmt += "+";
// 		if( spaceOnSign ) fmt += " ";
// 		if( padWithZero ) fmt += "0";
//
// 		fmt += "*.*";
//
// 		if( expSmall ) fmt += "e";
// 		else if( expLarge ) fmt += "E";
// 		else fmt += "f";
//
// 		string buf;
// 		buf.resize(width+precision+50);
// 	#if _MSC_VER >= 1400 && !defined(AS_MARMALADE)// MSVC 8.0 / 2005
// 		sprintf_s(&buf[0], buf.size(), fmt.c_str(), width, precision, value);
// 	#else
// 		sprintf(&buf[0], fmt.c_str(), width, precision, value);
// 	#endif
// 		buf.resize(strlen(&buf[0]));
//
// 		return buf;
// 	}

	// AngelScript signature:
	// int64 parseInt(const string &in val, uint base = 10, uint &out byteCount = 0)
// 	static asINT64 parseInt(const string &val, asUINT base, asUINT *byteCount)
// 	{
// 		// Only accept base 10 and 16
// 		if( base != 10 && base != 16 )
// 		{
// 			if( byteCount ) *byteCount = 0;
// 			return 0;
// 		}
//
// 		const char *end = &val[0];
//
// 		// Determine the sign
// 		bool sign = false;
// 		if( *end == '-' )
// 		{
// 			sign = true;
// 			*end++;
// 		}
// 		else if( *end == '+' )
// 			*end++;
//
// 		asINT64 res = 0;
// 		if( base == 10 )
// 		{
// 			while( *end >= '0' && *end <= '9' )
// 			{
// 				res *= 10;
// 				res += *end++ - '0';
// 			}
// 		}
// 		else if( base == 16 )
// 		{
// 			while( (*end >= '0' && *end <= '9') ||
// 				(*end >= 'a' && *end <= 'f') ||
// 				(*end >= 'A' && *end <= 'F') )
// 			{
// 				res *= 16;
// 				if( *end >= '0' && *end <= '9' )
// 					res += *end++ - '0';
// 				else if( *end >= 'a' && *end <= 'f' )
// 					res += *end++ - 'a' + 10;
// 				else if( *end >= 'A' && *end <= 'F' )
// 					res += *end++ - 'A' + 10;
// 			}
// 		}
//
// 		if( byteCount )
// 			*byteCount = asUINT(size_t(end - val.c_str()));
//
// 		if( sign )
// 			res = -res;
//
// 		return res;
// 	}
//
// 	// AngelScript signature:
// 	// double parseFloat(const string &in val, uint &out byteCount = 0)
// 	double parseFloat(const string &val, asUINT *byteCount)
// 	{
// 		char *end;
//
// 		// WinCE doesn't have setlocale. Some quick testing on my current platform
// 		// still manages to parse the numbers such as "3.14" even if the decimal for the
// 		// locale is ",".
// 	#if !defined(_WIN32_WCE) && !defined(ANDROID)
// 		// Set the locale to C so that we are guaranteed to parse the float value correctly
// 		char *orig = setlocale(LC_NUMERIC, 0);
// 		setlocale(LC_NUMERIC, "C");
// 	#endif
//
// 		double res = strtod(val.c_str(), &end);
//
// 	#if !defined(_WIN32_WCE) && !defined(ANDROID)
// 		// Restore the locale
// 		setlocale(LC_NUMERIC, orig);
// 	#endif
//
// 		if( byteCount )
// 			*byteCount = asUINT(size_t(end - val.c_str()));
//
// 		return res;
// 	}

	static UTF8String StringSubString(asUINT start, int count, const UTF8String &str)
	{
		// Check for out-of-bounds
		UTF8String ret;

		if( start < str.Length() && count != 0 )
		{
			ret = str;
			ret.Clip(start,count);
		}

		return ret;
	}

	static bool StringEquals(const UTF8String& lhs, const UTF8String& rhs)
	{
		return lhs == rhs;
	}

	void RegistryString(asIScriptEngine *engine)
	{
		int r;

		r = engine->RegisterObjectType("string", sizeof(UTF8String), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

		r = engine->RegisterStringFactory("string", asFUNCTION(StringFactory), asCALL_CDECL); assert( r >= 0 );

		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(const string &in)",    asFUNCTION(CopyConstructString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructString),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asMETHODPR(UTF8String, operator =, (const UTF8String &), UTF8String &), asCALL_THISCALL); assert( r >= 0 );
		// Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asFUNCTION(AddAssignStringToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//	r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asMETHODPR(string, operator+=, (const string&), string&), asCALL_THISCALL); assert( r >= 0 );

		// Need to use a wrapper for operator== otherwise gcc 4.7 fails to compile
		r = engine->RegisterObjectMethod("string", "bool opEquals(const string &in) const", asFUNCTIONPR(StringEquals, (const UTF8String &, const UTF8String &), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asFUNCTION(StringCmp), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asFUNCTIONPR(operator +, (const UTF8String &, const UTF8String &), UTF8String), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		// The string length can be accessed through methods or through virtual property
		r = engine->RegisterObjectMethod("string", "uint length() const", asFUNCTION(StringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "void resize(uint)", asFUNCTION(StringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "uint get_length() const", asFUNCTION(StringLength), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "void set_length(uint)", asFUNCTION(StringResize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		// Need to use a wrapper on Mac OS X 10.7/XCode 4.3 and CLang/LLVM, otherwise the linker fails
	//	r = engine->RegisterObjectMethod("string", "bool isEmpty() const", asMETHOD(string, empty), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "bool isEmpty() const", asFUNCTION(StringIsEmpty), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		// Register the index operator, both as a mutator and as an inspector
		// Note that we don't register the operator[] directly, as it doesn't do bounds checking
		r = engine->RegisterObjectMethod("string", "uint8 &opIndex(uint)", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "const uint8 &opIndex(uint) const", asFUNCTION(StringCharAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		// Automatic conversion from values
		r = engine->RegisterObjectMethod("string", "string &opAssign(double)", asFUNCTION(AssignDoubleToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(double)", asFUNCTION(AddAssignDoubleToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd(double) const", asFUNCTION(AddStringDouble), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd_r(double) const", asFUNCTION(AddDoubleString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string &opAssign(int)", asFUNCTION(AssignIntToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(int)", asFUNCTION(AddAssignIntToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd(int) const", asFUNCTION(AddStringInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd_r(int) const", asFUNCTION(AddIntString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string &opAssign(uint)", asFUNCTION(AssignUIntToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(uint)", asFUNCTION(AddAssignUIntToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd(uint) const", asFUNCTION(AddStringUInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd_r(uint) const", asFUNCTION(AddUIntString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string &opAssign(bool)", asFUNCTION(AssignBoolToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(bool)", asFUNCTION(AddAssignBoolToString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd(bool) const", asFUNCTION(AddStringBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opAdd_r(bool) const", asFUNCTION(AddBoolString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		// Utilities
		r = engine->RegisterObjectMethod("string", "string substr(uint start = 0, int count = -1) const", asFUNCTION(StringSubString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "int findFirst(const string &in, uint start = 0) const", asFUNCTION(StringFindFirst), asCALL_CDECL_OBJLAST); assert( r >= 0 );
//		r = engine->RegisterObjectMethod("string", "int findLast(const string &in, int start = -1) const", asFUNCTION(StringFindLast), asCALL_CDECL_OBJLAST); assert( r >= 0 );

// 		r = engine->RegisterGlobalFunction("string formatInt(int64 val, const string &in options, uint width = 0)", asFUNCTION(formatInt), asCALL_CDECL); assert(r >= 0);
// 		r = engine->RegisterGlobalFunction("string formatFloat(double val, const string &in options, uint width = 0, uint precision = 0)", asFUNCTION(formatFloat), asCALL_CDECL); assert(r >= 0);
//		r = engine->RegisterGlobalFunction("int64 parseInt(const string &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(parseInt), asCALL_CDECL); assert(r >= 0);
//		r = engine->RegisterGlobalFunction("double parseFloat(const string &in, uint &out byteCount = 0)", asFUNCTION(parseFloat), asCALL_CDECL); assert(r >= 0);
	}
}//namespace hgl
