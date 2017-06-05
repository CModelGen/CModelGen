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

#ifndef FRONTEND_H
#define FRONTEND_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "checkmachine.h"
#include "version_base.h"
#include "version_tag.h"
#include <ivl_target.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
// Backtracing is not supported by Cygwin.
#ifdef CMODELGEN_LINUX
#include <execinfo.h>
#endif
#include "config.h"
#include "common.h"
#include "type.h"
#include "errormessage.h"

#define CMODELGEN_USES_0_10

ivl_nexus_t cmodelgen_ivl_lpm_q(ivl_lpm_t net, unsigned idx);

// C++ Entrance.
void FrontendMain(ivl_design_t * pDesign);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#ifdef __cplusplus
// Ported from vvp.
#include "vvp_priv.h"
// C++ declarations.
#include "ir.h"

#endif // __cplusplus

#endif // FRONTEND_H
