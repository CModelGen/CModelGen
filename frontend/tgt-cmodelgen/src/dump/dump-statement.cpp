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

int clsDump::DumpLValue(clsLValue * pLValue, int onlyCalculateSize)
{
  int          iSize = 0;

  iSize += DumpLine("----- LVALUE BEG -----\n", onlyCalculateSize);
  _Assert(pLValue->m_pSignal && "Frontend Error: Each lvalue should have a signal.");
  iSize += DumpLine("\tSignal UID %u\n", pLValue->m_pSignal->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tWidth %u\n", pLValue->m_iWidth, onlyCalculateSize);
  if (pLValue->m_pOffsetExpression)
  {
    iSize += DumpLine("\tHas Offset Expression %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tOffset Expression UID %u\n", pLValue->m_pOffsetExpression->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Offset Expression %u\n", false, onlyCalculateSize);
  }
  if (pLValue->m_pIndexExpression)
  {
    iSize += DumpLine("\tHas Index Expression %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tIndex Expression UID %u\n", pLValue->m_pIndexExpression->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Index Expression %u\n", false, onlyCalculateSize);
  }
  iSize += DumpLine("----- LVALUE END -----\n", onlyCalculateSize);

  return iSize;
}

int clsDump::DumpStatement(clsStatement * pStatement, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- STATEMENT BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tStatement Type %u\n", pStatement->m_Type, onlyCalculateSize);
  iSize += DumpString("\tName\n", pStatement->m_sStatementName, onlyCalculateSize);
  iSize += DumpLocation(pStatement->m_Location, onlyCalculateSize);
  _Assert(pStatement->m_pBasicBlock && "Frontend Error: Each statement should belong to a basicblock.");
  iSize += DumpLine("\tBasicBlock UID %u\n", pStatement->m_pBasicBlock->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of LValues %u\n", pStatement->m_pLValues.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pStatement->m_pLValues.size(); ++iIndex)
  {
    iSize += DumpLValue(pStatement->m_pLValues[iIndex], onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Expressions %u\n", pStatement->m_pExpressions.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pStatement->m_pExpressions.size(); ++iIndex)
  {
    iSize += DumpLine("\tExpression UID %u\n", pStatement->m_pExpressions[iIndex]->m_UID, onlyCalculateSize);
  }
  if (pStatement->m_Type == CMODELGEN_STATEMENT_UTASK || pStatement->m_Type == CMODELGEN_STATEMENT_DISABLE ||
      pStatement->m_Type == CMODELGEN_STATEMENT_ALLOC || pStatement->m_Type == CMODELGEN_STATEMENT_FREE)
  {
    _Assert(pStatement->m_pCallScope && "Frontend Error: Each utask, disable, alloc, or free statement should have a call scope.");
    iSize += DumpLine("\tCall Scope UID %u\n", pStatement->m_pCallScope->m_UID, onlyCalculateSize);
  }
  if (pStatement->m_Type == CMODELGEN_STATEMENT_WAIT)
  {
    ERROR_WaitFound(pStatement->m_Location.m_sFile, pStatement->m_Location.m_iLine);
    _Assert(pStatement->m_pEvent && "Frontend Error: Each wait statement should have an event.");
    iSize += DumpLine("\tEvent UID %u\n", pStatement->m_pEvent->m_UID, onlyCalculateSize);
  }
  if (pStatement->m_Type == CMODELGEN_STATEMENT_COND)
  {
    iSize += DumpLine("\tHas Cond True %u\n", pStatement->m_HasCondTrue, onlyCalculateSize);
    iSize += DumpLine("\tHas Cond False = %u\n", pStatement->m_HasCondFalse, onlyCalculateSize);
  }
  iSize += DumpLine("----- STATEMENT END -----\n", onlyCalculateSize);

  return iSize;
}

int clsDump::DumpBasicBlock(clsBasicBlock * pBasicBlock, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- BASICBLOCK BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pBasicBlock->m_UID, onlyCalculateSize);
  iSize += DumpLine("\tNumber of Statements %u\n", pBasicBlock->m_pStatements.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pBasicBlock->m_pStatements.size(); ++iIndex)
  {
    iSize += DumpStatement(pBasicBlock->m_pStatements[iIndex], onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Successors %u\n", pBasicBlock->m_pSuccessors.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pBasicBlock->m_pSuccessors.size(); ++iIndex)
  {
    if (pBasicBlock->m_pSuccessors[iIndex] != NULL)
    {
      iSize += DumpLine("\tHas Non-Empty Successor %u\n", true, onlyCalculateSize);
      iSize += DumpLine("\tSuccessor BasicBlock UID %u\n", pBasicBlock->m_pSuccessors[iIndex]->m_UID, onlyCalculateSize);
    }
    else
    {
      iSize += DumpLine("\tHas No Non-Empty Successor %u\n", false, onlyCalculateSize);
    }
  }
  if (pBasicBlock->m_pEvent)
  {
    iSize += DumpLine("\tHas Event %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tEvent UID %u\n", pBasicBlock->m_pEvent->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Event %u\n", false, onlyCalculateSize);
  }
  _Assert(pBasicBlock->m_pScope && "Frontend Error: Each basicblock should belong to a scope.");
  iSize += DumpLine("\tScope UID %u\n", pBasicBlock->m_pScope->m_UID, onlyCalculateSize);
  if (pBasicBlock->m_pBlockScope)
  {
    iSize += DumpLine("\tHas Block Scope %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tBlock Scope UID %u\n", pBasicBlock->m_pBlockScope->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Block Scope %u\n", false, onlyCalculateSize);
  }
  if (pBasicBlock->m_pBlockEndScope)
  {
    iSize += DumpLine("\tHas Block End Scope %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tBlock End Scope UID %u\n", pBasicBlock->m_pBlockEndScope->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Block End Scope %u\n", false, onlyCalculateSize);
  }
  iSize += DumpLine("----- BASICBLOCK END -----\n", onlyCalculateSize);

  return iSize;
}
