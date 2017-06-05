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

int          clsDump::DumpSignal(clsSignal * pSignal, int onlyCalculateSize)
{
  int          iSize = 0;
  
  iSize += DumpLine("----- SIGNAL BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pSignal->m_UID, onlyCalculateSize);
  iSize += DumpLocation(pSignal->m_Location, onlyCalculateSize);
  iSize += DumpLine("\tVector Type = %u\n", pSignal->m_VectorType, onlyCalculateSize);
  iSize += DumpLine("\tType = %u\n", pSignal->m_Type, onlyCalculateSize);
  iSize += DumpLine("\tPort Type = %u\n", pSignal->m_PortType, onlyCalculateSize);
  iSize += DumpString("\tSignal Name\n", pSignal->m_sName, onlyCalculateSize);
  iSize += DumpLine("\tIndex = %u\n", pSignal->m_iIndex, onlyCalculateSize);
  iSize += DumpLine("\tSize = %u\n", pSignal->m_iSize, onlyCalculateSize);
  iSize += DumpLine("\tBaseAddress = %u\n", pSignal->m_BaseAddress, onlyCalculateSize);
  iSize += DumpLine("\tAddressSwapped = %u\n", pSignal->m_AddressSwapped, onlyCalculateSize);
  iSize += DumpLine("\tLocal = %u\n", pSignal->m_Local, onlyCalculateSize);
  _Assert(pSignal->m_pScope && "Frontend Error: Each signal should belong to a scope.");
  iSize += DumpLine("\tScope UID %u\n", pSignal->m_pScope->m_UID, onlyCalculateSize);
  _Assert(pSignal->m_pNexus && "Frontend Error: Each signal should have a nexus.");
  iSize += DumpLine("\tNexus UID %u\n", pSignal->m_pNexus->m_UID, onlyCalculateSize);
  _Assert(pSignal->m_pHeadSignal && "Frontend Error: Each signal should have a head signal.");
  iSize += DumpLine("\tHeadSignal UID %u\n", pSignal->m_pHeadSignal->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Assign = %u\n", pSignal->m_iNumberOfAssign, onlyCalculateSize);
  iSize += DumpLine("\tNumber of NB Assign = %u\n", pSignal->m_iNumberOfNBAssign, onlyCalculateSize);
  iSize += DumpLine("----- SIGNAL END -----\n", onlyCalculateSize);
  
  return iSize;
}

int          clsDump::DumpNexus(clsNexus * pNexus, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;
  
  iSize += DumpLine("----- NEXUS BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pNexus->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Signals %u\n", pNexus->m_pSignals.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pNexus->m_pSignals.size(); ++iIndex)
  {
    iSize += DumpLine("\tLinked Signal UID %u\n", pNexus->m_pSignals[iIndex]->m_UID, onlyCalculateSize);
  }
  _Assert(pNexus->m_pSignal && "Frontend Error: Each nexus should have a representative signal.");
  iSize += DumpLine("\tSignal UID %u\n", pNexus->m_pSignal->m_UID, onlyCalculateSize);
  _Assert(pNexus->m_pDataType && "Frontend Error: Each nexus should have a data type.");
  iSize += DumpLine("\tData Type UID %u\n", pNexus->m_pDataType->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Conditions %u\n", pNexus->m_pConditions.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pNexus->m_pConditions.size(); ++iIndex)
  {
    iSize += DumpLine("\tCondition UID %u\n", pNexus->m_pConditions[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Consts %u\n", pNexus->m_pConsts.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pNexus->m_pConsts.size(); ++iIndex)
  {
    iSize += DumpLine("\tConst UID %u\n", pNexus->m_pConsts[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Logic and Lpm Events %u\n", pNexus->m_pLogicAndLpmEvents.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pNexus->m_pLogicAndLpmEvents.size(); ++iIndex)
  {
    iSize += DumpLine("\tLogic and Lpm Event UID %u\n", pNexus->m_pLogicAndLpmEvents[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("----- NEXUS END -----\n", onlyCalculateSize);
  
  return iSize;
}

int          clsDump::DumpParameter(clsParameter * pParameter, int onlyCalculateSize)
{
  int          iSize = 0;
  
  iSize += DumpLine("----- PARAMETER BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pParameter->m_UID, onlyCalculateSize);
  iSize += DumpString("\tParameter Name\n", pParameter->m_sName, onlyCalculateSize);
  iSize += DumpLocation(pParameter->m_Location, onlyCalculateSize);
  _Assert(pParameter->m_pScope && "Frontend Error: Each parameter should belong to a scope.");
  iSize += DumpLine("\tScope UID %u\n", pParameter->m_pScope->m_UID, onlyCalculateSize);
  _Assert(pParameter->m_pExpression && "Frontend Error: Each parameter should have an expression.");
  iSize += DumpLine("\tExpression UID %u\n", pParameter->m_pExpression->m_UID, onlyCalculateSize);
  iSize += DumpLine("----- PARAMETER END -----\n", onlyCalculateSize);
  
  return iSize;
}
