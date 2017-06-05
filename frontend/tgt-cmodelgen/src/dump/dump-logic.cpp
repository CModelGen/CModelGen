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

int          clsDump::DumpLogic(clsLogic * pLogic, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- LOGIC BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pLogic->m_UID, onlyCalculateSize);
  iSize += DumpLocation(pLogic->m_Location, onlyCalculateSize);
  iSize += DumpLine("\tLogic Type = %u\n", pLogic->m_Type, onlyCalculateSize);
  iSize += DumpString("\tLogic Name\n", pLogic->m_sName, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Nexuses %u\n", pLogic->m_pNexuses.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pLogic->m_pNexuses.size(); ++iIndex)
  {
    iSize += DumpLine("\tNexus UID %u\n", pLogic->m_pNexuses[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("----- LOGIC END -----\n", onlyCalculateSize);

  return iSize;
}
