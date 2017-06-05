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

int          clsDump::DumpLocation(clsLocation Location, int onlyCalculateSize)
{
  int iSize = 0;

  iSize += DumpLine("- - - LOCATION BEG - - -\n", onlyCalculateSize);
  iSize += DumpString("\tFile\n", Location.m_sFile, onlyCalculateSize);
  iSize += DumpLine("\tLine %u\n", Location.m_iLine, onlyCalculateSize);
  iSize += DumpLine("- - - LOCATION END - - -\n", onlyCalculateSize);

  return iSize;
}
