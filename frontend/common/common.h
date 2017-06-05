/*
 *      Copyright (c) 2016 CModelGen Project
 *              ALL RIGHTS RESERVED
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    Library General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Library General Public License for more details.
 *
 *    You should have received a copy of the GNU Library General Public
 *    License along with this program; if not, write to the Free
 *    Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */
#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
// Stack back trace not supported by cygwin.
#include <execinfo.h>
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

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

#endif // COMMON_H
