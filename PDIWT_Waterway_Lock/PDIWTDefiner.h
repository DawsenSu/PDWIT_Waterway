#pragma once

//! Namespace TypeDefiners

// PDIWT namespace 
#define PDIWT_NAMESPACE_BEGIN namespace PDIWT {
#define PDIWT_NAMESPACE_END	}
#define USING_NAMESPACE_PDIWT using namespace PDIWT;

// PDIWT::Waterway namespace
#define PDIWT_WATERWAY_NAMESPACE_BEGIN	namespace PDIWT { namespace Waterway {
#define PDIWT_WATERWAY_NAMESPACE_END }}
#define	USING_NAMESPACE_PDIWT_WATERWAY using namespace PDIWT::Waterway;

// PDIWT::Waterway::Lock namespace
#define PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN	namespace PDIWT { namespace Waterway { namespace Lock {
#define PDIWT_WATERWAY_LOCK_NAMESPACE_END }}}
#define	USING_NAMESPACE_PDIWT_WATERWAY_LOCK using namespace PDIWT::Waterway::Lock;

//! class/struct forward declaration

// -GLOBAL_TYPDEF
#define GLOABL_TYPEDEF(_name_,structunion) \
	structunion _name_; \
    typedef structunion _name_*          _name_##P, &_name_##R;  \
    typedef structunion _name_ const*    _name_##CP; \
    typedef structunion _name_ const&    _name_##CR; \

#define GLOBAL_STRUCTDEF(_name_) GLOABL_TYPEDEF(_name_, struct);
#define GLOBAL_CLASSDEF(_name_) GLOABL_TYPEDEF(_name_, class);

// -PDIWT_TYPDEF
#define PDIWT_TYPEDEF(_name_,structunion) \
    PDIWT_NAMESPACE_BEGIN \
	structunion _name_; \
    typedef structunion _name_*          _name_##P, &_name_##R;  \
    typedef structunion _name_ const*    _name_##CP; \
    typedef structunion _name_ const&    _name_##CR; \
	PDIWT_NAMESPACE_END

#define PDIWT_STRUCTDEF(_name_) PDIWT_TYPEDEF(_name_, struct);
#define PDIWT_CLASSDEF(_name_) PDIWT_TYPEDEF(_name_, class);

// -PDIWT_TYPDEF
#define PDIWT_WATERWAY_TYPEDEF(_name_,structunion) \
    PDIWT_WATERWAY_NAMESPACE_BEGIN \
	structunion _name_; \
    typedef structunion _name_*          _name_##P, &_name_##R;  \
    typedef structunion _name_ const*    _name_##CP; \
    typedef structunion _name_ const&    _name_##CR; \
	PDIWT_WATERWAY_NAMESPACE_END

#define PDIWT_WATERWAY_STRUCTDEF(_name_) PDIWT_WATERWAY_TYPEDEF(_name_, struct);
#define PDIWT_WATERWAY_CLASSDEF(_name_) PDIWT_WATERWAY_TYPEDEF(_name_, class);

// -PDIWT_TYPDEF
#define PDIWT_WATERWAY_LOCK_TYPEDEF(_name_,structunion) \
    PDIWT_WATERWAY_LOCK_NAMESPACE_BEGIN \
	structunion _name_; \
    typedef structunion _name_*          _name_##P, &_name_##R;  \
    typedef structunion _name_ const*    _name_##CP; \
    typedef structunion _name_ const&    _name_##CR; \
	PDIWT_WATERWAY_LOCK_NAMESPACE_END

#define PDIWT_WATERWAY_LOCK_STRUCTDEF(_name_) PDIWT_WATERWAY_LOCK_TYPEDEF(_name_, struct);
#define PDIWT_WATERWAY_LOCK_CLASSDEF(_name_) PDIWT_WATERWAY_LOCK_TYPEDEF(_name_, class);

#ifndef Public
#define Public
#endif