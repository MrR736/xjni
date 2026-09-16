/**
 * @file xjni_windows.h
 * @brief
 *
 * @author MrR736
 * @date 2026
 * @license MIT
 */

#ifndef XJNI_WINDOWS_H
#define XJNI_WINDOWS_H

#include "xjnidef.h"

#ifdef _WIN32
# include <windows.h>
# include <propidl.h>
#else // _WIN32
#include <stdint.h>
#include <wchar.h>

#ifndef __has_include
#define __has_include 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef WINAPI
#define WINAPI
#endif

#ifndef __C89_NAMELESS
#define __C89_NAMELESS XJNI_EXTENSION
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef VOID
#define VOID void
#endif


#ifndef WINBASEAPI
#define WINBASEAPI XJNI_IMPORT_API
#endif

xusing(CHAR,char);
xusing(LPSTR,CHAR*);
xusing(LPCSTR,const CHAR*);

xusing(WCHAR,wchar_t);
xusing(LPWSTR,WCHAR*);
xusing(LPCWSTR,const WCHAR*);

xusing(TCHAR,wchar_t);
xusing(LPTSTR,TCHAR*);
xusing(LPCTSTR,const TCHAR*);

xusing(UINT,uint32_t);
xusing(PUINT,UINT*);
xusing(INT8,int8_t);
xusing(UINT8,uint8_t);
xusing(INT16,int16_t);
xusing(UINT16,uint16_t);
xusing(INT32,int32_t);
xusing(UINT32,uint32_t);
xusing(INT64,int64_t);
xusing(UINT64,uint64_t);

xusing(HANDLE,void*);
xusing(HRESULT,INT32);
xusing(VARIANT_BOOL,INT16);
xusing(VARTYPE,UINT16);
xusing(WORD,UINT16);
xusing(DWORD,UINT32);

#ifndef __LP64__
#define __LONG32 long
#else
#define __LONG32 int
#endif

xusing(LONG, __LONG32);
xusing(ULONG,DWORD);

#ifdef __XJNI_x64__
# define XJNI_INT3264 int64_t
# define XJNI_UINT3264 uint64_t
#else
# define XJNI_INT3264 int32_t
# define XJNI_UINT3264 uint32_t
#endif

xusing(INT_PTR,XJNI_INT3264);
xusing(PINT_PTR,INT_PTR*);

xusing(UINT_PTR,XJNI_UINT3264);
xusing(PUINT_PTR,UINT_PTR*);

xusing(LONG_PTR,XJNI_INT3264);
xusing(PLONG_PTR,LONG_PTR*);

xusing(ULONG_PTR,XJNI_UINT3264);
xusing(PULONG_PTR,ULONG_PTR*);

xusing(DWORD_PTR,ULONG_PTR);
xusing(PDWORD_PTR,DWORD_PTR*);

xusing(OLECHAR,wchar_t);
xusing(LPOLESTR,OLECHAR*);
xusing(LPCOLESTR,const OLECHAR*);

#define OLESTR(str) L##str

xusing(BSTR,OLECHAR*);

xusing(LONGLONG,INT64);
xusing(ULONGLONG,UINT64);
xusing(PLONGLONG,LONGLONG*);
xusing(PULONGLONG,ULONGLONG*);

xusing(UCHAR,unsigned char);
xusing(SHORT,short);
xusing(USHORT,unsigned short);
xusing(DOUBLE,double);
xusing(FLOAT,float);

xusing(DWORDLONG,UINT64);
xusing(PDWORDLONG,DWORDLONG*);

xusing(PVOID,void*);
xusing(LPVOID,void*);

xusing(WINBOOL,int);

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
typedef struct FILETIME *PFILETIME;
typedef struct FILETIME *LPFILETIME;

struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};

typedef struct LARGE_INTEGER LARGE_INTEGER;

struct LARGE_INTEGER {
	LONGLONG QuadPart;
};

typedef struct ULARGE_INTEGER ULARGE_INTEGER;

struct ULARGE_INTEGER {
	ULONGLONG QuadPart;
};

typedef struct _LLFILETIME {
	__C89_NAMELESS union {
		LONGLONG ll;
		FILETIME ft;
	};
} LLFILETIME;

typedef struct SYSTEMTIME SYSTEMTIME;

struct SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};
typedef struct SYSTEMTIME *PSYSTEMTIME;
typedef struct SYSTEMTIME *LPSYSTEMTIME;

typedef struct _SYSTEM_INFO {
	__C89_NAMELESS union {
		DWORD dwOemId;
		__C89_NAMELESS struct {
			WORD wProcessorArchitecture;
			WORD wReserved;
		};
	};
	DWORD dwPageSize;
	LPVOID lpMinimumApplicationAddress;
	LPVOID lpMaximumApplicationAddress;
	DWORD_PTR dwActiveProcessorMask;
	DWORD dwNumberOfProcessors;
	DWORD dwProcessorType;
	DWORD dwAllocationGranularity;
	WORD wProcessorLevel;
	WORD wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

typedef struct _MEMORYSTATUSEX {
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORDLONG ullTotalPhys;
	DWORDLONG ullAvailPhys;
	DWORDLONG ullTotalPageFile;
	DWORDLONG ullAvailPageFile;
	DWORDLONG ullTotalVirtual;
	DWORDLONG ullAvailVirtual;
	DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX,*LPMEMORYSTATUSEX;

#endif // _WIN32

#endif /* XJNI_WINDOWS_H */
