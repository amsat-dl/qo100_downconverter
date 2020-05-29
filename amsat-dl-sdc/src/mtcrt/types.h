#ifndef __CRT_TYPES_H__
#define __CRT_TYPES_H__

//basic signed types
typedef signed   char		s8;
typedef signed   short		s16;
typedef signed   long		s32;
typedef signed   long long	s64;
typedef signed   int		sint;

typedef signed   char		S8;
typedef signed   short		S16;
typedef signed   long		S32;
typedef signed   long long	S64;

//basic unsigned types
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;
typedef unsigned int		uint;

typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned long		U32;
typedef unsigned long long	U64;

//IO types
typedef volatile u8			IO8;
typedef volatile u16		IO16;
typedef volatile u32		IO32;

//other types
typedef u8					BYTE;
typedef int					BOOL;
typedef char*				LPSTR;
typedef const char*			LPCSTR;
typedef void*				LPVOID;
typedef const void*			LPCVOID;

//defines
#define TRUE  1
#define FALSE 0

#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))

#endif /*__CRT_TYPES_H__*/
