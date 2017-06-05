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
#ifndef CHECKMACHINE_H
#define CHECKMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

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

#endif // CHECKMACHINE_H
