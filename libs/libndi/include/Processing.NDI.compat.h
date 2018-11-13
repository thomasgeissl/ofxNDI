#pragma once

#ifdef _WIN32

#include <windows.h>

#else

#include <stdbool.h>
#include <stdint.h>

typedef char     CHAR, *PCHAR, **PPCHAR;
typedef int16_t  SHORT, *PSHORT;
typedef int32_t  LONG, *PLONG;
typedef int64_t  LONGLONG, *PLONGLONG;
typedef uint8_t  UCHAR, *PUCHAR;
typedef uint16_t USHORT, *PUSHORT;
typedef uint32_t ULONG, *PULONG;
typedef uint64_t ULONGLONG, *PULONGLONG;
typedef uint8_t  BYTE;
typedef uint32_t DWORD;

#endif
