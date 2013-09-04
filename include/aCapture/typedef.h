#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef unsigned char		byte;
typedef unsigned __int64	u64;
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;
typedef unsigned int		Bool;
typedef __int64				s64;
typedef int					s32;
typedef short				s16;
typedef char				s8;
typedef float				Float;
typedef double				Double;
typedef long				Long;

#include <string>
#include <tchar.h>

#ifndef _UNICODE
	typedef std::string			String; 
	typedef std::stringstream	Stringstream;
	typedef char				Char;
#else
	typedef std::wstring		String; 
	typedef std::wstringstream	Stringstream;
	typedef wchar_t				Char;
#endif

#include "macro.h"

#endif