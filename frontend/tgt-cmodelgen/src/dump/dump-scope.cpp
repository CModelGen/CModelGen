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

int          clsDump::DumpScope(clsScope * pScope, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- SCOPE BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pScope->m_UID, onlyCalculateSize);
  iSize += DumpLocation(pScope->m_Location, onlyCalculateSize);
  _Assert(pScope->m_pParentScope && "Frontend Error: All scopes should have a parent.");
  iSize += DumpLine("\tParent Scope UID %u\n", pScope->m_pParentScope->m_UID, onlyCalculateSize);
  _Assert(pScope->m_pRootScope && "Frontend Error: All scopes should have a root.");
  iSize += DumpLine("\tRoot Scope UID %u\n", pScope->m_pRootScope->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Children Scopes %u\n", pScope->m_pChildrenScopes.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pChildrenScopes.size(); ++iIndex)
  {
    iSize += DumpLine("\tChild Scope UID %u\n", pScope->m_pChildrenScopes[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Signals %u\n", pScope->m_pSignals.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pSignals.size(); ++iIndex)
  {
    iSize += DumpLine("\tSignal UID %u\n", pScope->m_pSignals[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Arguments %u\n", pScope->m_pArguments.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pArguments.size(); ++iIndex)
  {
    iSize += DumpLine("\tArgument UID %u\n", pScope->m_pArguments[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Parameters %u\n", pScope->m_pParameters.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pParameters.size(); ++iIndex)
  {
    iSize += DumpLine("\tParameter UID %u\n", pScope->m_pParameters[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Conditions %u\n", pScope->m_pConditions.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pConditions.size(); ++iIndex)
  {
    iSize += DumpLine("\tCondition UID %u\n", pScope->m_pConditions[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Events %u\n", pScope->m_pEvents.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScope->m_pEvents.size(); ++iIndex)
  {
    iSize += DumpLine("\tEvent UID %u\n", pScope->m_pEvents[iIndex]->m_UID, onlyCalculateSize);
  }
  if (pScope->m_pBasicBlock)
  {
    iSize += DumpLine("\tHas Basicblock %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tBasicblock UID %u\n", pScope->m_pBasicBlock->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Basicblock %u\n", false, onlyCalculateSize);
  }
  iSize += DumpString("\tScope Name\n", pScope->m_sScopeName, onlyCalculateSize);
  iSize += DumpLine("\tScope Type UID %u\n", pScope->m_pScopeType->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tScope is Root = %u\n", pScope->m_IsRoot, onlyCalculateSize);
  iSize += DumpLine("\tScope is Automatic = %u\n", pScope->m_IsAutomatic, onlyCalculateSize);
  iSize += DumpLine("----- SCOPE END -----\n", onlyCalculateSize);

  return iSize;
}

int          clsDump::DumpScopeType(clsScopeType * pScopeType, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- SCOPE TYPE BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pScopeType->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Friend Scopes %u\n", pScopeType->m_pFriendScopes.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pScopeType->m_pFriendScopes.size(); ++iIndex)
  {
    iSize += DumpLine("\tFriend Scope UID %u\n", pScopeType->m_pFriendScopes[iIndex]->m_UID, onlyCalculateSize);
  }
  iSize += DumpString("\tScope Type Name\n", pScopeType->m_sName, onlyCalculateSize);
  iSize += DumpLine("\tScope Type Type = %u\n", pScopeType->m_Type, onlyCalculateSize);
  iSize += DumpLine("\tPrototype Scope UID %u\n", pScopeType->m_pPrototypeScope->m_UID, onlyCalculateSize);
  iSize += DumpLine("----- SCOPE TYPE END -----\n", onlyCalculateSize);

  return iSize;
}
