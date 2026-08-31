/**
 * @file xjni_windows.h
 * @brief
 *
 * @author MrR736
 * @date 2026
 * @license GPL-3.0
 */

#ifndef XJNI_WINDOWS_H
#define XJNI_WINDOWS_H

#include "xjnidef.h"

#ifdef _WIN32
# include <windows.h>
# include <propidl.h>
#else // _WIN32
#include <wchar.h>

#ifndef __has_include
#define __has_include 0
#endif

#ifndef WINAPI
#define WINAPI
#endif

#if __has_include(<stdint.h>)
#include <stdint.h>
xusing(HRESULT,int32_t);
xusing(OLECHAR,wchar_t);
xusing(BSTR,OLECHAR*);
xusing(VARIANT_BOOL,int16_t);
xusing(VARTYPE,uint16_t);
xusing(WORD,uint16_t);
xusing(DWORD,uint32_t);
xusing(ULONG,uint32_t);
#else // __has_include
xusing(HRESULT,int);
xusing(OLECHAR,wchar_t);
xusing(BSTR,OLECHAR*);
xusing(VARIANT_BOOL,short);
xusing(VARTYPE,unsigned short);
xusing(WORD,unsigned short);
xusing(DWORD,unsigned int);
xusing(ULONG,unsigned int);
#endif // __has_include
xusing(PROPID,ULONG);


#ifdef __cplusplus
#ifndef S_OK
constexpr auto S_OK = static_cast< HRESULT >( 0x00000000L );
constexpr auto S_FALSE = static_cast< HRESULT >( 0x00000001L );
constexpr auto E_NOTIMPL = static_cast< HRESULT >( 0x80004001L );
constexpr auto E_NOINTERFACE = static_cast< HRESULT >( 0x80004002L );
constexpr auto E_ABORT = static_cast< HRESULT >( 0x80004004L );
constexpr auto E_FAIL = static_cast< HRESULT >( 0x80004005L );
constexpr auto STG_E_INVALIDFUNCTION = static_cast< HRESULT >( 0x80030001L );
constexpr auto E_OUTOFMEMORY = static_cast< HRESULT >( 0x8007000EL );
constexpr auto E_INVALIDARG = static_cast< HRESULT >( 0x80070057L );
#endif // S_OK
#else // __cplusplus
// Win32 HRESULT error codes.
#ifndef S_OK
#define S_OK ( HRESULT )( 0x00000000L )
#define S_FALSE ( HRESULT )( 0x00000001L );
#define E_NOTIMPL ( HRESULT )( 0x80004001L );
#define E_NOINTERFACE ( HRESULT )( 0x80004002L );
#define E_ABORT ( HRESULT )( 0x80004004L );
#define E_FAIL ( HRESULT )( 0x80004005L );
#define STG_E_INVALIDFUNCTION ( HRESULT )( 0x80030001L );
#define E_OUTOFMEMORY ( HRESULT )( 0x8007000EL );
#define E_INVALIDARG ( HRESULT )( 0x80070057L );
#endif // S_OK
#endif // __cplusplus

#ifndef CP_UTF8
#define CP_UTF8 65001u;
#endif

typedef struct FILETIME FILETIME;

struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};

typedef struct LARGE_INTEGER LARGE_INTEGER;

struct LARGE_INTEGER {
	int64_t QuadPart;
};

typedef struct ULARGE_INTEGER ULARGE_INTEGER;

struct ULARGE_INTEGER {
	uint64_t QuadPart;
};

#endif // _WIN32

#endif /* XJNI_WINDOWS_H */
