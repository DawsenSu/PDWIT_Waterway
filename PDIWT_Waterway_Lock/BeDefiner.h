/*--------------------------------------------------------------------------------------+
|
|   Supplied under applicable software license agreement.
|
|   Copyright (c) 2019 Bentley Systems, Incorporated. All rights reserved.
|
+---------------------------------------------------------------------------------------*/
#pragma once
#ifndef BRIDGE_REBAR_BEDEFINER_H
#define BRIDGE_REBAR_BEDEFINER_H
//
//#define D_SCL_SECURE_NO_WARNINGS 1
//
//#include <DgnPlatform\DgnPlatformBaseType.r.h>
//#include <Bentley\WString.h>
//#include <Mstn\MdlApi\mdl.h>
//
//#include "BeBaseName.h"
//
//USING_NAMESPACE_BENTLEY_DGNPLATFORM
//
//#if !defined (USING_NAMESPACE_BENTLEY)
//#ifndef MSVERSION
//#define MSVERSION           0x8b0
//#endif
////typedef wchar_t             WChar;
//typedef __int32             ModelId;
//typedef unsigned __int32    LevelId;
//
//#endif
//
//typedef unsigned int        USTN_UINT;
//typedef int                 USTN_INT;
//typedef long                LONG_COUNT;
//
//#ifndef assert2
//#define assert2(_Expression)     ((void)0)
//#endif
//
//#define _CRT_NON_CONFORMING_SWPRINTFS
//
//// a wonderful processor to get the owner of a member data
////#define GET_OWNER_CLASS(A, DATA) ((A *)((char *)this - offsetof(A, DATA)))
////#define GET_CONST_OWNER_CLASS(A, DATA) ((const A *)((char *)this - offsetof(A, DATA)))
//
//template<typename OWNER, typename MEMBER, size_t member_offset>
//OWNER *get_owner_class(const MEMBER *this_member)
//{
//	return (OWNER *)((size_t)this_member - member_offset);
//}
//#define THE_OWNER_CLASS(A, B, MEMB) get_owner_class<A, B, offsetof(A, MEMB)>(this);
//
//template<typename OWNER, typename MEMBER, size_t member_offset>
//const OWNER *get_const_owner_class(const MEMBER *this_member)
//{
//	return (const OWNER *)((size_t)this_member - member_offset);
//}
//#define THE_CONST_OWNER_CLASS(A, B, MEMB) get_const_owner_class<A, B, offsetof(A, MEMB)>(this);

// very magical in manipulating templates
template <int v>
struct Int2Class
{
	enum
	{
		type = v
	};
};

#define BE_DEFINE_SUFFIX_TYPEDEFS(type) \
      typedef type* type##P, &type##R; \
      typedef type const* type##CP; \
      typedef type const& type##CR;

///<
#define BE_DATA_VALUE(type, var) \
public: \
        type Get##var() const \
        { \
                return m_##var; \
        } \
        type &Pop##var() \
        { \
                return m_##var; \
        } \
        void Set##var(type arg) \
        { \
                m_##var = arg; \
        } \
private: \
        type m_##var; \

#define BE_DATA_REFER(type, var) \
public: \
        const type &Get##var() const \
        { \
                return m_##var; \
        } \
        type &Pop##var() \
        { \
                return m_##var; \
        } \
        void Set##var(const type &arg) \
        { \
                m_##var = arg; \
        } \
private: \
        type m_##var; \

#define BE_PRIVATE_VALUE(type, var) \
public: \
        type Get##var() const \
        { \
                return m_##var; \
        } \
private: \
        type &Pop##var() \
        { \
                return m_##var; \
        } \
        void Set##var(type arg) \
        { \
                m_##var = arg; \
        } \
        type m_##var; \

#define BE_PRIVATE_REFER(type, var) \
public: \
        const type &Get##var() const \
        { \
                return m_##var; \
        } \
private: \
        type &Pop##var() \
        { \
                return m_##var; \
        } \
        void Set##var(const type &arg) \
        { \
                m_##var = arg; \
        } \
        type m_##var; \

#define BE_PROTECT_VALUE(type, var) \
public: \
    const type Get##var() const \
{ \
    return m_##var; \
} \
protected: \
    type &Pop##var() \
{ \
    return m_##var; \
} \
    void Set##var(type arg) \
{ \
    m_##var = arg; \
} \
private: \
    type m_##var; \

#define BE_PROTECT_REFER(type, var) \
public: \
        const type &Get##var() const \
        { \
                return m_##var; \
        } \
protected: \
        type &Pop##var() \
        { \
                return m_##var; \
        } \
        void Set##var(const type &arg) \
        { \
                m_##var = arg; \
        } \
        type m_##var; \

#define BE_DATA_ARRAY(type, var, sz) \
public: \
        const type &Get##var(intptr_t index) const \
        { \
        index %= sz; \
                return m_##var[ index ]; \
        } \
    const type *Get##var() const \
    { \
        return m_##var; \
    } \
        type &Pop##var(intptr_t index) \
        { \
        index %= sz; \
                return m_##var[ index ]; \
        } \
    type *Pop##var() \
    { \
        return m_##var; \
    } \
        void Set##var(intptr_t index, const type &arg) \
        { \
        index %= sz; \
                m_##var[ index ] = arg; \
        } \
private: \
        type m_##var[ sz ]; \

#define DECLARE_PROPERTY(type, var) \
public: \
	type Get##var() const; \
	type &Pop##var(); \
	void Set##var(type arg); \
private: \
	type m_##var;

#define DEFINE_PROPERTY(ClassName, type, var) \
		type ClassName##::Get##var() const \
	{ \
		return m_##var; \
	} \
	type & ClassName##::Pop##var() \
	{ \
		return m_##var; \
	} \
	void ClassName##::Set##var(type arg)\
	{ \
		m_##var = arg; \
	} \

///>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compressing types
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//template <typename T, typename TB = BYTE>
//struct BeTypeUnion
//{
//    union
//    {
//        T       v;
//        TB      b[ ( sizeof( T ) - 1 ) / sizeof( TB ) + 1 ];
//    };
//
//    // keep this only as one argument default CTOR, otherwise compiler will get confused
//	BeTypeUnion( T val = 0 )
//	{
//        InitArray(b);
//        v = val;
//	}
//
//    BeTypeUnion( TB val, int index )
//        : v (0)
//    {
//        InitArray(b, false);
//        index %= Size();
//        b[ index ] = val;
//    }
//
//	int Size() const
//	{
//		return sizeof( b ) / sizeof( *b );
//	}
//
//	int Count() const
//	{
//		register int i, k;
//		register int maxnum = Size();
//		for( i = 0, k = 0; i < maxnum; i++ )
//		{
//			if ( b[ i ] != 0 )
//				k++;
//		}
//		return k;
//	}
//
//    BeTypeUnion &operator = (T vv)
//    {
//        InitArray(b, false);
//        v = vv;
//        return *this;
//    }
//
//    operator const T &() const
//    {
//        return v;
//    }
//    operator T &()
//    {
//        return v;
//    }
//
//    const TB &operator *() const
//    {
//        return *b;
//    }
//    TB &operator *()
//    {
//        return *b;
//    }
//
//    bool operator == (T vv) const
//    {
//        return v == vv;
//    }
//    bool operator == (TB bb) const
//    {
//        return *b == bb;
//    }
//};
//
//typedef BeTypeUnion< short > BeShortUnion;
//typedef BeTypeUnion< long > BeLongUnion;
//typedef BeTypeUnion< ElementId > BeElemIDUnion;
//typedef BeTypeUnion< double > BeDoubleUnion;
//typedef BeTypeUnion< ElementId, short > BeHandleUnion;

///////////////////////////////////////////////////////////////////////////////////////////////
// Dtor Ctor Init and Copy
//////////////////////////////////////////////////////////////////////////////////////////////
//template<typename TYPE>
//__forceinline void DtorClass(TYPE &ref)
//{
//    ref.~TYPE();
//}
//
//template<typename TYPE>
//__forceinline void DtorArray(TYPE &ref, intptr_t num = 1)
//{
//    register intptr_t i;
//    for (i = 0; i < num; i++)
//    {
//        DtorClass(*(ref + i));
//    }
//}
//
//template<typename TYPE>
//__forceinline void CtorClass(TYPE &ref)
//{
//#pragma push_macro("new")
//#undef new
//    ::new((void*)(&ref)) TYPE;
//#pragma pop_macro("new")
//}
//
//template<typename TYPE>
//__forceinline void DoInitClass(TYPE &ref, Int2Class< 1 >, bool todelete = true) // do one class only
//{
//    // do NOT destruct, if it may have not been properly constructed yet!!! -
//    // * BUT MOST OFTEN WE MUST DTOR before we memset to 0. simply Do not use this at the CTOR of ref and if you do use todelete = false
//    if (todelete)
//        DtorClass(ref);
//
//    memset(&ref, 0, sizeof(ref));
//    CtorClass(ref);
//}
//
//template<typename TYPE>
//__forceinline void DoInitClass(TYPE &ref, Int2Class< 0 >)       // do one class only
//{
//    char stop_compile[0];   // GUARD against miniature classes, You may be trying to InitClass a pointer which is not allowed
//    *stop_compile = 0;
//}
//
//template<typename TYPE>
//__forceinline void InitArray(TYPE &ref, bool todelete = true)
//{
//    register INT i;
//    register INT num = _countof(ref);
//
//    // see comment above for why we use DTOR
//    if (todelete)
//    {
//        DtorArray(ref, num);
//    }
//
//    memset((TYPE*)ref, 0, sizeof(ref));
//
//    for (i = 0; i < num; i++)
//    {
//		CtorClass(*(ref + i));
//    }
//}
//
//template<typename TYPE>
//__forceinline void InitArray(TYPE *ref, intptr_t num, bool todelete = true)
//{
//    register intptr_t i;
//
//    // see comment above for why we use DTOR
//    if (todelete)
//    {
//        DtorArray(ref, num);
//    }
//
//    memset(ref, 0, sizeof(*ref) * num);
//
//    for (i = 0; i < num; i++)
//    {
//        CtorClass(*(ref + i));
//    }
//}
//
//template<typename TYPE>
//__forceinline void InitClass(TYPE &ref, bool todelete = true)   // do one class only
//{
//    const int v = sizeof(ref) > sizeof(&ref) ? 1 : 0;
//    DoInitClass(ref, Int2Class< v >(), todelete);
//}
//
//template<typename DEST, typename SRC>
//__forceinline void CopyArray(DEST &dest, const SRC &src, intptr_t sz)
//{
//    register intptr_t i;
//    register intptr_t d = _countof(dest);
//    InitArray(dest);
//    for (i = 0; i < d && i < sz; i++)
//    {
//        dest[i] = src[i];               // we must use the operator = for C++ class assignment
//    }
//}
//
//template<typename DEST, typename SRC>
//__forceinline void CopyArray(DEST &dest, const SRC &src)
//{
//    register intptr_t sz = _countof(src);
//    CopyArray(dest, src, sz);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// have a pure operation class of another baseclass
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// see BE_DECLARE_SUBNAME in basename.h
//#define BE_IMPLEMENT_CLASS_ON(BASE, CLASS) \
//    public: \
//    CLASS( const BASE &base ) \
//    : BASE( base ) \
//    { \
//    } \
//    \
//    CLASS &operator = ( const BASE &base ) \
//    { \
//        ( BASE & )*this = base; \
//        return *this; \
//    } \
//    private: \
//end

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert this class to a different kind but compatible - Offset should be the same so arrays are compatible
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define BE_CONVERT_TEMPLATE( FromType ) \
//    template < typename TOTYPE > \
//    FromType &CopyFrom (const TOTYPE &totype) \
//    { \
//        /* do NOT modify or delete the line of testsize, fix your code, you may be casting not the same size pointers */ \
//        char testsize[sizeof(TOTYPE) == sizeof(*this)] = ""; \
//        return operator = ((const FromType &)totype);\
//    } \
//    template < typename TOTYPE > \
//    const TOTYPE &To() const \
//    { \
//        /* do NOT modify or delete the line of testsize, fix your code, you may be casting not the same size pointers*/ \
//        char testsize[ sizeof(TOTYPE) == sizeof(*this) ] = ""; \
//        return ( const TOTYPE & )(*this); \
//    } \
//    template < typename TOTYPE > \
//    TOTYPE &To() \
//    { \
//        /* do NOT modify or delete the line of testsize, fix your code, you may be casting not the same size pointers*/ \
//        char testsize[ sizeof(TOTYPE) == sizeof(*this) ] = ""; \
//        return ( TOTYPE & )(*this); \
//    } \
//    template < typename TOTYPE > \
//    operator const TOTYPE *() const \
//    { \
//        /* do NOT modify or delete the line of testsize, fix your code, you may be casting not the same size pointers*/ \
//        char testsize[ sizeof(TOTYPE) == sizeof(*this) ] = ""; \
//        return ( const TOTYPE * )(this); \
//    } \
//    template < typename TOTYPE > \
//    operator TOTYPE *() \
//    { \
//        /* do NOT modify or delete the line of testsize, fix your code, you may be casting not the same size pointers*/ \
//        char testsize[ sizeof(TOTYPE) == sizeof(*this) ] = ""; \
//        return ( TOTYPE * )(this); \
//    } \
////end
//
//template <typename T>
//int CompareIds(T this_id, T that_id)
//{
//    return this_id < that_id ? -1 : this_id > that_id ? 1 : 0;
//}
//
//#ifdef _DEBUG
//// IMPORTANT NOTE: MUST be surrounded by curly brackets e.g if(what) { Mprintf(_T("bla")); }
//#ifdef UNICODE
//#define Mprintf SS->Printf(_T("\n%S(%d): "), __FILE__, __LINE__); SS->Printf
//#else
//#define Mprintf SS->Printf(_T("\n%s(%d): "), __FILE__, __LINE__); SS->Printf
//#endif
//#else
//#define Mprintf SS->Printf
//#endif
//
///*
//// must override this if you do not want to use memcheck
//#define DEBUG_REBAR_NEW new
//*/
//#define DEBUG_REBAR_THIS_FILE static const char *REBAR_THIS_FILE = __FILE__;

#endif
