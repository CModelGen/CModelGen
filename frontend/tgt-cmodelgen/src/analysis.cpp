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

int clsAnalysis::m_flagInnerGraphAcyclic = 0;
int clsAnalysis::m_flagOuterGraphAcyclic = 0;

void clsAnalysis::SignalAliasAnalysis()
{
  vector<clsNexus *>::iterator  it_pNexus;
  vector<clsSignal *>::iterator it_pSignal;
  int                           flagFoundModulePort;
  unsigned int                  iNumberOfSigPaths;
  unsigned int                  iIndex;
  
  for (it_pNexus = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    flagFoundModulePort = 0;
    for (it_pSignal = (*it_pNexus)->m_pSignals.begin();
         it_pSignal != (*it_pNexus)->m_pSignals.end();
         ++it_pSignal)
    {
      if ( (*it_pSignal)->m_pScope->m_IsRoot &&
           (*it_pSignal)->m_PortType != CMODELGEN_PORT_NONE)
      {
        _Assert(flagFoundModulePort == 0 &&
                "Frontend Error: More than one port of root modules are mapped to the same nexus.");
        flagFoundModulePort = 1;
        (*it_pNexus)->m_pSignal = *it_pSignal;
      }
      iNumberOfSigPaths = ivl_signal_npath( (*it_pSignal)->m_ivl_Signal);
      for (iIndex = 0; iIndex < iNumberOfSigPaths; ++iIndex)
      {
#ifdef CMODELGEN_DEBUG
        ivl_delaypath_t Obj = ivl_signal_path( (*it_pSignal)->m_ivl_Signal, iIndex);
        _DebugPrint("Signal %s has a delay path of (%u, %u, %u) that is unsupported.", (*it_pSignal)->m_sName,
                    ivl_path_is_condit(Obj), ivl_path_source_posedge(Obj), ivl_path_source_negedge(Obj) );
#endif
      }
    }
    if (flagFoundModulePort == 0)
    {
      (*it_pNexus)->m_pSignal = *( (*it_pNexus)->m_pSignals.begin() );
    }
  }
}

void clsAnalysis::EventDependenceAnalysis()
{
  vector<clsEvent *>::iterator     it_pEvent;
  vector<clsEvent *>::iterator     it_pEvent2;
  clsEvent *                       pEvent;
  clsEvent *                       pEvent2;
  
  EventProduceConsumeAnalysis();
  
  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    pEvent = *it_pEvent;
    for (it_pEvent2  = clsIr::m_pEvents.begin();
         it_pEvent2 != clsIr::m_pEvents.end();
         ++it_pEvent2)
    {
      pEvent2 = *it_pEvent2;
      if (pEvent == pEvent2)
      {
        continue;
      }
      if (pEvent->m_Type == CMODELGEN_EVENT_EVALUATION)
      {
      }
      else if (pEvent->m_Type == CMODELGEN_EVENT_LOGIC)
      {
      }
      else if (pEvent->m_Type == CMODELGEN_EVENT_NB_UPDATE)
      {
      }
    }
  }
}

clsScope * clsAnalysis::FindScopeRecursively(clsScope * pScope, ivl_scope_t ivl_Scope)
{
  vector<clsScope *>::iterator     it_pScope;
  clsScope *                       pRetScope;
  
  for (it_pScope  = pScope->m_pChildrenScopes.begin();
       it_pScope != pScope->m_pChildrenScopes.end();
       ++it_pScope)
  {
    if ( (*it_pScope)->m_ivl_Scope == ivl_Scope)
    {
      return *it_pScope;
    }
    pRetScope = FindScopeRecursively(*it_pScope, ivl_Scope);
    if (pRetScope)
    {
      return pRetScope;
    }
  }
  return NULL;
}

clsScope * clsAnalysis::FindScope(ivl_scope_t ivl_Scope)
{
  vector<clsScope *>::iterator     it_pScope;
  clsScope *                       pRetScope;
  
  for (it_pScope  = clsIr::m_pRootScopes.begin();
       it_pScope != clsIr::m_pRootScopes.end();
       ++it_pScope)
  {
    if ( (*it_pScope)->m_ivl_Scope == ivl_Scope)
    {
      return *it_pScope;
    }
    pRetScope = FindScopeRecursively(*it_pScope, ivl_Scope);
    if (pRetScope)
    {
      return pRetScope;
    }
  }
  _Assert(0 && "Frontend Error: A scope is not found.");
  return NULL;
}

clsNexus * clsAnalysis::FindNexus(ivl_nexus_t ivl_Nexus)
{
  vector<clsNexus *>::iterator     it_pNexus;
  
  _Assert(ivl_Nexus && "Frontend Error: An empty nexus is used.");
  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    if ( (*it_pNexus)->m_ivl_Nexus == ivl_Nexus)
    {
      return *it_pNexus;
    }
  }
  _Assert(0 && "Frontend Error: A nexus is not found.");
  return NULL;
}

clsParameter * clsAnalysis::FindParameter(ivl_parameter_t ivl_Parameter)
{
  vector<clsParameter *>::iterator     it_pParameter;

  for (it_pParameter  = clsIr::m_pParameters.begin();
       it_pParameter != clsIr::m_pParameters.end();
       ++it_pParameter)
  {
    if ( (*it_pParameter)->m_ivl_Parameter == ivl_Parameter)
    {
      return *it_pParameter;
    }
  }
  _Assert(0 && "Frontend Error: A parameter is not found.");
  return NULL;
}

clsSignal * clsAnalysis::FindSignalFromScope(ivl_signal_t ivl_Signal, unsigned int iIndex, clsScope * pScope)
{
  vector<clsScope *>::iterator  it_pScope;
  vector<clsSignal *>::iterator it_pSignal;
  clsSignal *                   pSignal;
  
  for (it_pSignal  = pScope->m_pSignals.begin();
       it_pSignal != pScope->m_pSignals.end();
       ++it_pSignal)
  {
    if ( (*it_pSignal)->m_ivl_Signal == ivl_Signal               &&
         ( ( (*it_pSignal)->m_VectorType == CMODELGEN_VECTOR_VECTOR &&
             (*it_pSignal)->m_iIndex == iIndex)                  ||
           (*it_pSignal)->m_VectorType != CMODELGEN_VECTOR_VECTOR
         )
       )
    {
      return *it_pSignal;
    }
  }
  for (it_pScope  = pScope->m_pChildrenScopes.begin();
       it_pScope != pScope->m_pChildrenScopes.end();
       ++it_pScope)
  {
    pSignal = FindSignalFromScope(ivl_Signal, iIndex, *it_pScope);
    if (NULL != pSignal)
    {
      return pSignal;
    }
  }
  return NULL;
}

clsSignal * clsAnalysis::FindSignal(ivl_signal_t ivl_Signal, unsigned int iIndex)
{
  vector<clsScope *>::iterator  it_pScope;
  clsSignal *                   pSignal;
  
  for (it_pScope  = clsIr::m_pRootScopes.begin();
       it_pScope != clsIr::m_pRootScopes.end();
       ++it_pScope)
  {
    pSignal = FindSignalFromScope(ivl_Signal, iIndex, *it_pScope);
    if (NULL != pSignal)
    {
      return pSignal;
    }
  }
  if (ivl_signal_file(ivl_Signal) )
  {
    _DebugPrint("In file: %s, line %u. Signal with index %u is not found.",
                ivl_signal_file(ivl_Signal), ivl_signal_lineno(ivl_Signal), iIndex);
  }
  _Assert(0 && "Frontend Error: A signal is not found.");
  return NULL;
}

void clsAnalysis::EventProduceConsumeAnalysis()
{
  vector<clsEvent *>::iterator     it_pEvent;
  clsEvent *                       pEvent;
  
  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    pEvent = *it_pEvent;
    if (pEvent->m_Type == CMODELGEN_EVENT_EVALUATION)
    {
      _Assert(pEvent->m_pBasicBlock && "Frontend Error: An evaluation event has no statement.");
      AnalyzeBasicBlock(pEvent->m_pBasicBlock, pEvent);
    }
    else if (pEvent->m_Type == CMODELGEN_EVENT_NB_UPDATE)
    {
    }
  }
}

void clsAnalysis::AnalyzeBasicBlock(clsBasicBlock * pBasicBlock, clsEvent * pEvent)
{
  vector<clsStatement *>::iterator  it_pStatement;
  
  for (it_pStatement  = pBasicBlock->m_pStatements.begin();
       it_pStatement != pBasicBlock->m_pStatements.end();
       ++it_pStatement)
  {
    AnalyzeStatement(*it_pStatement, pBasicBlock, pEvent);
  }
}

void clsAnalysis::AnalyzeStatement(clsStatement * pStatement, clsBasicBlock * pBasicBlock, clsEvent * pEvent)
{
  ivl_statement_t                   ivl_Statement;
  vector<clsLValue *>::iterator     it_pLValue;
  vector<clsBasicBlock *>::iterator it_pBasicBlock;
  unsigned int                      iNumberOfParameters;
  unsigned int                      iNumberOfStatements;
  unsigned int                      iIndex;
  
  ivl_Statement = pStatement->m_ivl_Statement;
  it_pBasicBlock = pBasicBlock->m_pSuccessors.begin();
  
  switch (pStatement->m_Type)
  {
  default:
    _DebugPrint("/* Unrecognized statement of type %u is encountered. */\n", ivl_statement_type(ivl_Statement) );
    _Assert(0 && "Frontend Error: Unrecognized statement is encountered.");
    break;
  case CMODELGEN_STATEMENT_NOP:
    break;
  case CMODELGEN_STATEMENT_WAIT:
    if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
    {
      AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
    }
    break;
  case CMODELGEN_STATEMENT_CASE:
  case CMODELGEN_STATEMENT_CASEZ:
  case CMODELGEN_STATEMENT_CASEX:
    AnalyzeExpression(pStatement->m_pExpressions[0], pEvent);
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      AnalyzeExpression(pStatement->m_pExpressions[iIndex + 1], pEvent);
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
      {
        AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
      }
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() )
      {
        ++it_pBasicBlock;
      }
    }
    if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
    {
      AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
    }
    break;
  case CMODELGEN_STATEMENT_COND:
    AnalyzeExpression(pStatement->m_pExpressions[0], pEvent);
    if (0 != ivl_stmt_cond_true(ivl_Statement) )
    {
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
      {
        AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
      }
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() )
      {
        ++it_pBasicBlock;
      }
    }
    if (0 != ivl_stmt_cond_false(ivl_Statement) )
    {
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
      {
        AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
      }
      if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() )
      {
        ++it_pBasicBlock;
      }
    }
    if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
    {
      AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
    }
    break;
  case CMODELGEN_STATEMENT_REPEAT:
  case CMODELGEN_STATEMENT_WHILE:
    AnalyzeExpression(pStatement->m_pExpressions[0], pEvent);
    if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
    {
      AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
    }
    if (it_pBasicBlock != pBasicBlock->m_pSuccessors.end() && *it_pBasicBlock != NULL)
    {
      AnalyzeBasicBlock(*it_pBasicBlock, pEvent);
    }
    break;
  case CMODELGEN_STATEMENT_NB_ASSIGN:
    iIndex = 0;
    for (it_pLValue  = pStatement->m_pLValues.begin();
         it_pLValue != pStatement->m_pLValues.end();
         ++it_pLValue)
    {
      AnalyzeNBLValue(*it_pLValue, pEvent);
      ++iIndex;
    }
    AnalyzeExpression(pStatement->m_pExpressions[0], pEvent);
    break;
  case CMODELGEN_STATEMENT_ASSIGN:
    iIndex = 0;
    for (it_pLValue  = pStatement->m_pLValues.begin();
         it_pLValue != pStatement->m_pLValues.end();
         ++it_pLValue)
    {
      AnalyzeLValue(*it_pLValue, pEvent);
      ++iIndex;
    }
    AnalyzeExpression(pStatement->m_pExpressions[0], pEvent);
    break;
  case CMODELGEN_STATEMENT_STASK:
    iNumberOfParameters = ivl_stmt_parm_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfParameters; ++iIndex)
    {
      AnalyzeExpression(pStatement->m_pExpressions[iIndex], pEvent);
    }
    break;
  case CMODELGEN_STATEMENT_UTASK:
    break;
  case CMODELGEN_STATEMENT_DISABLE:
    break;
  case CMODELGEN_STATEMENT_ALLOC:
    break;
  case CMODELGEN_STATEMENT_FREE:
    break;
  }
}

void clsAnalysis::AnalyzeExpression(clsExpression * pExpression, clsEvent * pEvent)
{
  ivl_expr_t                       ivl_Expression;
  clsSignal *                      pSignal;
  vector<clsDataType *>::iterator  it_pDataType;
  unsigned int                     iIndex;
  
  ivl_Expression = pExpression->m_ivl_Expression;
  
  switch (pExpression->m_Type)
  {
  default:
    _Assert(0 && "Frontend Error: Unrecognized expression is encountered.");
    break;
  case CMODELGEN_EXPRESSION_NONE:
    break;
  case CMODELGEN_EXPRESSION_CONCAT:
    for (iIndex = 0; iIndex < pExpression->m_pSubExpressions.size(); ++iIndex)
    {
      AnalyzeExpression(pExpression->m_pSubExpressions[iIndex], pEvent);
    }
    break;
  case CMODELGEN_EXPRESSION_UNARY:
    AnalyzeExpression(pExpression->m_pSubExpressions[0], pEvent);
    break;
  case CMODELGEN_EXPRESSION_BINARY:
    AnalyzeExpression(pExpression->m_pSubExpressions[0], pEvent);
    AnalyzeExpression(pExpression->m_pSubExpressions[1], pEvent);
    break;
  case CMODELGEN_EXPRESSION_CONDITIONAL:
    AnalyzeExpression(pExpression->m_pSubExpressions[0], pEvent);
    AnalyzeExpression(pExpression->m_pSubExpressions[1], pEvent);
    AnalyzeExpression(pExpression->m_pSubExpressions[2], pEvent);
    break;
  case CMODELGEN_EXPRESSION_SELECT:
    AnalyzeExpression(pExpression->m_pSubExpressions[0], pEvent);
    if (pExpression->m_pSubExpressions.size() == 2)
    {
      AnalyzeExpression(pExpression->m_pSubExpressions[1], pEvent);
    }
    break;
  case CMODELGEN_EXPRESSION_NUMBER:
    break;
  case CMODELGEN_EXPRESSION_SIGNAL:
    iIndex = 0;
    if (ivl_expr_oper1(ivl_Expression) != NULL                          &&
        number_is_immediate(ivl_expr_oper1(ivl_Expression), IMM_WID, 0) &&
        !number_is_unknown(ivl_expr_oper1(ivl_Expression) ) )
    {
	    iIndex = get_number_immediate(ivl_expr_oper1(ivl_Expression) );
    }
    pSignal = clsAnalysis::FindSignal(ivl_expr_signal(ivl_Expression), iIndex);
    if (pSignal->m_VectorType == CMODELGEN_VECTOR_VIRTUAL_VECTOR)
    {
      AnalyzeExpression(pExpression->m_pSubExpressions[0], pEvent);
    }
    break;
  case CMODELGEN_EXPRESSION_ARRAY:
    break;
  case CMODELGEN_EXPRESSION_STRING:
    break;
  case CMODELGEN_EXPRESSION_FUNCTION:
  case CMODELGEN_EXPRESSION_SFUNCTION:
    for (iIndex = 0; iIndex < pExpression->m_pSubExpressions.size(); ++iIndex)
    {
      AnalyzeExpression(pExpression->m_pSubExpressions[iIndex], pEvent);
    }
    break;
  }
}

void clsAnalysis::AnalyzeLValue(clsLValue * pLValue, clsEvent * pEvent)
{
  clsSignal *                   pSignal;
  
  pSignal = pLValue->m_pSignal;
  _Assert(pSignal->m_pNexus && "Frontend Error: A signal is not attached with a nexus.");
  
  if (pSignal->m_VectorType == CMODELGEN_VECTOR_VIRTUAL_VECTOR)
  {
    AnalyzeExpression(pLValue->m_pIndexExpression, pEvent);
  }
}

void clsAnalysis::AnalyzeNBLValue(clsLValue * pLValue, clsEvent * pEvent)
{
  clsSignal *                   pSignal;
  
  pSignal = pLValue->m_pSignal;
  _Assert(pSignal->m_pNexus && "Frontend Error: A signal is not attached with a nexus.");
  
  if (pSignal->m_VectorType == CMODELGEN_VECTOR_VIRTUAL_VECTOR)
  {
    AnalyzeExpression(pLValue->m_pIndexExpression, pEvent);
  }
}

void clsAnalysis::RegisterConditionsAndEvents()
{
  vector<clsCondition *>::iterator it_pCondition;
  vector<clsEvent *>::iterator     it_pEvent;
  clsCondition *                   pCondition;
  clsEvent *                       pEvent;
  
  for (it_pCondition  = clsIr::m_pConditions.begin();
       it_pCondition != clsIr::m_pConditions.end();
       ++it_pCondition)
  {
    pCondition = *it_pCondition;
    _Assert(pCondition->m_pNexus && "Frontend Error: A condition has no nexus.");
    // An event can only be sensitive to conditions defined in the current root scope (and its children).
    pCondition->m_pNexus->m_pSignal->m_pScope->m_pRootScope->m_pConditions.push_back(pCondition);
  }
  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    pEvent = *it_pEvent;
    pEvent->m_pScope->m_pRootScope->m_pEvents.push_back(pEvent);
  }
}
