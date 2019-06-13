#include"angelscript.h"
#include<hgl/algorithm/VectorMath.h>
#include<assert.h>
#include<new>

namespace hgl
{
    using namespace algorithm;
    using namespace AngelScript;

    void vector3_Constructor(void *ptr)
    {
        new(ptr) Vector3f(0,0,0);
    }

    void vector3_Constructor(float x, float y, float z, void *ptr)
    {
        new(ptr) Vector3f(x,y,z);
    }

    void vector2_Constructor(void *ptr)
    {
        new(ptr) Vector2f(0,0);
    }

    void vector2_Constructor(float x, float y, void *ptr)
    {
        new(ptr) Vector2f(x,y);
    }

    bool        vector2_equal   (const Vector2f &lhs,const Vector2f &rhs){return(lhs==rhs);}
    bool        vector3_equal   (const Vector3f &lhs,const Vector3f &rhs){return(lhs==rhs);}

    Vector2f    vector2_add     (const Vector2f &lhs,const Vector2f &rhs){return(lhs+rhs);}
    Vector3f    vector3_add     (const Vector3f &lhs,const Vector3f &rhs){return(lhs+rhs);}

    Vector2f    vector2_sub     (const Vector2f &lhs,const Vector2f &rhs){return(lhs-rhs);}
    Vector3f    vector3_sub     (const Vector3f &lhs,const Vector3f &rhs){return(lhs-rhs);}

    Vector2f    vector2_mul     (const Vector2f &lhs,const float &rhs)  {return(lhs*rhs);}
    Vector3f    vector3_mul     (const Vector3f &lhs,const float &rhs)  {return(lhs*rhs);}

    Vector2f    vector2_div     (const Vector2f &lhs,const float &rhs)  {return(lhs/rhs);}
    Vector3f    vector3_div     (const Vector3f &lhs,const float &rhs)  {return(lhs/rhs);}

    Vector2f    vector2_neg     (const Vector2f &val){return -val;}
    Vector3f    vector3_neg     (const Vector3f &val){return -val;}

    Vector2f    vector2_mul_r   (const float &lhs,const Vector2f &rhs)  {return(lhs*rhs);}
    Vector3f    vector3_mul_r   (const float &lhs,const Vector3f &rhs)  {return(lhs*rhs);}
/*
    float       vector2_x       (const Vector2f &val){return val[0];}
    float       vector2_y       (const Vector2f &val){return val[1];}

    float       vector3_x       (const Vector3f &val){return val[0];}
    float       vector3_y       (const Vector3f &val){return val[1];}
    float       vector3_z       (const Vector3f &val){return val[2];}*/

    // The modf function doesn't seem very intuitive, so I'm writing this
    // function that simply returns the fractional part of the float value
    float fraction(float v)
    {
        float intPart;
        return modff(v, &intPart);
    }

    int rand2(int a,int b)
    {
        if(a>b)
            return b+(rand()%(a-b+1));
        else
            return a+(rand()%(b-a+1));
    }

    float rand_percentage()
    {
        return float(rand()%10001)/10000.0f;
    }

    void RegistryMath(asIScriptEngine *engine)
    {
        int r;

        //-------------------------------
        // vec2
        //-------------------------------
        r = engine->RegisterObjectType("vec2", sizeof(Vector2f), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(vector2_Constructor, (void*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTIONPR(vector2_Constructor, (float, float, void*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "vec2 &opAssign(const vec2 &in)",      asMETHODPR(Vector2f, operator=, (const Vector2f &), Vector2f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "vec2 &opAddAssign(const vec2 &in)",   asMETHODPR(Vector2f, operator+=,(const Vector2f &), Vector2f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "vec2 &opSubAssign(const vec2 &in)",   asMETHODPR(Vector2f, operator-=,(const Vector2f &), Vector2f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "vec2 &opMulAssign(float)",            asMETHODPR(Vector2f, operator*=,(float),            Vector2f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "vec2 &opDivAssign(float)",            asMETHODPR(Vector2f, operator/=,(float),            Vector2f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec2", "bool opEquals(const vec2 &in) const", asFUNCTIONPR(vector2_equal, (const Vector2f &,  const Vector2f &),  bool    ), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "vec2 opAdd(const vec2 &in) const",    asFUNCTIONPR(vector2_add,   (const Vector2f &,  const Vector2f &),  Vector2f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "vec2 opSub(const vec2 &in) const",    asFUNCTIONPR(vector2_sub,   (const Vector2f &,  const Vector2f &),  Vector2f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "vec2 opMul(float) const",             asFUNCTIONPR(vector2_mul,   (const Vector2f &,  const float &),     Vector2f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//          r = engine->RegisterObjectMethod("vec2", "vec2 opDiv(float) const",             asFUNCTIONPR(vector2_div,   (const Vector2f &,  const float &),     Vector2f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "vec2 opNeg() const",                  asFUNCTIONPR(vector2_neg,   (const Vector2f &),                     Vector2f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//          r = engine->RegisterObjectMethod("vec2", "vec2 opMul_r(float) const",           asFUNCTIONPR(vector2_mul_r, (const float &,     const Vector2f &),  Vector2f), asCALL_CDECL_OBJLAST ); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "float length() const", asMETHOD(Vector2f, length), asCALL_THISCALL); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec2", "float lengthSq() const", asMETHOD(Vector2f, lengthSq), asCALL_THISCALL); assert( r >= 0 );

//      r = engine->RegisterObjectProperty("vec2", "float x", asOFFSET(Vector2f, x)); assert( r >= 0 );
//      r = engine->RegisterObjectProperty("vec2", "float y", asOFFSET(Vector2f, y)); assert( r >= 0 );

        r = engine->RegisterGlobalFunction("float dot(const vec2 &in, const vec2 &in)", asFUNCTIONPR(dot, (const Vector2f &, const Vector2f &), float), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float length(const vec2 &in, const vec2 &in)", asFUNCTIONPR(length, (const Vector2f &, const Vector2f &), float), asCALL_CDECL); assert( r >= 0 );

        //-------------------------------
        // vec3
        //-------------------------------
        r = engine->RegisterObjectType("vec3", sizeof(Vector3f), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(vector3_Constructor, (void*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR(vector3_Constructor, (float, float, float, void*), void), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opAssign(const vec3 &in)",      asMETHODPR(Vector3f, operator=, (const Vector3f &), Vector3f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3 &in)",   asMETHODPR(Vector3f, operator+=,(const Vector3f &), Vector3f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opSubAssign(const vec3 &in)",   asMETHODPR(Vector3f, operator-=,(const Vector3f &), Vector3f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opMulAssign(float)",            asMETHODPR(Vector3f, operator*=,(float),            Vector3f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opDivAssign(float)",            asMETHODPR(Vector3f, operator/=,(float),            Vector3f &), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "bool opEquals(const vec3 &in) const", asFUNCTIONPR(vector3_equal, (const Vector3f &,  const Vector3f &),  bool    ), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const",    asFUNCTIONPR(vector3_add,   (const Vector3f &,  const Vector3f &),  Vector3f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const",    asFUNCTIONPR(vector3_sub,   (const Vector3f &,  const Vector3f &),  Vector3f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const",             asFUNCTIONPR(vector3_mul,   (const Vector3f &,  const float &),     Vector3f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//          r = engine->RegisterObjectMethod("vec3", "vec3 opDiv(float) const",             asFUNCTIONPR(vector3_div,   (const Vector3f &,  const float &),     Vector3f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "vec3 opNeg() const",                  asFUNCTIONPR(vector3_neg,   (const Vector3f &),                     Vector3f), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
//          r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const",           asFUNCTIONPR(vector3_mul_r, (const float &,     const Vector3f &),  Vector3f), asCALL_CDECL_OBJLAST ); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "float length() const", asMETHOD(Vector3f, length), asCALL_THISCALL); assert( r >= 0 );
//      r = engine->RegisterObjectMethod("vec3", "float lengthSq() const", asMETHOD(Vector3f, lengthSq), asCALL_THISCALL); assert( r >= 0 );

//      r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(Vector3f, x)); assert( r >= 0 );
//      r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(Vector3f, y)); assert( r >= 0 );
//      r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(Vector3f, z)); assert( r >= 0 );

        r = engine->RegisterGlobalFunction("float dot(const vec3 &in, const vec3 &in)", asFUNCTIONPR(dot, (const Vector3f &, const Vector3f &), float), asCALL_CDECL); assert( r >= 0 );
//      r = engine->RegisterGlobalFunction("vec3 cross(const vec3 &in, const vec3 &in)", asFUNCTIONPR(cross, (const Vector3f &, const Vector3f &), Vector3f), asCALL_CDECL); assert( r >= 0 );

        r = engine->RegisterGlobalFunction("float length(const vec3 &in, const vec3 &in)", asFUNCTIONPR(length, (const Vector3f &, const Vector3f &), float), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float length2d(const vec3 &in, const vec3 &in)", asFUNCTIONPR(length_2d, (const Vector3f &, const Vector3f &), float), asCALL_CDECL); assert( r >= 0 );

        //--------------------------
        // Scalar math
        //--------------------------

        // Trigonometric functions
        r = engine->RegisterGlobalFunction("float cos(float)", asFUNCTION(cosf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float sin(float)", asFUNCTION(sinf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float tan(float)", asFUNCTION(tanf), asCALL_CDECL); assert( r >= 0 );

        r = engine->RegisterGlobalFunction("float acos(float)", asFUNCTION(acosf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float asin(float)", asFUNCTION(asinf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float atan(float)", asFUNCTION(atanf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float atan2(float,float)", asFUNCTIONPR(atan2f, (float, float), float), asCALL_CDECL); assert( r >= 0 );

        // Hyberbolic functions
        r = engine->RegisterGlobalFunction("float cosh(float)", asFUNCTION(coshf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float sinh(float)", asFUNCTION(sinhf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float tanh(float)", asFUNCTION(tanhf), asCALL_CDECL); assert( r >= 0 );

        // Exponential and logarithmic functions
        r = engine->RegisterGlobalFunction("float log(float)", asFUNCTION(logf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float log10(float)", asFUNCTION(log10f), asCALL_CDECL); assert( r >= 0 );

        // Power functions
        r = engine->RegisterGlobalFunction("float pow(float, float)", asFUNCTION(powf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float sqrt(float)", asFUNCTION(sqrtf), asCALL_CDECL); assert( r >= 0 );

        // Nearest integer, absolute value, and remainder functions
        r = engine->RegisterGlobalFunction("float ceil(float)", asFUNCTION(ceilf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float abs(float)", asFUNCTION(fabsf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float floor(float)", asFUNCTION(floorf), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float fraction(float)", asFUNCTION(fraction), asCALL_CDECL); assert( r >= 0 );

        r = engine->RegisterGlobalFunction("int rand()", asFUNCTION(rand), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("int rand(int,int)", asFUNCTION(rand2), asCALL_CDECL); assert( r >= 0 );
        r = engine->RegisterGlobalFunction("float rand_per()", asFUNCTION(rand_percentage), asCALL_CDECL); assert( r >= 0 );
    }
}//namespace hgl
