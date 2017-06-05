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
#include "frontend.h"

// Define missing symbols of vvp.
// From vvp_process.c
unsigned local_count = 0;
unsigned thread_count = 0;
// From vvp.c
FILE*vvp_out = 0;
int vvp_errors = 0;
// End of define missing symbols of vvp.

FILE * pOutFile = NULL;
#ifdef CMODELGEN_DEBUG
FILE * pErrFile = NULL;
#endif

unsigned int gIndent = 0;

static const char * sVersion =
"Icarus Verilog CModelGen Target " STRING_S(VERSION) " \n\n"
"Copyright (c) 2016 CModelGen Project\nALL RIGHTS RESERVED\n"
"  This program is free software; you can redistribute it and/or modify\n"
"  it under the terms of the GNU General Public License as published by\n"
"  the Free Software Foundation; either version 2 of the License, or\n"
"  (at your option) any later version.\n"
"\n"
"  This program is distributed in the hope that it will be useful,\n"
"  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"  GNU General Public License for more details.\n"
"\n"
"  You should have received a copy of the GNU General Public License along\n"
"  with this program; if not, write to the Free Software Foundation, Inc.,\n"
"  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.\n"
;

int target_design(ivl_design_t theDesign)
{
  FrontendMain(&theDesign);
  return 0;
}

const char * target_query(const char * sKey)
{
  if(strcmp(sKey, "version") == 0)
  {
    return sVersion;
  }
  return 0;
}
