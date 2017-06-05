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

int          clsDump::DumpCondition(clsCondition * pCondition, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- CONDITION BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pCondition->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Sensitive Events %u\n", pCondition->m_pSensitiveEvents.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pCondition->m_pSensitiveEvents.size(); ++iIndex)
  {
    iSize += DumpLine("\tSensitive Event UID %u\n", pCondition->m_pSensitiveEvents[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tCondition Type = %u\n", pCondition->m_Type, onlyCalculateSize);
  _Assert(pCondition->m_pNexus && "Frontend Error: Each condition should have a nexus.");
  iSize += DumpLine("\tNexus UID %u\n", pCondition->m_pNexus->m_UID, onlyCalculateSize);
  iSize += DumpLine("----- CONDITION END -----\n", onlyCalculateSize);

  return iSize;
}
