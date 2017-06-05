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

int          clsDump::DumpEvent(clsEvent * pEvent, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- EVENT BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pEvent->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Conditions %u\n", pEvent->m_pConditions.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pEvent->m_pConditions.size(); ++iIndex)
  {
    iSize += DumpLine("\tCondition UID %u\n", pEvent->m_pConditions[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tEvent Type = %u\n", pEvent->m_Type, onlyCalculateSize);
  _Assert(pEvent->m_pScope && "Frontend Error: Each event should belong to a scope.");
  iSize += DumpLine("\tScope UID %u\n", pEvent->m_pScope->m_UID, onlyCalculateSize);
  if (pEvent->m_Type == CMODELGEN_EVENT_EVALUATION || pEvent->m_Type == CMODELGEN_EVENT_SUBEVENT)
  {
    _Assert(pEvent->m_pBasicBlock && "Frontend Error: Each evaluation event should have a basic block.");
    iSize += DumpLine("\tBasicBlock UID %u\n", pEvent->m_pBasicBlock->m_UID, onlyCalculateSize);
    iSize += DumpLine("\tNumber of Subevents %u\n", pEvent->m_pSubEvents.size(), onlyCalculateSize);
    for (iIndex = 0; iIndex < pEvent->m_pSubEvents.size(); ++iIndex)
    {
      iSize += DumpLine("\tSubevent UID %u\n", pEvent->m_pSubEvents[iIndex]->m_UID, onlyCalculateSize);
    }
  }
  if (pEvent->m_Type == CMODELGEN_EVENT_NB_UPDATE)
  {
    _Assert(pEvent->m_pLValue && "Frontend Error: Each non-blocking update event should have a lvalue.");
    iSize += DumpLValue(pEvent->m_pLValue, onlyCalculateSize);
  }
  if (pEvent->m_Type == CMODELGEN_EVENT_LOGIC)
  {
    _Assert(pEvent->m_pLogic && "Frontend Error: Each logic event should have a logic.");
    iSize += DumpLine("\tLogic UID %u\n", pEvent->m_pLogic->m_UID, onlyCalculateSize);
  }
  if (pEvent->m_Type == CMODELGEN_EVENT_LPM)
  {
    _Assert(pEvent->m_pLpm && "Frontend Error: Each lpm event should have a lpm.");
    iSize += DumpLine("\tLpm UID %u\n", pEvent->m_pLpm->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("----- EVENT END -----\n", onlyCalculateSize);

  return iSize;
}
