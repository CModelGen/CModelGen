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

int          clsDump::DumpExpression(clsExpression * pExpression, int onlyCalculateSize)
{
  int          iSize = 0;
  unsigned int iIndex;

  iSize += DumpLine("----- EXPRESSION BEG -----\n", onlyCalculateSize);
  iSize += DumpLine("\tUID %u\n", pExpression->m_UID, onlyCalculateSize);
  iSize += DumpLocation(pExpression->m_Location, onlyCalculateSize);
  iSize += DumpLine("\tExpression Type = %u\n", pExpression->m_Type, onlyCalculateSize);
  if (pExpression->m_pDataType)
  {
    iSize += DumpLine("\tHas Data Type %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tData Type UID %u\n", pExpression->m_pDataType->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Data Type %u\n", false, onlyCalculateSize);
  }
  if (pExpression->m_pParameter)
  {
    iSize += DumpLine("\tHas Parameter %u\n", true, onlyCalculateSize);
    iSize += DumpLine("\tParameter UID %u\n", pExpression->m_pParameter->m_UID, onlyCalculateSize);
  }
  else
  {
    iSize += DumpLine("\tHas No Parameter %u\n", false, onlyCalculateSize);
  }
  iSize += DumpLine("\tNumber of Subexpressions %u\n", pExpression->m_pSubExpressions.size(), onlyCalculateSize);
  for (iIndex = 0; iIndex < pExpression->m_pSubExpressions.size(); ++iIndex)
  {
    iSize += DumpLine("\tSubexpression UID %u\n", pExpression->m_pSubExpressions[iIndex]->m_UID, onlyCalculateSize);
  }
  if (pExpression->m_Type == CMODELGEN_EXPRESSION_SIGNAL || pExpression->m_Type == CMODELGEN_EXPRESSION_ARRAY)
  {
    _Assert(pExpression->m_pSignal && "Frontend Error: Each signal expression should have a signal.");
    iSize += DumpLine("\tSignal UID %u\n", pExpression->m_pSignal->m_UID, onlyCalculateSize);
  }
  if (pExpression->m_Type == CMODELGEN_EXPRESSION_NUMBER)
  {
    _Assert(pExpression->m_pConst && "Frontend Error: Each number expression should have a const.");
    iSize += DumpLine("\tConst UID %u\n", pExpression->m_pConst->m_UID, onlyCalculateSize);
  }
  if (pExpression->m_Type == CMODELGEN_EXPRESSION_UNARY || pExpression->m_Type == CMODELGEN_EXPRESSION_BINARY)
  {
    iSize += DumpLine("\tOperator Type %u\n", pExpression->m_OperatorType, onlyCalculateSize);
  }
  if (pExpression->m_Type == CMODELGEN_EXPRESSION_FUNCTION)
  {
    _Assert(pExpression->m_pScope && "Frontend Error: Each function expression should have a scope.");
    iSize += DumpLine("\tScope UID %u\n", pExpression->m_pScope->m_UID, onlyCalculateSize);
  }
  if (pExpression->m_Type == CMODELGEN_EXPRESSION_SFUNCTION ||
      pExpression->m_Type == CMODELGEN_EXPRESSION_STRING)
  {
    _Assert(pExpression->m_sString && "Frontend Error: Each string expression should have a string.");
    iSize += DumpString("\tString\n", pExpression->m_sString, onlyCalculateSize);
  }
  iSize += DumpLine("----- EXPRESSION END -----\n", onlyCalculateSize);

  return iSize;
}
