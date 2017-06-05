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

int          clsDump::DumpConst(clsConst * pConst, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- CONST BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pConst->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tData Type UID %u\n", pConst->m_pDataType->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tConst Width: %u\n", pConst->m_pDataType->m_iWidth, onlyCalculateSize);
  for (iIndex = 0; iIndex < pConst->m_pDataType->m_iWidth; ++iIndex)
  {
    iSize += DumpByte("\tBits: %u\n", pConst->m_sBits[iIndex], onlyCalculateSize);
  }
  iSize += DumpByte("\tBits End: %u\n", '\0', onlyCalculateSize);
  iSize += DumpLine("----- CONST END -----\n", onlyCalculateSize);

  return iSize;
}
