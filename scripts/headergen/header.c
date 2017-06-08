
/// HEADER_BEGIN checkmachine.h

/// EXTERN_C_BEGIN
#include <stdlib.h>
/// EXTERN_C_END

#undef CMODELGEN_LINUX
#undef CMODELGEN_CYGWIN
#undef CMODELGEN_MINGW
#undef CMODELGEN_32
#undef CMODELGEN_64

// Note that CMODELGEN_OSX is defined in rule.mk.
#ifdef __CYGWIN__
#define CMODELGEN_CYGWIN
#else
  #if (defined(__MINGW32__) || defined(__WIN32__) )
    #define CMODELGEN_MINGW
  #else
    #define CMODELGEN_LINUX
  #endif
#endif

#if (defined(__amd64__) && !(defined(__i386__) ) )
#define CMODELGEN_64
#else
#define CMODELGEN_32
#endif

/// HEADER_END checkmachine.h

/// HEADER_BEGIN common.h

/// EXTERN_C_BEGIN
#include "checkmachine.h"
#include "common-local.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#ifdef CMODELGEN_LINUX
#include <unistd.h>
// Stack back trace is not supported by cygwin.
#include <execinfo.h>
#endif
/// EXTERN_C_END

#define uint32          uint32_t
#define uint64          uint64_t
#define int32           int32_t
#define int64           int64_t

// Some algorithms allocate fixed sized string for short strings.
#define CMODELGEN_STRING_LENGTH 200

#define STRING_I(X) #X
#define STRING_S(X) STRING_I(X)

#define MIN(A,B) ( ( (A) < (B) ) ? (A) : (B) )
#define MAX(A,B) ( ( (A) > (B) ) ? (A) : (B) )
#define FOREACH(IT,V) for (IT = V.begin(), IT ## End = V.end(); IT != IT ## End; ++IT)

extern FILE *       pOutFile;
extern unsigned int gIndent;

#define _IncIndent ++gIndent
#define _DecIndent (gIndent != 0 ? --gIndent : 0)

#ifdef CMODELGEN_DEBUG
#if (defined CMODELGEN_LINUX) && (defined CMODELGEN_DEBUG) && (defined CMODELGEN_USE_SIGNAL_HANDLER)
  #define _Assert(Arg)                                            \
  do {                                                            \
    if (!(Arg) )                                                  \
    {                                                             \
      PrintBackTrace();                                           \
    }                                                             \
    assert(Arg);                                                  \
  } while (0)
#else
  #define _Assert(Arg) assert(Arg)
#endif
#define _Debug(Args...) Args
#define _DebugOutputLine(Args...) _WriteOutputLine(Args)
#define _DebugOutput(Args...) _WriteOutput(Args)
#define _DebugPrint(Args...)                                      \
do                                                                \
{                                                                 \
  fprintf(stderr, Args);                                          \
  fprintf(stderr, "\n");                                          \
  fflush(NULL);                                                   \
} while(0)
#define _Crash(Arg)                                               \
do                                                                \
{                                                                 \
  if (!(Arg) )                                                    \
  {                                                               \
    *(char *)NULL = '\0';                                         \
  }                                                               \
} while(0)
#else // CMODELGEN_DEBUG
#define _Assert(Arg)
#define _Debug(Args...)
#define _DebugOutputLine(Args...)
#define _DebugOutput(Args...)
#define _DebugPrint(Args...)
#define _Crash(Arg)
#endif // CMODELGEN_DEBUG

#ifdef CMODELGEN_BACKEND
#define _OutputInformation(Args...)                               \
do                                                                \
{                                                                 \
  if (!clsOptions::Silent)                                        \
  {                                                               \
    fprintf(stdout, "[Output] ");                                 \
    fprintf(stdout, Args);                                        \
    fprintf(stdout, "\n");                                        \
  }                                                               \
} while(0)
#else
#define _OutputInformation(Args...)                               \
do                                                                \
{                                                                 \
  fprintf(stdout, "[Output] ");                                   \
  fprintf(stdout, Args);                                          \
  fprintf(stdout, "\n");                                          \
} while(0)
#endif

#define _OtherInformation(Args...)                                \
do                                                                \
{                                                                 \
  fprintf(stdout, Args);                                          \
  fprintf(stdout, "\n");                                          \
} while(0)

#ifdef CMODELGEN_DEBUG
#define _WriteOutputLine(Args...)                                 \
do                                                                \
{                                                                 \
  if (pOutFile)                                                   \
  {                                                               \
    unsigned int tmpIndex;                                        \
    for(tmpIndex = 0; tmpIndex < gIndent; ++tmpIndex)             \
    {                                                             \
      fprintf(pOutFile, "  ");                                    \
    }                                                             \
    fprintf(pOutFile, Args);                                      \
  }                                                               \
  fflush(NULL);                                                   \
} while(0)

#define _WriteOutput(Args...)                                     \
do                                                                \
{                                                                 \
  if (pOutFile)                                                   \
  {                                                               \
    fprintf(pOutFile, Args);                                      \
  }                                                               \
  fflush(NULL);                                                   \
} while (0)
#else
#define _WriteOutputLine(Args...)                                 \
do                                                                \
{                                                                 \
  if (pOutFile)                                                   \
  {                                                               \
    unsigned int tmpIndex;                                        \
    for(tmpIndex = 0; tmpIndex < gIndent; ++tmpIndex)             \
    {                                                             \
      fprintf(pOutFile, "  ");                                    \
    }                                                             \
    fprintf(pOutFile, Args);                                      \
  }                                                               \
} while(0)

#define _WriteOutput(Args...)                                     \
do                                                                \
{                                                                 \
  if (pOutFile)                                                   \
  {                                                               \
    fprintf(pOutFile, Args);                                      \
  }                                                               \
} while (0)
#endif

#define _FlushOutput()                                            \
do                                                                \
{                                                                 \
  if (pOutFile)                                                   \
  {                                                               \
    fflush(pOutFile);                                             \
  }                                                               \
} while (0)

/// HEADER_END common.h

/// HEADER_BEGIN datatype.h

// Verilog standards defined data type related information.

// The Verilog standards require that the minimum max length of identifier 1024 charactors.
#define MAX_LENGTH_IDENTIFIER 1024U

// Datatype "long" has different definitions on 64-bit 64LP (Windows) and 64LLP (Linux) systems.
// However, "int" is always 32-bit, and "long long" is always 64-bit on all supported 32-bit and
// 64-bit targets.

#define DATATYPE_NORMAL_SIZE_32                       32
#define DATATYPE_NORMAL_NUMBER_OF_BYTES_32            4
#define DATATYPE_NORMAL_SUFFIX_SIGNED_32              ""
#define DATATYPE_NORMAL_SUFFIX_UNSIGNED_32            "U"
#define DATATYPE_NORMAL_NAME_SIGNED_32                PLI_INT32
#define DATATYPE_NORMAL_NAME_UNSIGNED_32              PLI_UINT32
#define DATATYPE_NORMAL_ZERO_SIGNED_32                0
#define DATATYPE_NORMAL_ZERO_UNSIGNED_32              0U
#define DATATYPE_NORMAL_ONE_SIGNED_32                 1
#define DATATYPE_NORMAL_ONE_UNSIGNED_32               1U
#define DATATYPE_NORMAL_ALLONE_SIGNED_32              0xFFFFFFFF
#define DATATYPE_NORMAL_ALLONE_UNSIGNED_32            0xFFFFFFFFU

#define DATATYPE_WIDE_SIZE_32                         64
#define DATATYPE_WIDE_NUMBER_OF_BYTES_32              8
#define DATATYPE_WIDE_SUFFIX_SIGNED_32                "LL"
#define DATATYPE_WIDE_SUFFIX_UNSIGNED_32              "ULL"
#define DATATYPE_WIDE_NAME_SIGNED_32                  PLI_INT64
#define DATATYPE_WIDE_NAME_UNSIGNED_32                PLI_UINT64
#define DATATYPE_WIDE_ZERO_SIGNED_32                  0LL
#define DATATYPE_WIDE_ZERO_UNSIGNED_32                0ULL
#define DATATYPE_WIDE_ONE_SIGNED_32                   1LL
#define DATATYPE_WIDE_ONE_UNSIGNED_32                 1ULL
#define DATATYPE_WIDE_ALLONE_SIGNED_32                0xFFFFFFFFFFFFFFFFLL
#define DATATYPE_WIDE_ALLONE_UNSIGNED_32              0xFFFFFFFFFFFFFFFFULL

#define DATATYPE_ARRAY_SIZE_32                        32
#define DATATYPE_ARRAY_SUFFIX_SIGNED_32               ""
#define DATATYPE_ARRAY_SUFFIX_UNSIGNED_32             "U"
#define DATATYPE_ARRAY_NAME_SIGNED_32                 PLI_INT32
#define DATATYPE_ARRAY_NAME_UNSIGNED_32               PLI_UINT32
#define DATATYPE_ARRAY_ZERO_SIGNED_32                 0
#define DATATYPE_ARRAY_ZERO_UNSIGNED_32               0U
#define DATATYPE_ARRAY_ONE_SIGNED_32                  1
#define DATATYPE_ARRAY_ONE_UNSIGNED_32                1U
#define DATATYPE_ARRAY_ALLONE_SIGNED_32               0xFFFFFFFF
#define DATATYPE_ARRAY_ALLONE_UNSIGNED_32             0xFFFFFFFFU
#define DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT_32   5
#define DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_32(WIDTH)   ( ( (WIDTH) + 31) >> 5)
#define DATATYPE_ARRAY_NUMBER_OF_BYTES_32(WIDTH)      (DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_32(WIDTH) << 2)
#define DATATYPE_ARRAY_BYTES_PER_ELEMENT_32(ELEMENTS) ( (ELEMENTS) << 2)

#define DATATYPE_NORMAL_SIZE_64                       32
#define DATATYPE_NORMAL_NUMBER_OF_BYTES_64            4
#define DATATYPE_NORMAL_SUFFIX_SIGNED_64              ""
#define DATATYPE_NORMAL_SUFFIX_UNSIGNED_64            "U"
#define DATATYPE_NORMAL_NAME_SIGNED_64                PLI_INT32
#define DATATYPE_NORMAL_NAME_UNSIGNED_64              PLI_UINT32
#define DATATYPE_NORMAL_ZERO_SIGNED_64                0
#define DATATYPE_NORMAL_ZERO_UNSIGNED_64              0U
#define DATATYPE_NORMAL_ONE_SIGNED_64                 1
#define DATATYPE_NORMAL_ONE_UNSIGNED_64               1U
#define DATATYPE_NORMAL_ALLONE_SIGNED_64              0xFFFFFFFF
#define DATATYPE_NORMAL_ALLONE_UNSIGNED_64            0xFFFFFFFFU

#define DATATYPE_WIDE_SIZE_64                         64
#define DATATYPE_WIDE_NUMBER_OF_BYTES_64              8
#define DATATYPE_WIDE_SUFFIX_SIGNED_64                "LL"
#define DATATYPE_WIDE_SUFFIX_UNSIGNED_64              "ULL"
#define DATATYPE_WIDE_NAME_SIGNED_64                  PLI_INT64
#define DATATYPE_WIDE_NAME_UNSIGNED_64                PLI_UINT64
#define DATATYPE_WIDE_ZERO_SIGNED_64                  0LL
#define DATATYPE_WIDE_ZERO_UNSIGNED_64                0ULL
#define DATATYPE_WIDE_ONE_SIGNED_64                   1LL
#define DATATYPE_WIDE_ONE_UNSIGNED_64                 1ULL
#define DATATYPE_WIDE_ALLONE_SIGNED_64                0xFFFFFFFFFFFFFFFFLL
#define DATATYPE_WIDE_ALLONE_UNSIGNED_64              0xFFFFFFFFFFFFFFFFULL

#define DATATYPE_ARRAY_SIZE_64                        64
#define DATATYPE_ARRAY_SUFFIX_SIGNED_64               "LL"
#define DATATYPE_ARRAY_SUFFIX_UNSIGNED_64             "ULL"
#define DATATYPE_ARRAY_NAME_SIGNED_64                 PLI_INT64
#define DATATYPE_ARRAY_NAME_UNSIGNED_64               PLI_UINT64
#define DATATYPE_ARRAY_ZERO_SIGNED_64                 0LL
#define DATATYPE_ARRAY_ZERO_UNSIGNED_64               0ULL
#define DATATYPE_ARRAY_ONE_SIGNED_64                  1LL
#define DATATYPE_ARRAY_ONE_UNSIGNED_64                1ULL
#define DATATYPE_ARRAY_ALLONE_SIGNED_64               0xFFFFFFFFFFFFFFFFLL
#define DATATYPE_ARRAY_ALLONE_UNSIGNED_64             0xFFFFFFFFFFFFFFFFULL
#define DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT_64   6
#define DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_64(WIDTH)   ( ( (WIDTH) + 63) >> 6)
#define DATATYPE_ARRAY_NUMBER_OF_BYTES_64(WIDTH)      (DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_64(WIDTH) << 3)
#define DATATYPE_ARRAY_BYTES_PER_ELEMENT_64(ELEMENTS) ( (ELEMENTS) << 3)

#ifdef CMODELGEN_64

#define DATATYPE_NORMAL_SIZE                          DATATYPE_NORMAL_SIZE_64
#define DATATYPE_NORMAL_NUMBER_OF_BYTES               DATATYPE_NORMAL_NUMBER_OF_BYTES_64
#define DATATYPE_NORMAL_SUFFIX_SIGNED                 DATATYPE_NORMAL_SUFFIX_SIGNED_64
#define DATATYPE_NORMAL_SUFFIX_UNSIGNED               DATATYPE_NORMAL_SUFFIX_UNSIGNED_64
#define DATATYPE_NORMAL_NAME_SIGNED                   DATATYPE_NORMAL_NAME_SIGNED_64
#define DATATYPE_NORMAL_NAME_UNSIGNED                 DATATYPE_NORMAL_NAME_UNSIGNED_64
#define DATATYPE_NORMAL_ZERO_SIGNED                   DATATYPE_NORMAL_ZERO_SIGNED_64
#define DATATYPE_NORMAL_ZERO_UNSIGNED                 DATATYPE_NORMAL_ZERO_UNSIGNED_64
#define DATATYPE_NORMAL_ONE_SIGNED                    DATATYPE_NORMAL_ONE_SIGNED_64
#define DATATYPE_NORMAL_ONE_UNSIGNED                  DATATYPE_NORMAL_ONE_UNSIGNED_64
#define DATATYPE_NORMAL_ALLONE_SIGNED                 DATATYPE_NORMAL_ALLONE_SIGNED_64
#define DATATYPE_NORMAL_ALLONE_UNSIGNED               DATATYPE_NORMAL_ALLONE_UNSIGNED_64

#define DATATYPE_WIDE_SIZE                            DATATYPE_WIDE_SIZE_64
#define DATATYPE_WIDE_NUMBER_OF_BYTES                 DATATYPE_WIDE_NUMBER_OF_BYTES_64
#define DATATYPE_WIDE_SUFFIX_SIGNED                   DATATYPE_WIDE_SUFFIX_SIGNED_64
#define DATATYPE_WIDE_SUFFIX_UNSIGNED                 DATATYPE_WIDE_SUFFIX_UNSIGNED_64
#define DATATYPE_WIDE_NAME_SIGNED                     DATATYPE_WIDE_NAME_SIGNED_64
#define DATATYPE_WIDE_NAME_UNSIGNED                   DATATYPE_WIDE_NAME_UNSIGNED_64
#define DATATYPE_WIDE_ZERO_SIGNED                     DATATYPE_WIDE_ZERO_SIGNED_64
#define DATATYPE_WIDE_ZERO_UNSIGNED                   DATATYPE_WIDE_ZERO_UNSIGNED_64
#define DATATYPE_WIDE_ONE_SIGNED                      DATATYPE_WIDE_ONE_SIGNED_64
#define DATATYPE_WIDE_ONE_UNSIGNED                    DATATYPE_WIDE_ONE_UNSIGNED_64
#define DATATYPE_WIDE_ALLONE_SIGNED                   DATATYPE_WIDE_ALLONE_SIGNED_64
#define DATATYPE_WIDE_ALLONE_UNSIGNED                 DATATYPE_WIDE_ALLONE_UNSIGNED_64

#define DATATYPE_ARRAY_SIZE                           DATATYPE_ARRAY_SIZE_64
#define DATATYPE_ARRAY_SUFFIX_SIGNED                  DATATYPE_ARRAY_SUFFIX_SIGNED_64
#define DATATYPE_ARRAY_SUFFIX_UNSIGNED                DATATYPE_ARRAY_SUFFIX_UNSIGNED_64
#define DATATYPE_ARRAY_NAME_SIGNED                    DATATYPE_ARRAY_NAME_SIGNED_64
#define DATATYPE_ARRAY_NAME_UNSIGNED                  DATATYPE_ARRAY_NAME_UNSIGNED_64
#define DATATYPE_ARRAY_ZERO_SIGNED                    DATATYPE_ARRAY_ZERO_SIGNED_64
#define DATATYPE_ARRAY_ZERO_UNSIGNED                  DATATYPE_ARRAY_ZERO_UNSIGNED_64
#define DATATYPE_ARRAY_ONE_SIGNED                     DATATYPE_ARRAY_ONE_SIGNED_64
#define DATATYPE_ARRAY_ONE_UNSIGNED                   DATATYPE_ARRAY_ONE_UNSIGNED_64
#define DATATYPE_ARRAY_ALLONE_SIGNED                  DATATYPE_ARRAY_ALLONE_SIGNED_64
#define DATATYPE_ARRAY_ALLONE_UNSIGNED                DATATYPE_ARRAY_ALLONE_UNSIGNED_64
#define DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT      DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT_64
#define DATATYPE_ARRAY_NUMBER_OF_ELEMENTS(WIDTH)      DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_64(WIDTH)
#define DATATYPE_ARRAY_NUMBER_OF_BYTES(WIDTH)         DATATYPE_ARRAY_NUMBER_OF_BYTES_64(WIDTH)
#define DATATYPE_ARRAY_BYTES_PER_ELEMENT(ELEMENTS)    DATATYPE_ARRAY_BYTES_PER_ELEMENT_64(ELEMENTS)

#else

#define DATATYPE_NORMAL_SIZE                          DATATYPE_NORMAL_SIZE_32
#define DATATYPE_NORMAL_NUMBER_OF_BYTES               DATATYPE_NORMAL_NUMBER_OF_BYTES_32
#define DATATYPE_NORMAL_SUFFIX_SIGNED                 DATATYPE_NORMAL_SUFFIX_SIGNED_32
#define DATATYPE_NORMAL_SUFFIX_UNSIGNED               DATATYPE_NORMAL_SUFFIX_UNSIGNED_32
#define DATATYPE_NORMAL_NAME_SIGNED                   DATATYPE_NORMAL_NAME_SIGNED_32
#define DATATYPE_NORMAL_NAME_UNSIGNED                 DATATYPE_NORMAL_NAME_UNSIGNED_32
#define DATATYPE_NORMAL_ZERO_SIGNED                   DATATYPE_NORMAL_ZERO_SIGNED_32
#define DATATYPE_NORMAL_ZERO_UNSIGNED                 DATATYPE_NORMAL_ZERO_UNSIGNED_32
#define DATATYPE_NORMAL_ONE_SIGNED                    DATATYPE_NORMAL_ONE_SIGNED_32
#define DATATYPE_NORMAL_ONE_UNSIGNED                  DATATYPE_NORMAL_ONE_UNSIGNED_32
#define DATATYPE_NORMAL_ALLONE_SIGNED                 DATATYPE_NORMAL_ALLONE_SIGNED_32
#define DATATYPE_NORMAL_ALLONE_UNSIGNED               DATATYPE_NORMAL_ALLONE_UNSIGNED_32

#define DATATYPE_WIDE_SIZE                            DATATYPE_WIDE_SIZE_32
#define DATATYPE_WIDE_NUMBER_OF_BYTES                 DATATYPE_WIDE_NUMBER_OF_BYTES_32
#define DATATYPE_WIDE_SUFFIX_SIGNED                   DATATYPE_WIDE_SUFFIX_SIGNED_32
#define DATATYPE_WIDE_SUFFIX_UNSIGNED                 DATATYPE_WIDE_SUFFIX_UNSIGNED_32
#define DATATYPE_WIDE_NAME_SIGNED                     DATATYPE_WIDE_NAME_SIGNED_32
#define DATATYPE_WIDE_NAME_UNSIGNED                   DATATYPE_WIDE_NAME_UNSIGNED_32
#define DATATYPE_WIDE_ZERO_SIGNED                     DATATYPE_WIDE_ZERO_SIGNED_32
#define DATATYPE_WIDE_ZERO_UNSIGNED                   DATATYPE_WIDE_ZERO_UNSIGNED_32
#define DATATYPE_WIDE_ONE_SIGNED                      DATATYPE_WIDE_ONE_SIGNED_32
#define DATATYPE_WIDE_ONE_UNSIGNED                    DATATYPE_WIDE_ONE_UNSIGNED_32
#define DATATYPE_WIDE_ALLONE_SIGNED                   DATATYPE_WIDE_ALLONE_SIGNED_32
#define DATATYPE_WIDE_ALLONE_UNSIGNED                 DATATYPE_WIDE_ALLONE_UNSIGNED_32

#define DATATYPE_ARRAY_SIZE                           DATATYPE_ARRAY_SIZE_32
#define DATATYPE_ARRAY_SUFFIX_SIGNED                  DATATYPE_ARRAY_SUFFIX_SIGNED_32
#define DATATYPE_ARRAY_SUFFIX_UNSIGNED                DATATYPE_ARRAY_SUFFIX_UNSIGNED_32
#define DATATYPE_ARRAY_NAME_SIGNED                    DATATYPE_ARRAY_NAME_SIGNED_32
#define DATATYPE_ARRAY_NAME_UNSIGNED                  DATATYPE_ARRAY_NAME_UNSIGNED_32
#define DATATYPE_ARRAY_ZERO_SIGNED                    DATATYPE_ARRAY_ZERO_SIGNED_32
#define DATATYPE_ARRAY_ZERO_UNSIGNED                  DATATYPE_ARRAY_ZERO_UNSIGNED_32
#define DATATYPE_ARRAY_ONE_SIGNED                     DATATYPE_ARRAY_ONE_SIGNED_32
#define DATATYPE_ARRAY_ONE_UNSIGNED                   DATATYPE_ARRAY_ONE_UNSIGNED_32
#define DATATYPE_ARRAY_ALLONE_SIGNED                  DATATYPE_ARRAY_ALLONE_SIGNED_32
#define DATATYPE_ARRAY_ALLONE_UNSIGNED                DATATYPE_ARRAY_ALLONE_UNSIGNED_32
#define DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT      DATATYPE_ARRAY_ORDER_OF_WIDTH_OF_ELEMENT_32
#define DATATYPE_ARRAY_NUMBER_OF_ELEMENTS(WIDTH)      DATATYPE_ARRAY_NUMBER_OF_ELEMENTS_32(WIDTH)
#define DATATYPE_ARRAY_NUMBER_OF_BYTES(WIDTH)         DATATYPE_ARRAY_NUMBER_OF_BYTES_32(WIDTH)
#define DATATYPE_ARRAY_BYTES_PER_ELEMENT(ELEMENTS)    DATATYPE_ARRAY_BYTES_PER_ELEMENT_32(ELEMENTS)

#endif

#define DATATYPE_NUMBER_OF_BYTES(WIDTH)                                  \
( ( (WIDTH) <= DATATYPE_NORMAL_SIZE) ? DATATYPE_NORMAL_NUMBER_OF_BYTES : \
  ( (WIDTH) <= DATATYPE_WIDE_SIZE)   ? DATATYPE_WIDE_NUMBER_OF_BYTES   : \
  DATATYPE_ARRAY_NUMBER_OF_BYTES(WIDTH) )

/* Properties has the following bits:                           */
/* [31..16   ]                                                  */
/* [Width - 1]                                                  */
/* [15        ] [14      ][13      ] [12..2               ]     */
/* [Signedness] [Reserved][Reserved] [Number of bytes >> 2]     */
/* [1           ] [0            ]                               */
/* [Read protect] [Write protect]                               */

/* Construct properties */
#define MGL_MAKE_PROPERTIES(SIGNED,WIDTH)                                \
  ( ( ( (WIDTH) - 1) << 16) | ( (SIGNED) ? (1 << 15) : 0) |              \
    (DATATYPE_NUMBER_OF_BYTES(WIDTH) ) )
/* Take signed out of properties */
#define MGL_SIGNED(PROPERTIES) ( ( (PROPERTIES) >> 15) & 0x1)
/* Take number of bytes out of properties */
#define MGL_NOB(PROPERTIES) ( (PROPERTIES) & 0x1FFC)
/* Take width out of properties */
#define MGL_WIDTH(PROPERTIES) ( ( (PROPERTIES) >> 16) + 1)
/* The read protect bit */
#define MGL_READPROTECT_BIT 0x2
/* Read the read protect bit */
#define MGL_IS_READPROTECT(PROPERTIES) ( (PROPERTIES) & MGL_READPROTECT_BIT)
/* The write protect bit */
#define MGL_WRITEPROTECT_BIT 0x1
/* Read the write protect bit */
#define MGL_IS_WRITEPROTECT(PROPERTIES) ( (PROPERTIES) & MGL_WRITEPROTECT_BIT)

/// HEADER_END datatype.h

/// HEADER_BEGIN errorcode.h

// Error codes for the frontend.
#define ERROR_CODE_FRONTEND_MAGIC_CODE           0x00000001
#define ERROR_CODE_FRONTEND_MAGIC_END_CODE       0x00000002
#define ERROR_CODE_FRONTEND_SEEK_FAILED          0x00000003
#define ERROR_CODE_FRONTEND_WRONG_FILE_SIZE      0x00000004
#define ERROR_CODE_FRONTEND_WRONG_SECTION_HEADER 0x00000005
#define ERROR_CODE_FRONTEND_WRONG_SECTION_SIZE   0x00000006
#define ERROR_CODE_FRONTEND_WRONG_SCOPE          0x00000007
#define ERROR_CODE_FRONTEND_WRONG_LOCATION       0x00000008
#define ERROR_CODE_FRONTEND_WRONG_STRING         0x00000009
#define ERROR_CODE_FRONTEND_WRONG_SCOPE_TYPE     0x0000000A
#define ERROR_CODE_FRONTEND_WRONG_DATA_TYPE      0x0000000B
#define ERROR_CODE_FRONTEND_WRONG_SIGNAL         0x0000000C
#define ERROR_CODE_FRONTEND_WRONG_NEXUS          0x0000000D
#define ERROR_CODE_FRONTEND_WRONG_LOGIC          0x0000000E
#define ERROR_CODE_FRONTEND_WRONG_LPM            0x0000000F
#define ERROR_CODE_FRONTEND_WRONG_CONST          0x00000010
#define ERROR_CODE_FRONTEND_WRONG_PARAMETER      0x00000011
#define ERROR_CODE_FRONTEND_WRONG_EXPRESSION     0x00000012
#define ERROR_CODE_FRONTEND_WRONG_LVALUE         0x00000013
#define ERROR_CODE_FRONTEND_WRONG_EVENT          0x00000014
#define ERROR_CODE_FRONTEND_WRONG_CONDITION      0x00000015
#define ERROR_CODE_FRONTEND_WRONG_STATEMENT      0x00000016
#define ERROR_CODE_FRONTEND_WRONG_BASICBLOCK     0x00000017

// Warning codes.
#define WARNING_CODE_IGNORED                     0x00000001
#define WARNING_CODE_REPLACED                    0x00000002

/// HEADER_END errorcode.h

/// HEADER_BEGIN type.h

enum typDataType
{
  CMODELGEN_DATATYPE_PREFERRED,
  CMODELGEN_DATATYPE_WIDE,
  CMODELGEN_DATATYPE_EXTENDED
};

enum typSignal
{
  CMODELGEN_SIGNAL_REG,
  CMODELGEN_SIGNAL_TRI,
  CMODELGEN_SIGNAL_TRI0,
  CMODELGEN_SIGNAL_TRI1,
  CMODELGEN_SIGNAL_TRIAND,
  CMODELGEN_SIGNAL_TRIOR,
  CMODELGEN_SIGNAL_SUPPLY0,
  CMODELGEN_SIGNAL_SUPPLY1,
  CMODELGEN_SIGNAL_NONE
};

enum typLogic
{
  // N-output.
  CMODELGEN_LOGIC_BUFZ,
  CMODELGEN_LOGIC_BUF,
  CMODELGEN_LOGIC_NOT,
  // N-input.
  CMODELGEN_LOGIC_AND,
  CMODELGEN_LOGIC_NAND,
  CMODELGEN_LOGIC_OR,
  CMODELGEN_LOGIC_NOR,
  CMODELGEN_LOGIC_XOR,
  CMODELGEN_LOGIC_XNOR,
  // Only one output.
  CMODELGEN_LOGIC_PULLUP,
  CMODELGEN_LOGIC_PULLDOWN,
  // Enable.
  CMODELGEN_LOGIC_BUFIF0,
  CMODELGEN_LOGIC_BUFIF1,
  CMODELGEN_LOGIC_NOTIF0,
  CMODELGEN_LOGIC_NOTIF1,
  CMODELGEN_LOGIC_NONE
};

enum typLpm
{
  CMODELGEN_LPM_ADD,
  CMODELGEN_LPM_SUB,
  CMODELGEN_LPM_MUL,
  CMODELGEN_LPM_DIV,
  CMODELGEN_LPM_MOD,
  CMODELGEN_LPM_POW,
  CMODELGEN_LPM_EQ,
  CMODELGEN_LPM_NE,
  CMODELGEN_LPM_GE,
  CMODELGEN_LPM_GT,
  CMODELGEN_LPM_TRI_EQ,
  CMODELGEN_LPM_TRI_NE,
  CMODELGEN_LPM_SHL,
  CMODELGEN_LPM_SHR,
  CMODELGEN_LPM_RDCT_AND,
  CMODELGEN_LPM_RDCT_NAND,
  CMODELGEN_LPM_RDCT_OR,
  CMODELGEN_LPM_RDCT_NOR,
  CMODELGEN_LPM_RDCT_XOR,
  CMODELGEN_LPM_RDCT_XNOR,
  CMODELGEN_LPM_SEXT,
  CMODELGEN_LPM_MUX,
  CMODELGEN_LPM_PART_VP,
  CMODELGEN_LPM_PART_PV,
  CMODELGEN_LPM_CONCAT,
  CMODELGEN_LPM_REPEAT,
  CMODELGEN_LPM_ARRAY,
  CMODELGEN_LPM_UFUNC,
  CMODELGEN_LPM_NONE
};

enum typOperator
{
  CMODELGEN_OPERATOR_ADD,
  CMODELGEN_OPERATOR_SUB,
  CMODELGEN_OPERATOR_MUL,
  CMODELGEN_OPERATOR_DIV,
  CMODELGEN_OPERATOR_MOD,
  CMODELGEN_OPERATOR_POW,
  CMODELGEN_OPERATOR_AND,
  CMODELGEN_OPERATOR_OR,
  CMODELGEN_OPERATOR_NAND,
  CMODELGEN_OPERATOR_NOR,
  CMODELGEN_OPERATOR_XOR,
  CMODELGEN_OPERATOR_XNOR,
  CMODELGEN_OPERATOR_EQ,
  CMODELGEN_OPERATOR_NE,
  CMODELGEN_OPERATOR_GE,
  CMODELGEN_OPERATOR_GT,
  CMODELGEN_OPERATOR_LE,
  CMODELGEN_OPERATOR_LT,
  CMODELGEN_OPERATOR_CASE_EQ,
  CMODELGEN_OPERATOR_CASE_NE,
  CMODELGEN_OPERATOR_SHL,
  CMODELGEN_OPERATOR_SHR,
  CMODELGEN_OPERATOR_ASHR,
  CMODELGEN_OPERATOR_NOT,
  CMODELGEN_OPERATOR_LOGIC_NOT,
  CMODELGEN_OPERATOR_LOGIC_AND,
  CMODELGEN_OPERATOR_LOGIC_OR,
  CMODELGEN_OPERATOR_NONE
};

enum typScope
{
  CMODELGEN_SCOPE_MODULE,
  CMODELGEN_SCOPE_BEGIN,
  CMODELGEN_SCOPE_FUNCTION,
  CMODELGEN_SCOPE_TASK,
  CMODELGEN_SCOPE_NONE
};

enum typExpression
{
  CMODELGEN_EXPRESSION_SELECT,
  CMODELGEN_EXPRESSION_UNARY,
  CMODELGEN_EXPRESSION_BINARY,
  CMODELGEN_EXPRESSION_CONDITIONAL,
  CMODELGEN_EXPRESSION_NUMBER,
  CMODELGEN_EXPRESSION_SIGNAL,
  CMODELGEN_EXPRESSION_ARRAY,
  CMODELGEN_EXPRESSION_STRING,
  CMODELGEN_EXPRESSION_CONCAT,
  CMODELGEN_EXPRESSION_FUNCTION,
  CMODELGEN_EXPRESSION_SFUNCTION,
  CMODELGEN_EXPRESSION_NONE
};

enum typStatement
{
  CMODELGEN_STATEMENT_CASEZ,
  CMODELGEN_STATEMENT_CASEX,
  CMODELGEN_STATEMENT_CASE,
  CMODELGEN_STATEMENT_COND,
  CMODELGEN_STATEMENT_REPEAT,
  CMODELGEN_STATEMENT_WHILE,
  CMODELGEN_STATEMENT_NB_ASSIGN,
  CMODELGEN_STATEMENT_ASSIGN,
  CMODELGEN_STATEMENT_STASK,
  CMODELGEN_STATEMENT_UTASK,
  CMODELGEN_STATEMENT_ALLOC,
  CMODELGEN_STATEMENT_FREE,
  CMODELGEN_STATEMENT_WAIT,
  CMODELGEN_STATEMENT_DISABLE,
  CMODELGEN_STATEMENT_NOP
};

enum typVector
{
  CMODELGEN_VECTOR_SCALAR,
  CMODELGEN_VECTOR_VECTOR,
  CMODELGEN_VECTOR_VIRTUAL_VECTOR
};

enum typPort
{
  CMODELGEN_PORT_IN,
  CMODELGEN_PORT_OUT,
  CMODELGEN_PORT_INOUT,
  CMODELGEN_PORT_NONE
};

enum typCond
{
  CMODELGEN_COND_ANY,
  CMODELGEN_COND_ANYASSIGN,
  CMODELGEN_COND_POSEDGE,
  CMODELGEN_COND_NEGEDGE,
  CMODELGEN_COND_SIGNAL
};

enum typEvent
{
  CMODELGEN_EVENT_EVALUATION,
  CMODELGEN_EVENT_SUBEVENT,
  CMODELGEN_EVENT_NB_UPDATE,
  CMODELGEN_EVENT_LOGIC,
  CMODELGEN_EVENT_LPM,
  CMODELGEN_EVENT_MONITOR,
  CMODELGEN_EVENT_GROUP,
  CMODELGEN_EVENT_NONE
};

/// HEADER_END type.h

