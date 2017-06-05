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

int          clsDump::DumpSectionHeader(int iSectionID, int iSectionSize, int onlyCalculateSize)
{
  int iSize = 0;

  iSize += DumpLine("===== SECTION HEADER BEG =====\n", onlyCalculateSize);
  iSize += DumpLine("\tTYPE %#.8X\n", iSectionID, onlyCalculateSize);
  iSize += DumpLine("\tSIZE %u\n", iSectionSize, onlyCalculateSize);
  iSize += DumpLine("===== SECTION HEADER END =====\n", onlyCalculateSize);

  return iSize;
}
