/*
 *
 * Copyright (c) 1998-9
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */
 
 /*
  *	FILE     jm_cfg.h
  *	VERSION  2.00
  */

#ifndef JM_CFG_H
#define JM_CFG_H

/************************************************************************

The purpose of this header is to provide compiler and STL configuration
options.  Options fall into three categaries (namespaces, compiler and STL),
throughout, the defaults assume that the compiler and STL are fully CD2
compliant, features that are not supported on your system may be selectively
turned off by defining the appropriate macros.  Borland C++, Borland C++ Builder,
and Microsoft Visual C++ should be auto-recognised and configured.
The SGI, HP, Microsoft and Rogue Wave STL's should be auto-recognised and configured.
Do not change this file unless you really really have to, add options to
<jm_opt.h> instead.

Namespace options:
~~~~~~~~~~~~~~~~~

JM_NO_NAMESPACES           Define if your compiler does not support namespaces
__JM                       Defines the namespace used for this library,
                           defaults to "jm", but can be changed by defining
                           __JM on the command line.
__JM_STD                  Defines the namespace used by the underlying STL
                          (if any), defaults to "std", can be changed by
                          defining __JM_STD on the command line.
__JM_STDC                 Defines the namespace used by the C Library defs.
                          Defaults to "std" as recomended by the latest
                          draft standard, can be redefined by defining
                          __JM_STDC on the command line.

Compiler options:
~~~~~~~~~~~~~~~~

JM_NO_EXEPTIONS           Disables exception handling support.
JM_NO_MUTABLE             Disables use of mutable keyword.
JM_NO_DEFAULT_PARAM       If templates can not have default parameters.
JM_NO_TRICKY_DEFAULT_PARAM If templates can not have derived default parameters.
JM_PLATFORM_WINDOWS       Platform is MS Windows.
JM_PLATFORM_DOS           Platform if MSDOS.
JM_NO_BOOL                If bool is not a distict type.
JM_NO_WCSTRING            If there are no wcslen and wcsncmp functions available.
JM_NO_MEMBER_TEMPLATES    If member function templates or nested template classes are not allowed.
JM_NO_TEMPLATE_RETURNS    If template functions based on return type are not supported.
JM_NO_INT64               If 64bit integers are not supported.
JM_INT64t                 The type of a 64-bit signed integer if available.
JM_IMM64(val)             Declares a 64-bit immediate value by appending any
                          necessary suffix to val.
JM_NO_CAT                 Define if the compiler does not support POSIX style
                          message categories (catopen catgets catclose).
JM_THREADS                Define if the compiler supports multiple threads in
                          the current translation mode.
JM_TEMPLATE_SPECIALISE    Defaults to template<> , ie the template specialisation
                          prefix, can be redefined to nothing for older compilers.
JM_NESTED_TEMPLATE_DECL   Defaults to template, the standard prefix when accessing
                          nested template classes, can be redefined to nothing if
                          the compiler does not support this.

STL options:
~~~~~~~~~~~

JM_NO_EXCEPTION_H         Define if you do not a compliant <exception>
                          header file.
JM_NO_ITERATOR_H          Define if you do not have a version of <iterator>.
JM_NO_MEMORY_H            Define if <memory> does not fully comply with the
                          latest standard, and is not auto-recognised,
                          that means nested template classes
                          which hardly any compilers support at present.
JM_NO_LOCALE_H            Define if there is no verion of the standard
                          <locale> header available.
JM_NO_STL                 Disables the use of any supporting STL code.
JM_NO_NOT_EQUAL           Disables the generation of operator!= if this
                          clashes with the STL version.

************************************************************************/

#include <jm/jm_opt.h>
#include <stdlib.h>
#include <stddef.h>

/* this will increase in future versions: */
#define JM_VERSION 200

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
   #define JM_PLATFORM_W32
#endif

#ifdef __BORLANDC__
   #define RE_CALL __fastcall
   #define RE_CCALL __stdcall

   #if __BORLANDC__ < 0x500
      #define JM_NO_NAMESPACES
      #define JM_NO_BOOL
      #define JM_NO_MUTABLE
   #endif

   #if __BORLANDC__ < 0x520
      #define JM_NO_WCSTRING
      #define JM_NO_INT64
      // Early versions of Borlands namespace code can't cope with iterators
      // that are in different namespaces from STL code.
      #define __JM std
      #define JM_NO_NOT_EQUAL
   #endif

   #if __BORLANDC__ < 0x530
      #define OLD_IOSTREAM
      #define __JM_STDC
      #define JM_NO_TRICKY_DEFAULT_PARAM
      #define JM_NO_EXCEPTION_H
      #ifndef __WIN32__
         #define JM_NO_WCSTRING
      #endif
      #define JM_NO_LOCALE_H
      #define JM_NO_TEMPLATE_RETURNS
      #define JM_TEMPLATE_SPECIALISE
   #endif

   #if __BORLANDC__ < 0x540
      #define JM_NO_MEMBER_TEMPLATES
      // inline contructors exhibit strange behaviour
      // under Builder 3 and C++ 5.x when throwing exceptions
      #define INLINE_EXCEPTION_BUG
      #define JM_NESTED_TEMPLATE_DECL
   #endif

   #ifndef _CPPUNWIND
      #define JM_NO_EXEPTIONS
   #endif

   #ifdef _Windows
      #define JM_PLATFORM_WINDOWS
   #else
      #define JM_PLATFORM_DOS
   #endif

   #define JM_INT64t __int64
   #define JM_IMM64(val) val##i64
   #define JM_NO_CAT

   #ifdef __MT__
      #define JM_THREADS
   #endif

   //
   // import export options:
   #ifdef _RTLDLL
      #ifdef RE_BUILD_DLL
         #define JM_IX_DECL __declspec( dllexport )
      #else
         #define JM_IX_DECL __declspec( dllimport ) 
      #endif
   #endif
   #include <jm/re_lib.h>
#endif

#ifdef _MSC_VER
   #define RE_CALL __fastcall
   #define RE_CCALL __stdcall

   #if _MSC_VER < 1100
      #define JM_NO_NAMESPACES
      #define JM_NO_DEFAULT_PARAM
      #define JM_NO_BOOL
      #define JM_NO_MUTABLE
      #define JM_NO_WCSTRING
      #define JM_NO_LOCALE_H
      #define JM_NO_TEMPLATE_RETURNS
      #define JM_NO_INT64
   #endif

   #if _MSC_VER < 1200
      #define JM_TEMPLATE_SPECIALISE
      #define JM_NESTED_TEMPLATE_DECL
   #endif

   #ifndef _CPPUNWIND
      #define JM_NO_EXEPTIONS
   #endif

   #define __JM_STDC
   #define JM_PLATFORM_WINDOWS
   //
   // no support for nested template classes yet....
   // although this part of VC6 is badly documented
   #define JM_NO_MEMBER_TEMPLATES
   #define JM_INT64t __int64
   #define JM_IMM64(val) val##i64
   #define JM_NO_CAT

   #ifdef _MT
      #define JM_THREADS
   #endif

#pragma warning(disable: 4786)
#pragma warning(disable: 4800)
#pragma warning(disable: 4200)

   //
   // import export options:
   #ifdef _DLL
      #ifdef RE_BUILD_DLL
         #define JM_IX_DECL __declspec( dllexport )
      #else
         #define JM_IX_DECL __declspec( dllimport ) 
      #endif
   #endif
   #include <jm/re_lib.h>
#endif

#ifdef __GNUC__
   #if (__GNUC__ < 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ < 91))
      #define JM_NO_NAMESPACES
      #define JM_NO_MUTABLE
      #define JM_NO_MEMBER_TEMPLATES
   #endif
   #ifndef __STL_USE_NAMESPACES
      #define JM_NO_EXCEPTION_H
   #endif
   #define JM_INT64t long long
   #define JM_IMM64(val) val##LL

   #ifdef _WIN32
      #define JM_PLATFORM_WINDOWS
   #endif
   #define JM_NO_CAT
   #define OLD_IOSTREAM
   #define JM_NESTED_TEMPLATE_DECL

#endif

#if !defined(JM_INT64t) || !defined(JM_IMM64)
#define JM_NO_INT64
#endif

#ifndef JM_TEMPLATE_SPECIALISE
#define JM_TEMPLATE_SPECIALISE template <>
#endif

#ifndef JM_NESTED_TEMPLATE_DECL
#define JM_NESTED_TEMPLATE_DECL template
#endif

#ifndef JM_IX_DECL
#define JM_IX_DECL
#endif

#ifndef MB_CUR_MAX
// yuk!
// better make a conservative guess!
#define MB_CUR_MAX 10
#endif


/* everything else is C++: */

#ifdef __cplusplus

/* define macro's to make default parameter declaration easier: */

#ifdef JM_NO_DEFAULT_PARAM
   #define JM_DEFAULT_PARAM(x)
   #define JM_TRICKY_DEFAULT_PARAM(x)
#elif defined(JM_NO_TRICKY_DEFAULT_PARAM)
   #define JM_DEFAULT_PARAM(x) = x
   #define JM_TRICKY_DEFAULT_PARAM(x)
#else
   #define JM_DEFAULT_PARAM(x) = x
   #define JM_TRICKY_DEFAULT_PARAM(x) = x
#endif

/* STL configuration goes here: */

#ifdef JM_NO_STL
   #define JM_NO_EXCEPTION_H
   #define JM_NO_ITERATOR_H
   #define JM_NO_MEMORY_H
   #define JM_NO_LOCALE_H
#endif

#ifndef JM_NO_EXCEPTION_H
   #include <exception>
#endif

#ifndef JM_NO_ITERATOR_H
   #include <iterator>

   #if defined(__SGI_STL_INTERNAL_ITERATOR_H) || defined(__SGI_STL_ITERATOR_H)
      #define JM_NO_LOCALE_H
      #define OLD_IOSTREAM

      /* we are using SGI's STL
       some of these (__JM_STDC)
       may be guesswork: */
      #if !defined(__STL_MEMBER_TEMPLATE_CLASSES) ||  !defined(__STL_MEMBER_TEMPLATES)
         #define JM_NO_MEMBER_TEMPLATES
      #endif

      #if !defined( __JM_STD)
         #if defined (__STL_USE_NAMESPACES)
            #define __JM_STD __STD
         #else
            #define __JM_STD
         #endif
      #endif
      #ifndef __JM_STDC
         #define __JM_STDC
      #endif
      #ifdef __STL_NO_BOOL
         #define JM_NO_BOOL
      #endif
      #ifdef __STL_LIMITED_DEFAULT_TEMPLATES
         #define JM_NO_TRICKY_DEFAULT_PARAM
      #endif
      #ifndef __STL_USE_EXCEPTIONS
         #define JM_NO_EXCEPTIONS
      #endif

      #include <algo.h>
      #include <alloc.h>

      #define JM_ALGO_INCLUDED

      #define JM_DISTANCE(i, j, n) __JM_STD::distance(i, j, n)
      #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::output_iterator
      #define JM_INPUT_ITERATOR(T, D) __JM_STD::input_iterator<T, D>
      #define JM_FWD_ITERATOR(T, D) __JM_STD::forward_iterator<T, D>
      #define JM_BIDI_ITERATOR(T, D) __JM_STD::bidirectional_iterator<T, D>
      #define JM_RA_ITERATOR(T, D) __JM_STD::random_access_iterator<T, D>

      #ifdef __STL_USE_STD_ALLOCATORS

         /* new style allocator's with nested template classes */

         #define REBIND_INSTANCE(x, y, inst) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other(inst)
         #define REBIND_TYPE(x, y) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other
         #define JM_DEF_ALLOC_PARAM(x) JM_TRICKY_DEFAULT_PARAM( __JM_STD::allocator<x> )
         #define JM_DEF_ALLOC(x) __JM_STD::allocator<x>

      #else  /* __STL_USE_STD_ALLOCATORS */

         /* old style byte allocator's, no nested templates */
         #define JM_OLD_ALLOCATORS
         #define REBIND_INSTANCE(x, y, inst) re_alloc_binder<x, y>(inst)
         #define REBIND_TYPE(x, y) re_alloc_binder<x, y>
         #define JM_DEF_ALLOC_PARAM(x) JM_DEFAULT_PARAM( __JM_STD::alloc )
         #define JM_DEF_ALLOC(x) __JM_STD::alloc 
         #define JM_NEED_BINDER

      #endif /* __STL_USE_STD_ALLOCATORS */

      #define JM_STL_DONE
      #define JM_NO_NOT_EQUAL

   #elif defined(__STD_ITERATOR__)

      /* Rogue Wave STL */

      #if defined(RWSTD_NO_MEMBER_TEMPLATES) || defined(RWSTD_NO_MEM_CLASS_TEMPLATES)
         #define JM_NO_MEMBER_TEMPLATES
      #endif
      #ifdef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
         #define JM_NO_TEMPLATE_RETURNS
      #endif

      #ifdef _RWSTD_NO_NAMESPACE
         #define __JM_STD
         #define __JM_STDC
      #else
         #define __JM_STD std
      #endif

      #ifdef RWSTD_NO_EXCEPTIONS
         #define JM_NO_EXCEPTIONS
      #endif

      #ifdef RWSTD_NO_MUTABLE
         #define JM_NO_MUTABLE
      #endif

      #ifdef RWSTD_NO_DEFAULT_TEMPLATES
         #define JM_NO_DEFAULT_PARAM
         #define JM_NO_TRICKY_DEFAULT_PARAM
      #endif

      #ifdef RWSTD_NO_BOOL
         #define JM_NO_BOOL
      #endif

      #if _RWSTD_VER > 0x020000
         #define JM_DISTANCE(i, j, n) (n = __JM_STD::distance(i, j))
         #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::output_iterator_tag, T, D>
         #define JM_INPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::input_iterator_tag, T, D>
         #define JM_FWD_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::forward_iterator_tag, T, D>
         #define JM_BIDI_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::bidirectional_iterator_tag, T, D>
         #define JM_RA_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::random_access_iterator_tag, T, D>
      #else 
         #define JM_DISTANCE(i, j, n) __JM_STD::distance(i, j, n)
         #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::output_iterator
         #if _RWSTD_VER >= 0x0200
            #define JM_INPUT_ITERATOR(T, D) __JM_STD::input_iterator<T>
         #else
            #define JM_INPUT_ITERATOR(T, D) __JM_STD::input_iterator<T, D>
         #endif
         #define JM_FWD_ITERATOR(T, D) __JM_STD::forward_iterator<T, D>
         #define JM_BIDI_ITERATOR(T, D) __JM_STD::bidirectional_iterator<T, D>
         #define JM_RA_ITERATOR(T, D) __JM_STD::random_access_iterator<T, D>
      #endif

      #include <memory>

      #ifdef _RWSTD_ALLOCATOR

         /* new style allocator */

         #define REBIND_INSTANCE(x, y, inst) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other(inst)
         #define REBIND_TYPE(x, y) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other
         #define JM_DEF_ALLOC_PARAM(x) JM_TRICKY_DEFAULT_PARAM( __JM_STD::allocator<x> )
         #define JM_DEF_ALLOC(x)  __JM_STD::allocator<x>

      #else
         /*
         // old style allocator
         // this varies a great deal between versions, and there is no way
         // that I can tell of differentiating between them, so use our
         // own default allocator...
         */
         #define JM_OLD_ALLOCATORS
         #define REBIND_INSTANCE(x, y, inst) re_alloc_binder<x, y>(inst)
         #define REBIND_TYPE(x, y) re_alloc_binder<x, y>
         #define JM_DEF_ALLOC_PARAM(x) JM_DEFAULT_PARAM( jm_def_alloc )
         #define JM_DEF_ALLOC(x) jm_def_alloc

         #define JM_NEED_BINDER
         #define JM_NEED_ALLOC

      #endif

      #define JM_STL_DONE

   #elif defined (ITERATOR_H)

      /* HP STL */

      #define __JM_STD
      #define __JM_STDC
      #define JM_NO_LOCALE_H

      #include <algo.h>
      #define JM_ALGO_INCLUDED

      #define JM_DISTANCE(i, j, n) __JM_STD::distance(i, j, n)
      #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::output_iterator
      #define JM_INPUT_ITERATOR(T, D) __JM_STD::input_iterator<T, D>
      #define JM_FWD_ITERATOR(T, D) __JM_STD::forward_iterator<T, D>
      #define JM_BIDI_ITERATOR(T, D) __JM_STD::bidirectional_iterator<T, D>
      #define JM_RA_ITERATOR(T, D) __JM_STD::random_access_iterator<T, D>

      /* old style allocator */
      #define JM_OLD_ALLOCATORS
      #define REBIND_INSTANCE(x, y, inst) re_alloc_binder<x, y>(inst)
      #define REBIND_TYPE(x, y) re_alloc_binder<x, y>
      #define JM_DEF_ALLOC_PARAM(x) JM_DEFAULT_PARAM( jm_def_alloc )
      #define JM_DEF_ALLOC(x) jm_def_alloc

      #define JM_NEED_BINDER
      #define JM_NEED_ALLOC
      #define JM_NO_NOT_EQUAL

      #define JM_STL_DONE

   #elif defined (_MSC_VER)

      /* assume we're using MS's own STL (VC++ 5/6) */
      #define __JM_STD std
      #define __JM_STDC

      #define JM_DISTANCE(i, j, n) n = __JM_STD::distance(i, j)
      #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::output_iterator_tag, T, D>
      #define JM_INPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::input_iterator_tag, T, D>
      #define JM_FWD_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::forward_iterator_tag, T, D>
      #define JM_BIDI_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::bidirectional_iterator_tag, T, D>
      #define JM_RA_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::random_access_iterator_tag, T, D>

      /* MS's allocators are rather ambiguous about their properties
      at least as far as MSDN is concerned, so play safe: */
      #define JM_OLD_ALLOCATORS
      #define REBIND_INSTANCE(x, y, inst) re_alloc_binder<x, y>(inst)
      #define REBIND_TYPE(x, y) re_alloc_binder<x, y>
      #define JM_DEF_ALLOC_PARAM(x) JM_DEFAULT_PARAM( jm_def_alloc )
      #define JM_DEF_ALLOC(x) jm_def_alloc

      #define JM_NEED_BINDER
      #define JM_NEED_ALLOC

      #define JM_STL_DONE

      #define JM_USE_FACET(l, type) __JM_STD::use_facet(l, (type*)0, true)
      #define JM_HAS_FACET(l, type) __JM_STD::has_facet(l, (type*)0)



   #else

      /* unknown STL version
       try the defaults: */

      #define JM_DISTANCE(i, j, n) __JM_STD::distance(i, j, n)
      /* these may be suspect for older libraries */
      #define JM_OUTPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::output_iterator_tag, T, D>
      #define JM_INPUT_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::input_iterator_tag, T, D>
      #define JM_FWD_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::forward_iterator_tag, T, D>
      #define JM_BIDI_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::bidirectional_iterator_tag, T, D>
      #define JM_RA_ITERATOR(T, D) __JM_STD::iterator<__JM_STD::random_access_iterator_tag, T, D>

   #endif  /* <iterator> config */

#else   /* no <iterator> at all */

   #define JM_DISTANCE(i, j, n) (n = j - i)
   #define JM_OUTPUT_ITERATOR(T, D) dummy_iterator_base
   #define JM_INPUT_ITERATOR(T, D) dummy_iterator_base
   #define JM_FWD_ITERATOR(T, D) dummy_iterator_base
   #define JM_BIDI_ITERATOR(T, D) dummy_iterator_base
   #define JM_RA_ITERATOR(T, D) dummy_iterator_base


#endif

/* now do allocator if not already done */

#ifndef JM_STL_DONE

   #ifdef JM_NO_MEMORY_H

      /* old style allocator */
      
      #define JM_OLD_ALLOCATORS

      #define REBIND_INSTANCE(x, y, inst) re_alloc_binder<x, y>(inst)
      #define REBIND_TYPE(x, y) re_alloc_binder<x, y>
      #define JM_DEF_ALLOC_PARAM(x) JM_DEFAULT_PARAM( jm_def_alloc )
      #define JM_DEF_ALLOC(x) jm_def_alloc

      #define JM_NEED_BINDER
      #define JM_NEED_ALLOC

   #else

      /* new style allocator's with nested template classes */

      #define REBIND_INSTANCE(x, y, inst) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other(inst)
      #define REBIND_TYPE(x, y) y::JM_NESTED_TEMPLATE_DECL rebind<x>::other
      #define JM_DEF_ALLOC_PARAM(x) JM_TRICKY_DEFAULT_PARAM( __JM_STD::allocator<x> )
      #define JM_DEF_ALLOC(x) __JM_STD::allocator<x>

   #endif

#endif



/* namespace configuration goes here: */
#ifdef JM_NO_NAMESPACES

   #ifdef __JM_STD
      #undef __JM_STD
   #endif

   #ifdef __JM_STDC
      #undef __JM_STDC
   #endif

   #ifdef __JM
      #undef __JM
   #endif

   #define __JM
   #define __JM_STD
   #define __JM_STDC
   #define JM_NAMESPACE(x)
   #define JM_END_NAMESPACE
   #define JM_USING(x)

#else

   #ifndef __JM_STD
      #define __JM_STD std
   #endif

   #ifndef __JM_STDC
      #define __JM_STDC std
   #endif

   #ifndef __JM
      #define __JM jm
   #endif

   #define JM_NAMESPACE(x) namespace x{
   #define JM_END_NAMESPACE };
   #define JM_USING(x) using namespace x;

#endif

/* locale configuration goes here */
#if !defined(JM_NO_LOCALE_H) && defined(RE_LOCALE_CPP)
    #include <locale>
    #define LOCALE_INSTANCE(i) __JM_STD::locale i;
    #define MAYBE_PASS_LOCALE(i) , i
    #ifndef JM_NO_TEMPLATE_RETURNS
      #ifndef JM_USE_FACET
         #define JM_USE_FACET(l, type) __JM_STD::use_facet< type >(l)
      #endif
      #ifndef JM_HAS_FACET
         #define JM_HAS_FACET(l, type) __JM_STD::has_facet< type >(l)
      #endif
    #else
      #ifndef JM_USE_FACET
         #define JM_USE_FACET(l, type) __JM_STD::use_facet(l, (type*)0)
      #endif
      #ifndef JM_HAS_FACET
         #define JM_HAS_FACET(l, type) __JM_STD::has_facet(l, (type*)0)
      #endif
    #endif
#else
    #define LOCALE_INSTANCE(i)
    #define MAYBE_PASS_LOCALE(i)
#endif

/* compiler configuration goes here: */

#ifdef JM_NO_MUTABLE
   #define JM_MUTABLE
#else
   #define JM_MUTABLE mutable
#endif

#if defined( JM_NO_BOOL) && !defined(bool)
   #define bool int
   #define true 1
   #define false 0
#endif

#ifndef RE_CALL
#define RE_CALL
#endif

#ifndef RE_CCALL
#define RE_CCALL
#endif

#ifndef RE_DECL
#define RE_DECL
#endif


/* add our class def's if they are needed: */

JM_NAMESPACE(__JM)

// add our destroy functions:

template <class T>
inline void RE_CALL jm_destroy(T* t)
{
   t->~T();
}

template <class T>
inline void RE_CALL jm_construct(void* p, const T& t)
{
   new (p) T(t);
}


template<class T, class Allocator>
class re_alloc_binder 
{
   Allocator inst;
public:
   typedef T         value_type;
   typedef T*        pointer;
   typedef const T*  const_pointer;
   typedef T&        reference;
   typedef const T&  const_reference;
   typedef size_t    size_type;
   typedef __JM_STDC::ptrdiff_t difference_type;

   re_alloc_binder(const Allocator& i);
   re_alloc_binder(const re_alloc_binder& o) : inst(o.inst) {}

   T* RE_CALL allocate(size_t n)
             { return 0 == n ? 0 : (T*) inst.allocate(n * sizeof(T)); }
   void RE_CALL deallocate(T *p, size_t n)
             { if (0 != n) inst.deallocate((char*)p, n * sizeof (T)); }

   pointer RE_CALL address(reference x) const { return &x; }
   const_pointer RE_CALL address(const_reference x) const { return &x; }
   static size_type RE_CALL max_size() { return -1; }
   static void RE_CALL construct(pointer p, const T& val) { jm_construct(p, val); }
   void RE_CALL destroy(pointer p) { jm_destroy(p); }

   operator const Allocator& () const { return inst; }

#ifndef JM_NO_MEMBER_TEMPLATES
   const Allocator& RE_CALL instance() { return inst; }

   template <class U>
   struct rebind
   {
      typedef re_alloc_binder<U, Allocator> other;
   };
   
   template <class U>
   RE_CALL re_alloc_binder(const re_alloc_binder<U, Allocator>& o) throw()
      : inst(o.instance())
   {
   }
#endif
};

template<class T, class Allocator>
inline re_alloc_binder<T, Allocator>::re_alloc_binder(const Allocator &i)
    : inst(i)
{}


//
// class jm_def_alloc
// basically a standard allocator that only allocates bytes...
//
class jm_def_alloc
{
public:
   typedef char         value_type;
   typedef char*        pointer;
   typedef const char*  const_pointer;
   typedef char&        reference;
   typedef const char&  const_reference;
   typedef size_t    size_type;
   typedef __JM_STDC::ptrdiff_t difference_type;

   pointer RE_CALL address(reference x) const { return &x; }
   const_pointer RE_CALL address(const_reference x) const { return &x; }
   static size_type RE_CALL max_size() { return -1; }
   static void RE_CALL construct(pointer , const char& ) {  }
   void RE_CALL destroy(pointer ) {  }
   static void * RE_CALL allocate(size_t n)
   {
      return ::operator new(n);
   }
   static void RE_CALL deallocate(void *p, size_t /*n*/ )
   {
      ::operator delete(p);
   }

#ifndef JM_NO_MEMBER_TEMPLATES
   template <class U>
   struct rebind
   {
      typedef re_alloc_binder<U, jm_def_alloc> other;
   };

   //template <class U>
   //RE_CALL jm_def_alloc(const re_alloc_binder<U, jm_def_alloc>& ) throw() { }
#endif
   jm_def_alloc(const jm_def_alloc&) {}
   jm_def_alloc() {}
};


class dummy_iterator_base{};

// we need to absolutely sure that int values are correctly
// translated to bool (true or false) values...
// note that the HP STL redefines the bool type regardless
// of whether the compiler supports it.... yuk

#if defined(JM_NO_BOOL) || defined(ITERATOR_H) || defined(bool)
#define JM_MAKE_BOOL(x) boolify(x)

template <class I>
inline bool RE_CALL boolify(I val)
{
   return val ? true : false;
}

#else
#define JM_MAKE_BOOL(x) x
#endif

// class auto_array:
//
// encapsulates objects allocated with ::operator new[]()
// interface the same as auto_ptr, but no stream operators
//	since we don't know how big the array is.
//
// Usage: auto_array<char> buf = new char[256];
//

template<class X>
class auto_array
{
public: // construct/copy/destroy:

	auto_array(X* p =0)
	{ ptr = p; }

	auto_array(const auto_array& ap)
	{ ptr = const_cast<auto_array&>(ap).release(); }

	void RE_CALL operator=(const auto_array&);

	~auto_array();

		//  members:

	X& RE_CALL operator*() const;
	X* RE_CALL operator->() const;
	X* RE_CALL get() const;
   X& RE_CALL operator[](int i);
	X* RE_CALL release();

   // operator not part of the spec:
   RE_CALL operator X*()const
   { return ptr; }

private:	//  data:

	X* ptr;
};

template <class X>
inline void RE_CALL auto_array<X>::operator=(const auto_array<X>& ap)
{
   delete[] ptr;
	ptr = const_cast<auto_array<X>&>(ap).release();
}

template <class X>
inline auto_array<X>::~auto_array()
{
	delete[] ptr;
}

template <class X>
inline X& RE_CALL auto_array<X>::operator*() const
{
	return *ptr;
}

template <class X>
inline X* RE_CALL auto_array<X>::operator->() const
{
	return ptr;
}

template <class X>
inline X* RE_CALL auto_array<X>::get() const
{
	return ptr;
}

template <class X>
inline X& RE_CALL auto_array<X>::operator[](int i)
{
	return ptr[i];
}

template <class X>
inline X* RE_CALL auto_array<X>::release()
{
	X* tmp = ptr;
   ptr = NULL;
   return tmp;
}


JM_END_NAMESPACE

#ifndef INLINE_EXCEPTION_BUG
    #define CONSTRUCTOR_INLINE inline
#else
    #define CONSTRUCTOR_INLINE
#endif

#if defined(JM_PLATFORM_W32) && !defined(RE_LOCALE_W32) && !defined(RE_LOCALE_C) && !defined(RE_LOCALE_CPP)
#define RE_LOCALE_W32
#endif

#if !defined(RE_LOCALE_W32) && !defined(RE_LOCALE_C) && !defined(RE_LOCALE_CPP)
#define RE_LOCALE_C
#endif

#ifdef JM_OLD_ALLOCATORS
#define JM_MAYBE_TYPENAME
#else
#define JM_MAYBE_TYPENAME typename
#endif


/* now do debugging stuff: */

#ifdef JM_DEBUG

#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
#endif

   #ifndef jm_assert
      #define jm_assert(x) if((x) == 0){ cerr << "Assertion failed: " << #x << " in file " << __FILE__ << "and line " << __LINE__ << endl; exit(-1); }
   #endif
   #ifndef jm_trace
      #define jm_trace(x) cerr << x;
   #endif

   #ifdef __BORLANDC__
      #pragma message ("macro __jm_std: " __JM_STD)
      #pragma message ("macro __jm_stdc: " __JM_STDC)
      #pragma message "macro namespace: " JM_NAMESPACE(__JM_STD)
      #pragma message "macro allocator: " JM_DEF_ALLOC_PARAM(wchar_t)
      #pragma message "macro jm_input_iterator: " JM_INPUT_ITERATOR(char, __JM_STDC::ptrdiff_t)
      #pragma message "macro jm_output_iterator: " JM_OUTPUT_ITERATOR(char, __JM_STDC::ptrdiff_t)
      #pragma message "macro jm_fwd_iterator: " JM_FWD_ITERATOR(char, __JM_STDC::ptrdiff_t)
      #pragma message "macro jm_bidi_iterator: " JM_BIDI_ITERATOR(char, __JM_STDC::ptrdiff_t)
      #pragma message "macro jm_ra_iterator: " JM_RA_ITERATOR(char, __JM_STDC::ptrdiff_t)
      #ifndef JM_NO_LOCALE_H
         #pragma message ("locale support enabled")
      #endif
   #endif

#else

   #define jm_assert(x)
   #define jm_trace(x)

#endif

#endif  /* __cplusplus */


#endif









