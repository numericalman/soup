/* Useful common macroses
   Author: Roy Bebru (RoyBebru@Gmail.Com)
*/

#ifndef _ETC_HPP_
# define _ETC_HPP_

typedef unsigned int uint;
typedef unsigned char uchar;

# define COUT_HEADER __func__ << "(" << __LINE__ << "):\t"

# define ABS(x)         ((x) < 0 ? -(x) : (x))

# define _0             '\0'
# define _LF            '\n'
# define _CR            '\r'
# define LF             "\n"
# define CR             "\r"
# define CRLF           "\r\n"

# define STR(x)         #x

# ifndef NDEBUG
// DEBUG ON

#  include <cstdio>

#  define G_(x)         x
#  define _G(x)         x
#  define C_(x,c)       x
#  define _C(x,c)       x

#  define OX_PRINTF     printf

#  define PC_(var)      OX_PRINTF("%s(%u):\t" STR(var) "='%c'" LF, __func__, __LINE__, var)
#  define PS_(var)      OX_PRINTF("%s(%u):\t" STR(var) "=\"%s\"" LF, __func__, __LINE__, var)
#  define PI_(var)      OX_PRINTF("%s(%u):\t" STR(var) "=%d" LF, __func__, __LINE__, var)
#  define PUI_(var)     OX_PRINTF("%s(%u):\t" STR(var) "=%u" LF, __func__, __LINE__, var)
#  define PLI_(var)     OX_PRINTF("%s(%u):\t" STR(var) "=%ld" LF, __func__, __LINE__, var)
#  define PLUI_(var)    OX_PRINTF("%s(%u):\t" STR(var) "=%lu" LF, __func__, __LINE__, var)
#  define PIX_(var)     OX_PRINTF("%s(%u):\t" STR(var) "=%X" LF, __func__, __LINE__, var)
#  define PLIX_(var)    OX_PRINTF("%s(%u):\t" STR(var) "=%lX" LF, __func__, __LINE__, var)
#  define PF_(var)      OX_PRINTF("%s(%u):\t" STR(var) "=%f" LF, __func__, __LINE__, (double) var)

#  define OX(b,l) \
{ \
    unsigned int ix; \
    for (ix = 0; ix < (l); ++ix) \
        OX_PRINTF("%02X%s", \
            ((unsigned char *) (b))[ix], \
            ((ix & 0x0F) == 0x0F) ? "\n" : (((ix & 0x07) == 0x07) ? " - " : " ") \
        ); \
}

# define OXLF(b,l) \
{ \
    unsigned int ix; \
    for (ix = 0; ix < (l); ++ix) \
        OX_PRINTF("%02X%s", \
            ((unsigned char *) (b))[ix], \
            ((ix & 0x0F) == 0x0F) ? "\n" : (((ix & 0x07) == 0x07) ? " - " : " ") \
        ); \
    OX_PRINTF(((ix & 0x0F) == 0x0F) ? "" : "\n"); \
}

#  define OXH(h,b,l) \
{ \
    unsigned int ix; \
    OX_PRINTF(h); \
    for (ix = 0; ix < (l); ++ix) \
        OX_PRINTF("%02X%s", \
            ((unsigned char *) (b))[ix], \
            ((ix & 0x0F) == 0x0F) ? "\n" : (((ix & 0x07) == 0x07) ? " - " : " ") \
        ); \
}

# define OXHLF(h,b,l) \
{ \
    unsigned int ix; \
    OX_PRINTF(h); \
    for (ix = 0; ix < (l); ++ix) \
        OX_PRINTF("%02X%s", \
            ((unsigned char *) (b))[ix], \
            ((ix & 0x0F) == 0x0F) ? "\n" : (((ix & 0x07) == 0x07) ? " - " : " ") \
        ); \
    OX_PRINTF(((ix & 0x0F) == 0x0F) ? "" : "\n"); \
}

# else
// DEBUG OFF

#  define G_(x)
#  define _G(x)
#  define C_(x,c)
#  define _C(x,c)

#  define PC_(var)
#  define PS_(var)
#  define PI_(var)
#  define PUI_(var)
#  define PLI_(var)
#  define PLUI_(var)
#  define PIX_(var)
#  define PLIX_(var)
#  define PF_(var)
#  define PE_(var)

#  define OX(b,l)
#  define OXLF(b,l)
#  define OXH(h,b,l)
#  define OXHLF(h,b,l)

# endif

#endif //_ETC_HPP_
