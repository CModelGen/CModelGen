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

vector<clsDataType *>   clsIr::m_pDataTypes;
vector<clsConst *>      clsIr::m_pConsts;
vector<clsNexus *>      clsIr::m_pNexuses;
vector<clsParameter *>  clsIr::m_pParameters;
vector<clsSignal *>     clsIr::m_pSignals;
vector<clsScope *>      clsIr::m_pScopes;
vector<clsScope *>      clsIr::m_pRootScopes;
vector<clsScopeType *>  clsIr::m_pScopeTypes;
vector<clsEvent *>      clsIr::m_pEvents;
vector<clsCondition *>  clsIr::m_pConditions;
vector<clsExpression *> clsIr::m_pExpressions;
vector<clsBasicBlock *> clsIr::m_pBasicBlocks;
vector<clsLogic *>      clsIr::m_pLogics;
vector<clsLpm *>        clsIr::m_pLpms;

// A functor to generate ir for a scope.
int clsIr::IrGenScope(ivl_scope_t ivl_Scope, void * pArgs)
{
  unsigned int                          iNumberOfSignals;
  unsigned int                          iNumberOfParameters;
  unsigned int                          iIndex;
  unsigned int                          iIndex2;
  ivl_signal_t                          ivl_Signal;
  clsScope *                            pScope;
  clsScope *                            pRootScope;
  clsSignal *                           pSignal;
  clsSignal *                           pHeadSignal;
  clsParameter *                        pParameter;
  enum typPort                          PortType;
  enum typScope                         ScopeType;
  int                                   SignalIsArgument;
  vector<clsSignal *>::iterator         it_pSignal;
  
  pScope = new clsScope(ivl_Scope);
  pScope->m_sScopeName = ivl_scope_basename(ivl_Scope);
  pScope->m_Location = clsLocation(ivl_scope_def_file(ivl_Scope), ivl_scope_def_lineno(ivl_Scope) );
  
  if (pArgs == NULL)
  {
    clsIr::m_pRootScopes.push_back(pScope);
    pScope->m_IsRoot = 1;
    pScope->m_pParentScope = pScope;
    pScope->m_pRootScope = pScope;
  }
  else
  {
    ( (clsScope *)pArgs)->m_pChildrenScopes.push_back(pScope);
    pRootScope = ( (clsScope *)pArgs)->m_pRootScope;
    pScope->m_pParentScope = (clsScope *)pArgs;
    pScope->m_pRootScope = pRootScope;
  }
  
  iNumberOfSignals = ivl_scope_sigs(ivl_Scope);
  for (iIndex = 0; iIndex < iNumberOfSignals; ++iIndex)
  {
    ivl_Signal = ivl_scope_sig(ivl_Scope, iIndex);
    SignalIsArgument = false;
    
    if (ivl_signal_port(ivl_Signal) != IVL_SIP_NONE)
    {
      if (ivl_signal_port(ivl_Signal) == IVL_SIP_INPUT)
      {
        PortType = CMODELGEN_PORT_IN;
      }
      else if (ivl_signal_port(ivl_Signal) == IVL_SIP_OUTPUT)
      {
        PortType = CMODELGEN_PORT_OUT;
      }
      else
      {
        PortType = CMODELGEN_PORT_INOUT;
      }
      
      if (ivl_scope_type(ivl_Scope) == IVL_SCT_FUNCTION ||
          ivl_scope_type(ivl_Scope) == IVL_SCT_TASK)
      {
        // For tasks, we use m_pArguments to store the arguments.
        // Note that output arguments for tasks are initialized to unknown
        // in the tasks.
        SignalIsArgument = true;
      }
    }
    else
    {
      PortType = CMODELGEN_PORT_NONE;
    }
    
    if (ivl_signal_array_count(ivl_Signal) > 1)
    {
      // An array is either virtualized or scalarized, which can be decided by
      // checking the first element of it.
      if (NULL != ivl_signal_nex(ivl_Signal, 0) )
      {
        pHeadSignal = NULL;
        for (iIndex2 = 0; iIndex2 < ivl_signal_array_count(ivl_Signal); ++iIndex2)
        {
          _Assert(NULL != ivl_signal_nex(ivl_Signal, iIndex2) &&
                  "Frontend Error: Partially virtualized array is not supported.");
          pSignal = new clsSignal(ivl_Signal, iIndex2, ivl_signal_nex(ivl_Signal, iIndex2), PortType);
          if (pHeadSignal == NULL)
          {
            pHeadSignal = pSignal;
          }
          if (pSignal != pHeadSignal)
          {
            pSignal->m_pHeadSignal = pHeadSignal;
          }
          if (pSignal->m_pNexus->m_pDataType == NULL)
          {
            pSignal->m_pNexus->m_pDataType = clsIr::GenDataType(ivl_signal_width(ivl_Signal), ivl_signal_signed(ivl_Signal) );
          }
          else
          {
            _Assert(pSignal->m_pNexus->m_pDataType == clsIr::GenDataType(ivl_signal_width(ivl_Signal), ivl_signal_signed(ivl_Signal) ) &&
                    "Frontend Error: The data types of connected signals are not the same.");
          }
          pScope->m_pSignals.push_back(pSignal);
          if (SignalIsArgument)
          {
            pScope->m_pArguments.push_back(pSignal);
          }
          pSignal->m_pScope = pScope;
        }
      }
      else
      {
        // A virtual vector.
        _Assert(ivl_signal_type(ivl_Signal) == IVL_SIT_REG &&
                ivl_signal_array_count(ivl_Signal) > 1     &&
                "Frontend Error: A virtualized array can only be a register array.");
        pSignal = new clsSignal(ivl_Signal, NULL, PortType);
        if (pSignal->m_pNexus->m_pDataType == NULL)
        {
          pSignal->m_pNexus->m_pDataType = clsIr::GenDataType(ivl_signal_width(ivl_Signal), ivl_signal_signed(ivl_Signal) );
        }
        else
        {
          _Assert(pSignal->m_pNexus->m_pDataType == clsIr::GenDataType(ivl_signal_width(ivl_Signal), ivl_signal_signed(ivl_Signal) ) &&
                  "Frontend Error: The data types of connected signals are not the same.");
        }
        pScope->m_pSignals.push_back(pSignal);
        if (SignalIsArgument)
        {
          pScope->m_pArguments.push_back(pSignal);
        }
        pSignal->m_pScope = pScope;
      }
    }
    else
    {
      _Assert(NULL != ivl_signal_nex(ivl_Signal, 0) &&
              "Frontend Error: A scalar signal cannot be virtualized.");
      pSignal = new clsSignal(ivl_Signal, ivl_signal_nex(ivl_Signal, 0), PortType);
      if (pSignal->m_pNexus->m_pDataType == NULL)
      {
        pSignal->m_pNexus->m_pDataType = clsIr::GenDataType(ivl_signal_width(ivl_Signal), ivl_signal_signed(ivl_Signal) );
      }
      else
      {
        _Assert(pSignal->m_pNexus->m_pDataType->m_iWidth == ivl_signal_width(ivl_Signal) &&
                "Frontend Error: The data types of connected signals are not of the same width.");
        if (!pSignal->m_pNexus->m_pDataType->m_IsSigned &&
            ivl_signal_signed(ivl_Signal) )
        {
          // If any of the signal on the nexus is signed, the signal is signed.
          pSignal->m_pNexus->m_pDataType = clsIr::GenDataType(ivl_signal_width(ivl_Signal), true);
        }
      }
      pScope->m_pSignals.push_back(pSignal);
      if (SignalIsArgument)
      {
        pScope->m_pArguments.push_back(pSignal);
      }
      pSignal->m_pScope = pScope;
    }
  }
  // Sort arguments and add argument types.
  if (ivl_scope_type(ivl_Scope) == IVL_SCT_FUNCTION ||
      ivl_scope_type(ivl_Scope) == IVL_SCT_TASK)
  {
    iNumberOfSignals = ivl_scope_ports(ivl_Scope);
    if (ivl_scope_type(ivl_Scope) == IVL_SCT_FUNCTION)
    {
      _Assert(iNumberOfSignals >= 1 &&
              "Frontend Error: There is at least a return value");
    }
    for (iIndex = 0; iIndex < iNumberOfSignals; ++iIndex)
    {
      _Assert(iNumberOfSignals == pScope->m_pArguments.size() &&
              "Frontend Error: Does not have the right number of arguments and return value.");
      ivl_Signal = ivl_scope_port(ivl_Scope, iIndex);
      for (it_pSignal  = pScope->m_pArguments.begin();
           it_pSignal != pScope->m_pArguments.end();
           ++it_pSignal)
      {
        if ( (*it_pSignal)->m_ivl_Signal == ivl_Signal)
        {
          break;
        }
      }
      _Assert(it_pSignal != pScope->m_pArguments.end() &&
              "Frontend Error: The signal is not found from the arguments and return value.");
      pSignal = *it_pSignal;
      pScope->m_pArguments.erase(it_pSignal);
      pScope->m_pArguments.push_back(pSignal);
    }
  }
  
  iNumberOfParameters = ivl_scope_params(ivl_Scope);
  for (iIndex = 0; iIndex < iNumberOfParameters; ++iIndex)
  {
    pParameter = new clsParameter(ivl_scope_param(ivl_Scope, iIndex) );
    pParameter->m_pScope = pScope;
    pScope->m_pParameters.push_back(pParameter);
    pParameter->m_pExpression = GenAndProcessExpression(ivl_parameter_expr(pParameter->m_ivl_Parameter) );
  }
  
  ivl_scope_children(ivl_Scope, IrGenScope, (void *)pScope);
  
  // To make sure the scope type of a child appears before its parents, the scope
  // types are generated in a post-order traverse.
  pScope->m_pScopeType = GenScopeType(pScope, ivl_scope_tname(ivl_Scope) );

  ScopeType = CMODELGEN_SCOPE_NONE;
  switch (ivl_scope_type(ivl_Scope) )
  {
  case IVL_SCT_MODULE:
    ScopeType = CMODELGEN_SCOPE_MODULE;
    break;
  case IVL_SCT_BEGIN:
    ScopeType = CMODELGEN_SCOPE_BEGIN;
    break;
  case IVL_SCT_FUNCTION:
    if (ivl_scope_is_auto(ivl_Scope) )
    {
      pScope->m_IsAutomatic = true;
    }
    ScopeType = CMODELGEN_SCOPE_FUNCTION;
    break;
  case IVL_SCT_TASK:
    if (ivl_scope_is_auto(ivl_Scope) )
    {
      pScope->m_IsAutomatic = true;
    }
    ScopeType = CMODELGEN_SCOPE_TASK;
    break;
  default:
    _DebugPrint("In file: %s, line %u. Scope type %u is not supported.",
                ivl_scope_def_file(ivl_Scope), ivl_scope_def_lineno(ivl_Scope), ivl_scope_type(ivl_Scope) );
    _Assert(0 && "Frontend Error: A scope type is not supported.");
    break;
  }
  if (pScope->m_pScopeType->m_Type == CMODELGEN_SCOPE_NONE)
  {
    pScope->m_pScopeType->m_Type = ScopeType;
  }
  else
  {
    _Assert(pScope->m_pScopeType->m_Type == ScopeType && "Frontend Error: A scope type conflict happens.");
  }
  
  if (pArgs != NULL)
  {
    if (find(pScope->m_pScopeType->m_pFriendScopes.begin(),
             pScope->m_pScopeType->m_pFriendScopes.end(),
             pRootScope) == pScope->m_pScopeType->m_pFriendScopes.end() )
    {
      pScope->m_pScopeType->m_pFriendScopes.push_back(pRootScope);
    }
  }

  return 0;
}

// Generate basic block for a scope.
void clsIr::IrGenScopeBasicBlock(clsScope * pScope)
{
  vector<clsScope *>::iterator     it_pScope;

  switch (pScope->m_pScopeType->m_Type)
  {
  default:
    break;
  case CMODELGEN_SCOPE_FUNCTION:
  case CMODELGEN_SCOPE_TASK:
    _Assert(pScope->m_pScopeType && "Frontend Error: Scope type is not identified.");
    _Assert(pScope->m_pScopeType->m_pPrototypeScope && "Frontend Error: Prototype scope is not connected.");
    if (pScope != pScope->m_pScopeType->m_pPrototypeScope)
    {
      #if 0
      // Only process a function or task when it is the prototype scope.
      return;
      #endif
    }
    pScope->m_pBasicBlock = GenBasicBlock(ivl_scope_def(pScope->m_ivl_Scope), pScope, NULL);
    break;
  }
  
  for (it_pScope  = pScope->m_pChildrenScopes.begin();
       it_pScope != pScope->m_pChildrenScopes.end();
       ++it_pScope)
  {
    IrGenScopeBasicBlock(*it_pScope);
  }
}

void clsIr::IrGenScopeBasicBlocks()
{
  vector<clsScope *>::iterator     it_pScope;

  for (it_pScope  = clsIr::m_pRootScopes.begin();
       it_pScope != clsIr::m_pRootScopes.end();
       ++it_pScope)
  {
    IrGenScopeBasicBlock(*it_pScope);
  }
}

// A functor to generate logic events for a scope.
int clsIr::IrPreGenLogicEvents(ivl_scope_t ivl_Scope, void * pArgs __attribute__((unused)) )
{
  unsigned int                          iNumberOfLpms;
  unsigned int                          iNumberOfLogs;
  unsigned int                          iNumberOfPins;
  unsigned int                          iIndex;
  unsigned int                          iIndex2;
  ivl_net_logic_t                       ivl_Logic;
  ivl_lpm_t                             ivl_Lpm;
  clsEvent *                            pEvent;
  clsNexus *                            pNexus;
  clsLogic *                            pLogic;
  clsLpm *                              pLpm;
  clsSignal *                           pSignal;

  iNumberOfLpms = ivl_scope_lpms(ivl_Scope);
  for (iIndex = 0; iIndex < iNumberOfLpms; ++iIndex)
  {
    ivl_Lpm = ivl_scope_lpm(ivl_Scope, iIndex);
    pEvent = new clsEvent;
    pEvent->m_Type = CMODELGEN_EVENT_LPM;
    pEvent->m_pScope = clsAnalysis::FindScope(ivl_Scope);
    pLpm = new clsLpm(ivl_Lpm);
    if (!clsIr::m_pLpms.empty() )
    {
      pLpm->m_UID = clsIr::m_pLpms.back()->m_UID + 1;
    }
    clsIr::m_pLpms.push_back(pLpm);
    pEvent->m_pLpm = pLpm;
    switch (ivl_lpm_type(ivl_Lpm) )
    {
    case IVL_LPM_ABS:       // VAMS
    case IVL_LPM_CAST_INT:  // From real.
    #ifdef CMODELGEN_USES_0_10
    case IVL_LPM_CAST_INT2: // SystemVerilog only.
    #endif
    case IVL_LPM_CAST_REAL: // To real.
    case IVL_LPM_FF:        // When flipflop is detected.
    case IVL_LPM_SFUNC:     // System function call shall not be supported.
    default:
      if (ivl_lpm_file(ivl_Lpm) )
      {
        _DebugPrint("At line %u of file %s\n", ivl_lpm_lineno(ivl_Lpm), ivl_lpm_file(ivl_Lpm) );
      }
      _DebugPrint("LPM type %u is not supported.", ivl_lpm_type(ivl_Lpm) );
      _DebugPrint("LPM name: %s\n", ivl_lpm_basename(ivl_Lpm) );
      _Assert(0 && "Frontend Error: A lpm type is not supported.");
      break;
    case IVL_LPM_REPEAT:
      // Produce an output vector by repeating the input vector a number of times.
      pLpm->m_Type = CMODELGEN_LPM_REPEAT;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 0) ) );
      _Assert(ivl_lpm_size(ivl_Lpm) * pLpm->m_pNexuses[1]->m_pDataType->m_iWidth == pLpm->m_pNexuses[0]->m_pDataType->m_iWidth &&
              "Frontend Error: The size of repeat LPM does not match its input and output.");
      break;
    case IVL_LPM_ADD:
    case IVL_LPM_SUB:
    case IVL_LPM_MULT:
    case IVL_LPM_DIVIDE:
    case IVL_LPM_MOD:
    case IVL_LPM_POW:
    case IVL_LPM_CMP_EEQ:
    case IVL_LPM_CMP_EQ:
    case IVL_LPM_CMP_GE:
    case IVL_LPM_CMP_GT:
    case IVL_LPM_CMP_NE:
    case IVL_LPM_CMP_NEE:
    case IVL_LPM_SHIFTL:
    case IVL_LPM_SHIFTR:
      if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_ADD)
      {
        pLpm->m_Type = CMODELGEN_LPM_ADD;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_SUB)
      {
        pLpm->m_Type = CMODELGEN_LPM_SUB;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_MULT)
      {
        pLpm->m_Type = CMODELGEN_LPM_MUL;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_DIVIDE)
      {
        pLpm->m_Type = CMODELGEN_LPM_DIV;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_MOD)
      {
        pLpm->m_Type = CMODELGEN_LPM_MOD;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_POW)
      {
        pLpm->m_Type = CMODELGEN_LPM_POW;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_EEQ)
      {
        pLpm->m_Type = CMODELGEN_LPM_TRI_EQ;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_NEE)
      {
        pLpm->m_Type = CMODELGEN_LPM_TRI_NE;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_EQ)
      {
        pLpm->m_Type = CMODELGEN_LPM_EQ;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_NE)
      {
        pLpm->m_Type = CMODELGEN_LPM_NE;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_GE)
      {
        pLpm->m_Type = CMODELGEN_LPM_GE;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_CMP_GT)
      {
        pLpm->m_Type = CMODELGEN_LPM_GT;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_SHIFTL)
      {
        pLpm->m_Type = CMODELGEN_LPM_SHL;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_SHIFTR)
      {
        pLpm->m_Type = CMODELGEN_LPM_SHR;
      }
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 1) ) );
      break;
    case IVL_LPM_RE_AND:
    case IVL_LPM_RE_NAND:
    case IVL_LPM_RE_NOR:
    case IVL_LPM_RE_OR:
    case IVL_LPM_RE_XNOR:
    case IVL_LPM_RE_XOR:
    case IVL_LPM_SIGN_EXT:
      if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_AND)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_AND;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_NAND)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_NAND;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_OR)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_OR;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_NOR)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_NOR;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_XOR)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_XOR;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_RE_XNOR)
      {
        pLpm->m_Type = CMODELGEN_LPM_RDCT_XNOR;
      }
      else if (ivl_lpm_type(ivl_Lpm) == IVL_LPM_SIGN_EXT)
      {
        pLpm->m_Type = CMODELGEN_LPM_SEXT;
      }
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 0) ) );
      break;
    case IVL_LPM_MUX:
      pLpm->m_Type = CMODELGEN_LPM_MUX;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_select(ivl_Lpm) ) );
      for (iIndex2 = 0; iIndex2 < ivl_lpm_size(ivl_Lpm); ++iIndex2)
      {
        pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, iIndex2) ) );
      }
      break;
    case IVL_LPM_PART_VP:
      pLpm->m_Type = CMODELGEN_LPM_PART_VP;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 0) ) );
      if (ivl_lpm_data(ivl_Lpm, 1) != NULL)
      {
        pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 1) ) );
      }
      else
      {
        pLpm->m_iBase = ivl_lpm_base(ivl_Lpm);
      }
      break;
    case IVL_LPM_PART_PV:
      pLpm->m_Type = CMODELGEN_LPM_PART_PV;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 0) ) );
      if (ivl_lpm_data(ivl_Lpm, 1) != NULL)
      {
        pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, 1) ) );
      }
      else
      {
        pLpm->m_iBase = ivl_lpm_base(ivl_Lpm);
      }
      break;
    case IVL_LPM_CONCAT:
    #ifdef CMODELGEN_USES_0_10
    case IVL_LPM_CONCATZ:
    #endif
      pLpm->m_Type = CMODELGEN_LPM_CONCAT;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      for (iIndex2 = 0; iIndex2 < ivl_lpm_size(ivl_Lpm); ++iIndex2)
      {
        pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, iIndex2) ) );
      }
      break;
    case IVL_LPM_ARRAY:
      pLpm->m_Type = CMODELGEN_LPM_ARRAY;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pSignal = clsAnalysis::FindSignal(ivl_lpm_array(ivl_Lpm), 0);
      _Assert(pSignal->m_pNexus && "Frontend Error: A signal has no nexus.");
      pLpm->m_pNexuses.push_back(pSignal->m_pNexus);
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_select(ivl_Lpm) ) );
      break;
    case IVL_LPM_UFUNC:
      pLpm->m_Type = CMODELGEN_LPM_UFUNC;
      pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(cmodelgen_ivl_lpm_q(ivl_Lpm, 0) ) );
      pLpm->m_pDefine = clsAnalysis::FindScope(ivl_lpm_define(ivl_Lpm) );
      for (iIndex2 = 0; iIndex2 < ivl_lpm_size(ivl_Lpm); ++iIndex2)
      {
        pLpm->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_lpm_data(ivl_Lpm, iIndex2) ) );
      }
      break;
    }
    pNexus = pLpm->m_pNexuses[0];
    pNexus->m_pLogicAndLpmEvents.push_back(pEvent);
  }
  iNumberOfLogs = ivl_scope_logs(ivl_Scope);
  for (iIndex = 0; iIndex < iNumberOfLogs; ++iIndex)
  {
    ivl_Logic = ivl_scope_log(ivl_Scope, iIndex);
    pEvent = new clsEvent;
    pEvent->m_pScope = clsAnalysis::FindScope(ivl_Scope);
    pEvent->m_Type = CMODELGEN_EVENT_LOGIC;
    pLogic = new clsLogic(ivl_Logic);
    if (!clsIr::m_pLogics.empty() )
    {
      pLogic->m_UID = clsIr::m_pLogics.back()->m_UID + 1;
    }
    clsIr::m_pLogics.push_back(pLogic);
    pEvent->m_pLogic = pLogic;
    pLogic->m_sName = ivl_logic_basename(ivl_Logic);
    iNumberOfPins = ivl_logic_pins(ivl_Logic);
    for (iIndex2 = 0; iIndex2 < iNumberOfPins; ++iIndex2)
    {
      pLogic->m_pNexuses.push_back(clsAnalysis::FindNexus(ivl_logic_pin(ivl_Logic, iIndex2) ) );
    }
    // Supported keywords:
    //   bufif0 bufif1 notif0 notif1
    //   and, nand, or, nor, xor, xnor
    //   buf, not
    switch (ivl_logic_type(ivl_Logic) )
    {
    default:
      _DebugPrint("Logic type %u is not supported.", ivl_logic_type(ivl_Logic) );
      _DebugPrint("Logic name: %s\n", ivl_logic_basename(ivl_Logic) );
      _Assert(0 && "Frontend Error: A logic type is not supported.");
      break;
    case IVL_LO_PULLUP:
    case IVL_LO_PULLDOWN:
      if (ivl_logic_type(ivl_Logic) == IVL_LO_PULLUP)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_PULLUP;
      }
      else
      {
        pLogic->m_Type = CMODELGEN_LOGIC_PULLDOWN;
      }
      _Assert(iNumberOfPins == 1 && "Frontend Error: Pullup/Pulldown should only have one output.");
      pNexus = pLogic->m_pNexuses[0];
      pNexus->m_pLogicAndLpmEvents.push_back(pEvent);
      // Change the nexus's type to supply0 or supply1 respectively.
      if (ivl_logic_type(ivl_Logic) == IVL_LO_PULLUP)
      {
        pNexus->m_Type = CMODELGEN_SIGNAL_SUPPLY1;
      }
      else
      {
        pNexus->m_Type = CMODELGEN_SIGNAL_SUPPLY0;
      }
      break;
    #ifdef CMODELGEN_USES_0_10
    case IVL_LO_BUFT:
    #endif
    case IVL_LO_BUFZ:
    case IVL_LO_BUF:
    case IVL_LO_NOT:
      if (ivl_logic_type(ivl_Logic) == IVL_LO_BUFZ
      #ifdef CMODELGEN_USES_0_10
          || ivl_logic_type(ivl_Logic) == IVL_LO_BUFT
      #endif
          )
      {
        pLogic->m_Type = CMODELGEN_LOGIC_BUFZ;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_BUF)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_BUF;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_NOT)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_NOT;
      }
      for (iIndex2 = 0; iIndex2 < iNumberOfPins - 1; ++iIndex2)
      {
        pNexus = pLogic->m_pNexuses[iIndex2];
        pNexus->m_pLogicAndLpmEvents.push_back(pEvent);
      }
      break;
    case IVL_LO_AND:
    case IVL_LO_NAND:
    case IVL_LO_OR:
    case IVL_LO_NOR:
    case IVL_LO_XOR:
    case IVL_LO_XNOR:
      if (ivl_logic_type(ivl_Logic) == IVL_LO_AND)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_AND;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_NAND)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_NAND;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_OR)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_OR;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_NOR)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_NOR;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_XOR)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_XOR;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_XNOR)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_XNOR;
      }
      pNexus = pLogic->m_pNexuses[0];
      pNexus->m_pLogicAndLpmEvents.push_back(pEvent);
      break;
    case IVL_LO_BUFIF0:
    case IVL_LO_BUFIF1:
    case IVL_LO_NOTIF0:
    case IVL_LO_NOTIF1:
      if (ivl_logic_type(ivl_Logic) == IVL_LO_BUFIF0)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_BUFIF0;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_BUFIF1)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_BUFIF1;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_NOTIF0)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_NOTIF0;
      }
      else if (ivl_logic_type(ivl_Logic) == IVL_LO_NOTIF1)
      {
        pLogic->m_Type = CMODELGEN_LOGIC_NOTIF1;
      }
      pNexus = pLogic->m_pNexuses[0];
      pNexus->m_pLogicAndLpmEvents.push_back(pEvent);
      break;
    }
  }

  ivl_scope_children(ivl_Scope, IrPreGenLogicEvents, NULL);

  return 0;
}

// A functor to generate IR for a process.
int clsIr::IrGenProcess(ivl_process_t ivl_Process, void * pArgs __attribute__((unused)) )
{
  clsEvent *           pEvent;

  switch(ivl_process_type(ivl_Process) )
  {
  default:
    _DebugPrint("In file: %s, line %u. Process type %u is not supported.",
                ivl_process_file(ivl_Process), ivl_process_lineno(ivl_Process), ivl_process_type(ivl_Process) );
    _Assert(0 && "Frontend Error: Unrecognized process is encountered.");
    break;
  case IVL_PR_ALWAYS:
    pEvent = new clsEvent;
    pEvent->m_pScope = clsAnalysis::FindScope(ivl_process_scope(ivl_Process) );
    
    if (ivl_statement_type(ivl_process_stmt(ivl_Process) ) == IVL_ST_WAIT)
    {
      pEvent->m_Type = CMODELGEN_EVENT_EVALUATION;
      pEvent->m_pBasicBlock = GenBasicBlock(ivl_stmt_sub_stmt(ivl_process_stmt(ivl_Process) ), pEvent->m_pScope, pEvent);
      GenEventConditions(pEvent, ivl_process_stmt(ivl_Process) );
    }
    else
    {
      _DebugPrint("In file: %s, line %u. Process statement type %u is not supported.",
                  ivl_process_file(ivl_Process), ivl_process_lineno(ivl_Process), ivl_process_type(ivl_Process) );
      _Assert(0 && "Frontend Error: Unrecognized process statement is encountered.");
    }
    
    GenUpdateEvents(pEvent->m_pBasicBlock);
    break;
  case IVL_PR_INITIAL:
    pEvent = new clsEvent;
    pEvent->m_pScope = clsAnalysis::FindScope(ivl_process_scope(ivl_Process) );
    
    pEvent->m_Type = CMODELGEN_EVENT_EVALUATION;
    pEvent->m_pBasicBlock = GenBasicBlock(ivl_process_stmt(ivl_Process), pEvent->m_pScope, pEvent);
    
    GenUpdateEvents(pEvent->m_pBasicBlock);
    break;
  }
  return 0;
}

void clsIr::IrGenEvents()
{
  vector<clsEvent *>::iterator   it_pEvent;
  clsEvent *                     pEvent;
  clsCondition *                 pCondition;
  ivl_net_logic_t                ivl_Logic;
  ivl_lpm_t                      ivl_Lpm;
  unsigned int                   iNumberOfPins;
  unsigned int                   iIndex;
  unsigned int                   iIndex2;

  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    pEvent = *it_pEvent;
    if (pEvent->m_Type == CMODELGEN_EVENT_LOGIC)
    {
      ivl_Logic = pEvent->m_pLogic->m_ivl_Logic;
      switch (ivl_logic_type(ivl_Logic) )
      {
      default:
        _DebugPrint("Logic type %u is not supported.", ivl_logic_type(ivl_Logic) );
        _DebugPrint("Logic name: %s\n", ivl_logic_basename(ivl_Logic) );
        _Assert(0 && "Frontend Error: A logic type is not supported.");
        break;
      case IVL_LO_PULLUP:
      case IVL_LO_PULLDOWN:
        iNumberOfPins = ivl_logic_pins(ivl_Logic);
        _Assert(iNumberOfPins == 1 && "Frontend Error: This logic should only have one output.");
        break;
      #ifdef CMODELGEN_USES_0_10
      case IVL_LO_BUFT:
      #endif
      case IVL_LO_BUFZ:
      case IVL_LO_BUF:
      case IVL_LO_NOT:
        iNumberOfPins = ivl_logic_pins(ivl_Logic);
        _Assert(iNumberOfPins >= 2 && "Frontend Error: A N-output logic has at least two pins.");
        pCondition = GenCondition(CMODELGEN_COND_ANY, pEvent->m_pLogic->m_pNexuses[iNumberOfPins - 1]);
        pEvent->m_pConditions.push_back(pCondition);
        pCondition->m_pSensitiveEvents.push_back(pEvent);
        break;
      case IVL_LO_AND:
      case IVL_LO_NAND:
      case IVL_LO_OR:
      case IVL_LO_NOR:
      case IVL_LO_XOR:
      case IVL_LO_XNOR:
        iNumberOfPins = ivl_logic_pins(ivl_Logic);
        _Assert(iNumberOfPins >= 3 && "Frontend Error: A N-input logic has at least three pins.");
        for (iIndex2 = 1; iIndex2 < iNumberOfPins; ++iIndex2)
        {
          pCondition = GenCondition(CMODELGEN_COND_ANY, pEvent->m_pLogic->m_pNexuses[iIndex2]);
          pEvent->m_pConditions.push_back(pCondition);
          pCondition->m_pSensitiveEvents.push_back(pEvent);
        }
        break;
      case IVL_LO_BUFIF0:
      case IVL_LO_BUFIF1:
      case IVL_LO_NOTIF0:
      case IVL_LO_NOTIF1:
        iNumberOfPins = ivl_logic_pins(ivl_Logic);
        _Assert(iNumberOfPins == 3 && "Frontend Error: An enable logic has exactly three pins.");
        for (iIndex2 = 1; iIndex2 < iNumberOfPins; ++iIndex2)
        {
          pCondition = GenCondition(CMODELGEN_COND_ANY, pEvent->m_pLogic->m_pNexuses[iIndex2]);
          pEvent->m_pConditions.push_back(pCondition);
          pCondition->m_pSensitiveEvents.push_back(pEvent);
        }
        break;
      }
    }
    else if (pEvent->m_Type == CMODELGEN_EVENT_LPM)
    {
      ivl_Lpm = pEvent->m_pLpm->m_ivl_Lpm;
      switch (ivl_lpm_type(ivl_Lpm) )
      {
      default:
        _DebugPrint("LPM type %u is not supported.", ivl_lpm_type(ivl_Lpm) );
        _DebugPrint("LPM name: %s\n", ivl_lpm_basename(ivl_Lpm) );
        _Assert(0 && "Frontend Error: A LPM type is not supported.");
        break;
      case IVL_LPM_REPEAT:
      case IVL_LPM_ADD:
      case IVL_LPM_SUB:
      case IVL_LPM_MULT:
      case IVL_LPM_DIVIDE:
      case IVL_LPM_MOD:
      case IVL_LPM_POW:
      case IVL_LPM_CMP_EEQ:
      case IVL_LPM_CMP_EQ:
      case IVL_LPM_CMP_GE:
      case IVL_LPM_CMP_GT:
      case IVL_LPM_CMP_NE:
      case IVL_LPM_CMP_NEE:
      case IVL_LPM_SHIFTL:
      case IVL_LPM_SHIFTR:
      case IVL_LPM_SIGN_EXT:
      case IVL_LPM_RE_AND:
      case IVL_LPM_RE_NAND:
      case IVL_LPM_RE_NOR:
      case IVL_LPM_RE_OR:
      case IVL_LPM_RE_XNOR:
      case IVL_LPM_RE_XOR:
      case IVL_LPM_MUX:
      case IVL_LPM_PART_VP:
      case IVL_LPM_PART_PV:
      case IVL_LPM_CONCAT:
      #ifdef CMODELGEN_USES_0_10
      case IVL_LPM_CONCATZ:
      #endif
      case IVL_LPM_ARRAY:
        for (iIndex = 1; iIndex < pEvent->m_pLpm->m_pNexuses.size(); ++iIndex)
        {
          pCondition = GenCondition(CMODELGEN_COND_ANY, pEvent->m_pLpm->m_pNexuses[iIndex]);
          pEvent->m_pConditions.push_back(pCondition);
          pCondition->m_pSensitiveEvents.push_back(pEvent);
        }
        break;
      case IVL_LPM_UFUNC: // TODO: ivl_lpm_trigger provides extra trigger events that might need be supported, or at least forbidden.
        for (iIndex = 1; iIndex < pEvent->m_pLpm->m_pNexuses.size(); ++iIndex)
        {
          pCondition = GenCondition(CMODELGEN_COND_ANY, pEvent->m_pLpm->m_pNexuses[iIndex]);
          pEvent->m_pConditions.push_back(pCondition);
          pCondition->m_pSensitiveEvents.push_back(pEvent);
        }
        break;
      }
    }
  }
}

void clsIr::IrGenConstNexus()
{
  vector<clsNexus *>::iterator     it_pNexus;
  ivl_net_const_t                  ivl_Const;
  unsigned int                     iIndex;
  
  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    if ( (*it_pNexus)->m_pSignal->m_VectorType != CMODELGEN_VECTOR_VIRTUAL_VECTOR)
    {
      for (iIndex = 0;
           iIndex < ivl_nexus_ptrs( (*it_pNexus)->m_ivl_Nexus);
           ++iIndex)
      {
        ivl_Const = ivl_nexus_ptr_con(ivl_nexus_ptr( (*it_pNexus)->m_ivl_Nexus, iIndex) );
        if (ivl_Const)
        {
          if (ivl_const_type(ivl_Const) == IVL_VT_LOGIC ||
              ivl_const_type(ivl_Const) == IVL_VT_BOOL)
          {
            (*it_pNexus)->m_pConsts.push_back(GenConst(ivl_const_bits(ivl_Const),
                                                       GenDataType(ivl_const_width(ivl_Const), ivl_const_signed(ivl_Const) ) ) );
          }
          else
          {
            _Assert(0 && "Frontend Error: Unsupported constant type.");
          }
        }
      }
    }
  }
}

void clsIr::IrClean()
{
  vector<clsScope *>::iterator     it_pScope;
  vector<clsNexus *>::iterator     it_pNexus;
  vector<clsEvent *>::iterator     it_pEvent;
  vector<clsParameter *>::iterator it_pParameter;
  vector<clsCondition *>::iterator it_pCondition;

  for (it_pScope  = clsIr::m_pRootScopes.begin();
       it_pScope != clsIr::m_pRootScopes.end();
       ++it_pScope)
  {
    delete *it_pScope;
  }
  clsIr::m_pRootScopes.clear();

  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    delete *it_pNexus;
  }
  clsIr::m_pNexuses.clear();

  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    delete *it_pEvent;
  }
  clsIr::m_pEvents.clear();

  for (it_pCondition  = clsIr::m_pConditions.begin();
       it_pCondition != clsIr::m_pConditions.end();
       ++it_pCondition)
  {
    delete *it_pCondition;
  }
  clsIr::m_pConditions.clear();

  for (it_pParameter  = clsIr::m_pParameters.begin();
       it_pParameter != clsIr::m_pParameters.end();
       ++it_pParameter)
  {
    delete *it_pParameter;
  }
  clsIr::m_pParameters.clear();
}

clsDataType * clsIr::GenDataType(unsigned int iWidth, int IsSigned)
{
  clsDataType *                   pDataType;
  vector<clsDataType *>::iterator it_pDataType;

  for (it_pDataType  = clsIr::m_pDataTypes.begin();
       it_pDataType != clsIr::m_pDataTypes.end();
       ++it_pDataType)
  {
    if ( (*it_pDataType)->m_iWidth == iWidth &&
         (*it_pDataType)->m_IsSigned == IsSigned
       )
    {
      return *it_pDataType;
    }
  }
  pDataType = new clsDataType();
  pDataType->m_iWidth = iWidth;
  pDataType->m_IsSigned = IsSigned;

  return pDataType;
}

clsConst * clsIr::GenConst(const char * sBits, clsDataType * pDataType)
{
  clsConst *                      pConst;
  vector<clsConst *>::iterator    it_pConst;

  for (it_pConst  = clsIr::m_pConsts.begin();
       it_pConst != clsIr::m_pConsts.end();
       ++it_pConst)
  {
    if ( (*it_pConst)->m_pDataType == pDataType)
    {
      if ( (*it_pConst)->m_sBits == sBits ||
           0 == memcmp( (*it_pConst)->m_sBits, sBits, pDataType->m_iWidth) )
      {
        return *it_pConst;
      }
    }
  }
  pConst = new clsConst(sBits);
  pConst->m_pDataType = pDataType;

  return pConst;
}

clsScopeType * clsIr::GenScopeType(clsScope * pScope, const char * sTypeName)
{
  clsScopeType *                   pScopeType;
  vector<clsScopeType *>::iterator it_pScopeType;

  if (pScope->m_pParameters.size() == 0)
  {
    for (it_pScopeType  = clsIr::m_pScopeTypes.begin();
         it_pScopeType != clsIr::m_pScopeTypes.end();
         ++it_pScopeType)
    {
      if ( (*it_pScopeType)->m_sName == sTypeName ||
           0 == strcmp( (*it_pScopeType)->m_sName, sTypeName)
         )
      {
        return *it_pScopeType;
      }
    }
  }
  pScopeType = new clsScopeType(sTypeName);
  pScopeType->m_pPrototypeScope = pScope;

  return pScopeType;
}

clsBasicBlock * clsIr::GenBasicBlock(ivl_statement_t ivl_Statement, clsScope * pScope, clsEvent * pEvent)
{
  clsBasicBlock *      pBasicBlock;
  clsBasicBlock *      pCurrentBasicBlock;
  int                  flagNewBasicBlock;

  pBasicBlock = new clsBasicBlock;
  pCurrentBasicBlock = pBasicBlock;
  pCurrentBasicBlock->m_pScope = pScope;
  pCurrentBasicBlock->m_pEvent = pEvent;
  if (!clsIr::m_pBasicBlocks.empty() )
  {
    pCurrentBasicBlock->m_UID = clsIr::m_pBasicBlocks.back()->m_UID + 1;
  }
  clsIr::m_pBasicBlocks.push_back(pCurrentBasicBlock);
  flagNewBasicBlock = AddToBasicBlock(ivl_Statement, &pCurrentBasicBlock);
  if (flagNewBasicBlock)
  {
    pCurrentBasicBlock->m_pSuccessors.push_back(NULL);
  }

  return pBasicBlock;
}

void clsIr::GenEventConditions(clsEvent * pEvent, ivl_statement_t ivl_Statement)
{
  clsCondition *                 pCondition;
  ivl_event_t                    ivl_Event;
  unsigned int                   iNumberOfEvents;
  unsigned int                   iNumberOfConditions;
  unsigned int                   iIndex;
  unsigned int                   iIndex2;
  
  iNumberOfEvents = ivl_stmt_nevent(ivl_Statement);
  for(iIndex = 0; iIndex < iNumberOfEvents; ++iIndex)
  {
    ivl_Event = ivl_stmt_events(ivl_Statement, iIndex);
    iNumberOfConditions = ivl_event_nany(ivl_Event);
    for (iIndex2 = 0; iIndex2 < iNumberOfConditions; ++iIndex2)
    {
      pCondition = GenCondition(CMODELGEN_COND_ANY, clsAnalysis::FindNexus(ivl_event_any(ivl_Event, iIndex2) ) );
      pEvent->m_pConditions.push_back(pCondition);
      pCondition->m_pSensitiveEvents.push_back(pEvent);
    }
    iNumberOfConditions = ivl_event_nneg(ivl_Event);
    for (iIndex2 = 0; iIndex2 < iNumberOfConditions; ++iIndex2)
    {
      pCondition = GenCondition(CMODELGEN_COND_NEGEDGE, clsAnalysis::FindNexus(ivl_event_neg(ivl_Event, iIndex2) ) );
      pEvent->m_pConditions.push_back(pCondition);
      pCondition->m_pSensitiveEvents.push_back(pEvent);
    }
    iNumberOfConditions = ivl_event_npos(ivl_Event);
    for (iIndex2 = 0; iIndex2 < iNumberOfConditions; ++iIndex2)
    {
      pCondition = GenCondition(CMODELGEN_COND_POSEDGE, clsAnalysis::FindNexus(ivl_event_pos(ivl_Event, iIndex2) ) );
      pEvent->m_pConditions.push_back(pCondition);
      pCondition->m_pSensitiveEvents.push_back(pEvent);
    }
  }
}

int clsIr::AddToBasicBlock(ivl_statement_t ivl_Statement, clsBasicBlock ** ppBasicBlock)
{
  clsBasicBlock *      pCurrentBasicBlock;
  clsBasicBlock *      pNewBasicBlock;
  ivl_statement_t      ivl_CurrentStatement;
  clsStatement *       pStatement;
  ivl_statement_type_t ivl_StatementType;
  clsScope *           pBlockScope;
  clsScope *           pBlockEndScope;
  unsigned int         iIndex;
  unsigned int         iNumberOfStatements;
  int                  flagNewBasicBlock;

  flagNewBasicBlock = false;
  pCurrentBasicBlock = *ppBasicBlock;
  ivl_StatementType = ivl_statement_type(ivl_Statement);

  switch (ivl_StatementType)
  {
  case IVL_ST_DELAY:
  case IVL_ST_DELAYX:
    WARNING_DelayFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    return AddToBasicBlock(ivl_stmt_sub_stmt(ivl_Statement), ppBasicBlock);
    break;
  default:
    if (ivl_stmt_file(ivl_Statement) )
    {
      _DebugPrint("In file: %s, line %u.",
                  ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    _DebugPrint("Statement type %u is not supported.", ivl_StatementType);
    _Assert(0 && "Frontend Error: Unrecognized statement is encountered.");
    break;
  case IVL_ST_NOOP:
    break;
  case IVL_ST_WAIT:
    if (IVL_ST_NOOP == ivl_statement_type(ivl_stmt_sub_stmt(ivl_Statement) ) )
    {
      ERROR_WaitFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    WARNING_WaitExpressionFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    return AddToBasicBlock(ivl_stmt_sub_stmt(ivl_Statement), ppBasicBlock);
    break;
  case IVL_ST_BLOCK:
    pBlockScope    = NULL;
    pBlockEndScope = NULL;
    if (ivl_stmt_block_scope(ivl_Statement) )
    {
      pBlockScope    = clsAnalysis::FindScope(ivl_stmt_block_scope(ivl_Statement) );
      pBlockEndScope = pBlockScope;
    }
    if (pBlockScope)
    {
      flagNewBasicBlock = true;
    }
    iNumberOfStatements = ivl_stmt_block_count(ivl_Statement);
    iIndex = 0;
    do
    {
      if (flagNewBasicBlock)
      {
        pNewBasicBlock = new clsBasicBlock;
        if (pBlockScope)
        {
          _Assert(pNewBasicBlock->m_pBlockScope == NULL &&
                  "Frontend Error: Block scope already has a value.");
          pNewBasicBlock->m_pBlockScope = pBlockScope;
          pBlockScope = NULL;
        }
        pNewBasicBlock->m_pScope = pCurrentBasicBlock->m_pScope;
        pNewBasicBlock->m_pEvent = pCurrentBasicBlock->m_pEvent;
        if (!clsIr::m_pBasicBlocks.empty() )
        {
          pNewBasicBlock->m_UID = clsIr::m_pBasicBlocks.back()->m_UID + 1;
        }
        clsIr::m_pBasicBlocks.push_back(pNewBasicBlock);
        pCurrentBasicBlock->m_pSuccessors.push_back(pNewBasicBlock);
        pCurrentBasicBlock = pNewBasicBlock;
        *ppBasicBlock = pCurrentBasicBlock;
      }
      if (iIndex < iNumberOfStatements)
      {
        ivl_CurrentStatement = ivl_stmt_block_stmt(ivl_Statement, iIndex);
        flagNewBasicBlock = AddToBasicBlock(ivl_CurrentStatement, &pCurrentBasicBlock);
        *ppBasicBlock = pCurrentBasicBlock;
        ++iIndex;
      }
      else
      {
        break;
      }
    } while (1);
    if (pBlockEndScope)
    {
      _Assert(pCurrentBasicBlock->m_pBlockEndScope == NULL &&
              "Frontend Error: Block end scope already has a value.");
      pCurrentBasicBlock->m_pBlockEndScope = pBlockEndScope;
      flagNewBasicBlock = 1;
    }
    return flagNewBasicBlock;
  case IVL_ST_CASE:
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_case_stmt(ivl_Statement, iIndex), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    }
    flagNewBasicBlock = 1;
    break;
  case IVL_ST_CASEZ:
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_case_stmt(ivl_Statement, iIndex), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    }
    flagNewBasicBlock = 1;
    break;
  case IVL_ST_CASEX:
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_case_stmt(ivl_Statement, iIndex), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    }
    flagNewBasicBlock = 1;
    break;  
  case IVL_ST_CONDIT:
    if (0 != ivl_stmt_cond_true(ivl_Statement) )
    {
      pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_cond_true(ivl_Statement), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    }
    if (0 != ivl_stmt_cond_false(ivl_Statement) )
    {
      pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_cond_false(ivl_Statement), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    }
    flagNewBasicBlock = 1;
    break;
  case IVL_ST_REPEAT:
  case IVL_ST_WHILE:
    pCurrentBasicBlock->m_pSuccessors.push_back(GenBasicBlock(ivl_stmt_sub_stmt(ivl_Statement), pCurrentBasicBlock->m_pScope, pCurrentBasicBlock->m_pEvent) );
    flagNewBasicBlock = 1;
    break;
  case IVL_ST_ASSIGN_NB:
    break;
  case IVL_ST_ASSIGN:
    break;
  case IVL_ST_STASK:
    break;
  case IVL_ST_UTASK:
    break;
  case IVL_ST_ALLOC:
    // Used for enabling automatic tasks.
    break;
  case IVL_ST_FREE:
    // Used for disabling automatic tasks.
    break;
  case IVL_ST_DISABLE:
    break;
  }
  pStatement = new clsStatement(ivl_Statement, pCurrentBasicBlock);
  ProcessStatement(pStatement);
  pCurrentBasicBlock->m_pStatements.push_back(pStatement);
  return flagNewBasicBlock;
}

clsCondition * clsIr::GenCondition(enum typCond Type, clsNexus * pNexus)
{
  vector<clsCondition *>::iterator it_pCondition;
  clsCondition *                   pCondition;

  switch (Type)
  {
  default:
    _DebugPrint("Unknown condition of type %u is encountered.", Type);
    _Assert(0 && "Frontend Error: Unknown condition is encountered.");
    break;
  case CMODELGEN_COND_ANY:
  case CMODELGEN_COND_POSEDGE:
  case CMODELGEN_COND_NEGEDGE:
  case CMODELGEN_COND_SIGNAL:
    for (it_pCondition  = clsIr::m_pConditions.begin();
         it_pCondition != clsIr::m_pConditions.end();
         ++it_pCondition)
    {
      if ( (*it_pCondition)->m_Type == Type &&
           (*it_pCondition)->m_pNexus == pNexus
         )
      {
        pCondition = *it_pCondition;
        return pCondition;
      }
    }
    break;
  }

  pCondition = new clsCondition();
  pCondition->m_Type = Type;
  pCondition->m_pNexus = pNexus;
  pNexus->m_pConditions.push_back(pCondition);

  return pCondition;
}

void clsIr::GenUpdateEvents(clsBasicBlock * pBasicBlock)
{
  vector<clsBasicBlock *>::iterator it_pSuccessor;
  vector<clsStatement *>::iterator  it_pStatement;
  vector<clsEvent *>::iterator      it_pEvent;
  clsEvent *                        pEvent;
  clsCondition *                    pCondition;
  unsigned int                      iIndex;
  unsigned int                      iNumberOfLValues;

  for (it_pStatement  = pBasicBlock->m_pStatements.begin();
       it_pStatement != pBasicBlock->m_pStatements.end();
       ++it_pStatement)
  {
    if (ivl_statement_type( (*it_pStatement)->m_ivl_Statement) == IVL_ST_ASSIGN_NB)
    {
      iNumberOfLValues = ivl_stmt_lvals( (*it_pStatement)->m_ivl_Statement);
      for (iIndex = 0; iIndex < iNumberOfLValues; ++iIndex)
      {
        pEvent = NULL;
        for (it_pEvent  = clsIr::m_pEvents.begin();
             it_pEvent != clsIr::m_pEvents.end();
             ++it_pEvent)
        {
          pEvent = *it_pEvent;
          if (pEvent->m_Type == CMODELGEN_EVENT_NB_UPDATE)
          {
            if (pEvent->m_pLValue == (*it_pStatement)->m_pLValues[iIndex] ||
                (pEvent->m_pLValue &&
                 pEvent->m_pLValue->m_pSignal == (*it_pStatement)->m_pLValues[iIndex]->m_pSignal) )
            {
              // Found an existing NB event.
              break;
            }
            else
            {
              pEvent = NULL;
            }
          }
          else
          {
            pEvent = NULL;
          }
        }
        if (pEvent == NULL)
        {
          pEvent = new clsEvent;
          pEvent->m_pScope = pBasicBlock->m_pScope;
          pEvent->m_Type = CMODELGEN_EVENT_NB_UPDATE;
          pEvent->m_pLValue = (*it_pStatement)->m_pLValues[iIndex];
        }
        pCondition = GenCondition(CMODELGEN_COND_SIGNAL,
                                  pEvent->m_pLValue->m_pSignal->m_pNexus);
        if (pEvent->m_pConditions.end() ==
            find(pEvent->m_pConditions.begin(),
                 pEvent->m_pConditions.end(),
                 pCondition) )
        {
          pEvent->m_pConditions.push_back(pCondition);
          pCondition->m_pSensitiveEvents.push_back(pEvent);
          pEvent->m_pLValue->m_pSignal->m_iNumberOfNBAssign++;
        }
      }
    }
    else if (ivl_statement_type( (*it_pStatement)->m_ivl_Statement) == IVL_ST_ASSIGN)
    {
      iNumberOfLValues = ivl_stmt_lvals( (*it_pStatement)->m_ivl_Statement);
      for(iIndex = 0; iIndex < iNumberOfLValues; ++iIndex)
      {
        (*it_pStatement)->m_pLValues[iIndex]->m_pSignal->m_iNumberOfAssign++;
      }
    }
  }

  for (it_pSuccessor  = pBasicBlock->m_pSuccessors.begin();
       it_pSuccessor != pBasicBlock->m_pSuccessors.end();
       ++it_pSuccessor)
  {
    if (*it_pSuccessor != NULL)
    {
      GenUpdateEvents(*it_pSuccessor);
    }
  }
}

void clsIr::ProcessStatement(clsStatement * pStatement)
{
  ivl_statement_t                   ivl_Statement;
  ivl_lval_t                        ivl_LValue;
  clsLValue *                       pLValue;
  unsigned int                      iNumberOfStatements;
  unsigned int                      iNumberOfLValues;
  unsigned int                      iNumberOfParameters;
  unsigned int                      iIndex;
  unsigned int                      iIndex2;

  ivl_Statement = pStatement->m_ivl_Statement;

  switch (ivl_statement_type(ivl_Statement) )
  {
  case IVL_ST_DELAY:
  case IVL_ST_DELAYX:
  case IVL_ST_WAIT:
  case IVL_ST_BLOCK:
  default:
    _DebugPrint("Unrecognized statement of type %u is encountered.", ivl_statement_type(ivl_Statement) );
    _Assert(0 && "Frontend Error: Unrecognized statement is encountered.");
    break;
  case IVL_ST_NOOP:
    pStatement->m_Type = CMODELGEN_STATEMENT_NOP;
    break;
  case IVL_ST_CASE:
    pStatement->m_Type = CMODELGEN_STATEMENT_CASE;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_case_expr(ivl_Statement, iIndex) ) );
    }
    break;
  case IVL_ST_CASEZ:
    pStatement->m_Type = CMODELGEN_STATEMENT_CASEZ;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_case_expr(ivl_Statement, iIndex) ) );
    }
    break;
  case IVL_ST_CASEX:
    pStatement->m_Type = CMODELGEN_STATEMENT_CASEX;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    iNumberOfStatements = ivl_stmt_case_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfStatements; ++iIndex)
    {
      pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_case_expr(ivl_Statement, iIndex) ) );
    }
    break;
  case IVL_ST_CONDIT:
    pStatement->m_Type = CMODELGEN_STATEMENT_COND;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    break;
  case IVL_ST_REPEAT:
    ERROR_RepeatFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    pStatement->m_Type = CMODELGEN_STATEMENT_REPEAT;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    break;
  case IVL_ST_WHILE:
    pStatement->m_Type = CMODELGEN_STATEMENT_WHILE;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_cond_expr(ivl_Statement) ) );
    break;
  case IVL_ST_ASSIGN_NB:
    if (ivl_stmt_nevent(ivl_Statement) )
    {
      WARNING_WaitExpressionFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    pStatement->m_Type = CMODELGEN_STATEMENT_NB_ASSIGN;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_rval(ivl_Statement) ) );
    if (ivl_stmt_delay_expr(ivl_Statement) )
    {
      WARNING_DelayExpressionFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    iNumberOfLValues = ivl_stmt_lvals(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfLValues; ++iIndex)
    {
      ivl_LValue = ivl_stmt_lval(ivl_Statement, iIndex);
      pLValue = new clsLValue(ivl_LValue);
      iIndex2 = 0;
      // Note: This property is also set but useless.
      // ivl_lval_sel_type(ivl_LValue)
      if (ivl_lval_idx(ivl_LValue) != NULL)
      {
        if (number_is_immediate(ivl_lval_idx(ivl_LValue), IMM_WID, 0) &&
            !number_is_unknown(ivl_lval_idx(ivl_LValue) ) )
        {
          iIndex2 = get_number_immediate(ivl_lval_idx(ivl_LValue) );
          pLValue->m_pIndexExpression = GenAndProcessExpression(ivl_lval_idx(ivl_LValue) );
        }
        else
        {
          pLValue->m_pIndexExpression = GenAndProcessExpression(ivl_lval_idx(ivl_LValue) );
        }
      }
      pLValue->m_pSignal = clsAnalysis::FindSignal(ivl_lval_sig(ivl_LValue), iIndex2);
      pLValue->m_iWidth = ivl_lval_width(ivl_LValue);
      if (ivl_lval_part_off(ivl_LValue) != NULL)
      {
        pLValue->m_pOffsetExpression = GenAndProcessExpression(ivl_lval_part_off(ivl_LValue) );
      }
      pStatement->m_pLValues.push_back(pLValue);
    }
    break;
  case IVL_ST_ASSIGN:
    pStatement->m_Type = CMODELGEN_STATEMENT_ASSIGN;
    pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_rval(ivl_Statement) ) );
    if (ivl_stmt_delay_expr(ivl_Statement) )
    {
      WARNING_DelayExpressionFound(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    iNumberOfLValues = ivl_stmt_lvals(ivl_Statement);
    #ifdef CMODELGEN_USES_0_10
    // This code is only available in 0.10.x
    if (ivl_stmt_opcode(ivl_Statement) != 0)
    {
      ERROR_UnsupportedStatement(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
    }
    #endif
    for(iIndex = 0; iIndex < iNumberOfLValues; ++iIndex)
    {
      ivl_LValue = ivl_stmt_lval(ivl_Statement, iIndex);
      pLValue = new clsLValue(ivl_LValue);
      iIndex2 = 0;
      if (ivl_lval_idx(ivl_LValue) != NULL)
      {
        if (number_is_immediate(ivl_lval_idx(ivl_LValue), IMM_WID, 0) &&
            !number_is_unknown(ivl_lval_idx(ivl_LValue) ) )
        {
          iIndex2 = get_number_immediate(ivl_lval_idx(ivl_LValue) );
          pLValue->m_pIndexExpression = GenAndProcessExpression(ivl_lval_idx(ivl_LValue) );
        }
        else
        {
          pLValue->m_pIndexExpression = GenAndProcessExpression(ivl_lval_idx(ivl_LValue) );
        }
      }
      pLValue->m_pSignal = clsAnalysis::FindSignal(ivl_lval_sig(ivl_LValue), iIndex2);
      pLValue->m_iWidth = ivl_lval_width(ivl_LValue);
      if (ivl_lval_part_off(ivl_LValue) != NULL)
      {
        pLValue->m_pOffsetExpression = GenAndProcessExpression(ivl_lval_part_off(ivl_LValue) );
      }
      pStatement->m_pLValues.push_back(pLValue);
    }
    break;
  case IVL_ST_STASK:
    pStatement->m_Type = CMODELGEN_STATEMENT_STASK;
    iNumberOfParameters = ivl_stmt_parm_count(ivl_Statement);
    for(iIndex = 0; iIndex < iNumberOfParameters; ++iIndex)
    {
      pStatement->m_pExpressions.push_back(GenAndProcessExpression(ivl_stmt_parm(ivl_Statement, iIndex) ) );
    }
    pStatement->m_sStatementName = ivl_stmt_name(ivl_Statement);
    break;
  case IVL_ST_UTASK:
    pStatement->m_Type = CMODELGEN_STATEMENT_UTASK;
    pStatement->m_pCallScope = clsAnalysis::FindScope(ivl_stmt_call(ivl_Statement) );
    break;
  case IVL_ST_ALLOC:
    // Used for enabling automatic tasks.
    pStatement->m_Type = CMODELGEN_STATEMENT_ALLOC;
    pStatement->m_pCallScope = clsAnalysis::FindScope(ivl_stmt_call(ivl_Statement) );
    break;
  case IVL_ST_FREE:
    // Used for disabling automatic tasks.
    pStatement->m_Type = CMODELGEN_STATEMENT_FREE;
    pStatement->m_pCallScope = clsAnalysis::FindScope(ivl_stmt_call(ivl_Statement) );
    break;
  case IVL_ST_DISABLE:
    pStatement->m_Type = CMODELGEN_STATEMENT_DISABLE;
    pStatement->m_pCallScope = clsAnalysis::FindScope(ivl_stmt_call(ivl_Statement) );
    break;
  }
}

clsExpression * clsIr::GenAndProcessExpression(ivl_expr_t ivl_Expression)
{
  clsExpression * pExpression;
  clsSignal *     pSignal;
  unsigned int    iIndex;
  unsigned int    iIndex1;
  
  pExpression = new clsExpression(ivl_Expression);

  if (ivl_Expression && NULL != ivl_expr_parameter(ivl_Expression) )
  {
    pExpression->m_pParameter = clsAnalysis::FindParameter(ivl_expr_parameter(ivl_Expression) );
  }

  switch (ivl_expr_type(ivl_Expression) )
  {
  case IVL_EX_REALNUM:
    ERROR_RealNumber(ivl_expr_file(ivl_Expression), ivl_expr_lineno(ivl_Expression) );
    break;
  case IVL_EX_BACCESS:
  case IVL_EX_DELAY:
  #ifdef CMODELGEN_USES_0_10
  case IVL_EX_ENUMTYPE:
  #endif
  case IVL_EX_EVENT:
  case IVL_EX_MEMORY:
  case IVL_EX_SCOPE:
  case IVL_EX_ULONG:
  default:
    _DebugPrint("Unrecognized expression of type %u is encountered.", ivl_expr_type(ivl_Expression) );
    _Assert(0 && "Frontend Error: Unrecognized expression is encountered.");
    break;
  case IVL_EX_NONE:
    pExpression->m_Type = CMODELGEN_EXPRESSION_NONE;
    pExpression->m_pDataType = GenDataType(0, 0);
    break;
  case IVL_EX_CONCAT:
    pExpression->m_Type = CMODELGEN_EXPRESSION_CONCAT;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    for (iIndex = 0; iIndex < ivl_expr_repeat(ivl_Expression); ++iIndex)
    {
      for (iIndex1 = 0; iIndex1 < ivl_expr_parms(ivl_Expression); ++iIndex1)
      {
        pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_parm(ivl_Expression, iIndex1) ) );
      }
    }
    break;
  case IVL_EX_UNARY:
    pExpression->m_Type = CMODELGEN_EXPRESSION_UNARY;
    // The data type generated from a unary expression has no high impendance.
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper1(ivl_Expression) ) );
    pExpression->m_OperatorType = GenOperator(ivl_expr_opcode(ivl_Expression), true);
    break;
  case IVL_EX_BINARY:
    pExpression->m_Type = CMODELGEN_EXPRESSION_BINARY;
    // The data type generated from a binary expression has no high impendance.
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper1(ivl_Expression) ) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper2(ivl_Expression) ) );
    pExpression->m_OperatorType = GenOperator(ivl_expr_opcode(ivl_Expression), false);
    break;
  case IVL_EX_TERNARY:
    pExpression->m_Type = CMODELGEN_EXPRESSION_CONDITIONAL;
    // The data type generated from a ternary expression (?:) may have high impendance.
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper1(ivl_Expression) ) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper2(ivl_Expression) ) );
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper3(ivl_Expression) ) );
    break;
  case IVL_EX_SELECT:
    pExpression->m_Type = CMODELGEN_EXPRESSION_SELECT;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    // Note: This property is also set but it is useless.
    // ivl_expr_sel_type(ivl_Expression);
    pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper1(ivl_Expression) ) );
    if (ivl_expr_oper2(ivl_Expression) )
    {
      pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper2(ivl_Expression) ) );
    }
    break;
  case IVL_EX_NUMBER:
    pExpression->m_Type = CMODELGEN_EXPRESSION_NUMBER;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_pConst = GenConst(ivl_expr_bits(ivl_Expression), pExpression->m_pDataType);
    break;
  case IVL_EX_SIGNAL:
    pExpression->m_Type = CMODELGEN_EXPRESSION_SIGNAL;
    if (ivl_expr_oper1(ivl_Expression) != NULL)
    {
      pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_oper1(ivl_Expression) ) );
    }
    iIndex = 0;
    if (ivl_expr_oper1(ivl_Expression) != NULL                          &&
        number_is_immediate(ivl_expr_oper1(ivl_Expression), IMM_WID, 0) &&
        !number_is_unknown(ivl_expr_oper1(ivl_Expression) ) )
    {
	    iIndex = get_number_immediate(ivl_expr_oper1(ivl_Expression) );
    }
    pSignal = clsAnalysis::FindSignal(ivl_expr_signal(ivl_Expression), iIndex);
    pExpression->m_pSignal = pSignal;
    pExpression->m_pDataType = pSignal->m_pNexus->m_pDataType;
    break;
  case IVL_EX_ARRAY:
    pExpression->m_Type = CMODELGEN_EXPRESSION_ARRAY;
    iIndex = 0;
    pSignal = clsAnalysis::FindSignal(ivl_expr_signal(ivl_Expression), iIndex);
    pExpression->m_pSignal = pSignal;
    pExpression->m_pDataType = pSignal->m_pNexus->m_pDataType;
    break;
  case IVL_EX_STRING:
    pExpression->m_Type = CMODELGEN_EXPRESSION_STRING;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_sString = ivl_expr_string(pExpression->m_ivl_Expression);
    break;
  case IVL_EX_UFUNC:
    pExpression->m_Type = CMODELGEN_EXPRESSION_FUNCTION;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_pScope = clsAnalysis::FindScope(ivl_expr_def(ivl_Expression) );
    for (iIndex1 = 0; iIndex1 < ivl_expr_parms(ivl_Expression); ++iIndex1)
    {
      pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_parm(ivl_Expression, iIndex1) ) );
    }
    break;
  case IVL_EX_SFUNC:
    pExpression->m_Type = CMODELGEN_EXPRESSION_SFUNCTION;
    pExpression->m_pDataType = GenDataType(ivl_expr_width(ivl_Expression), ivl_expr_signed(ivl_Expression) );
    pExpression->m_sString = ivl_expr_name(ivl_Expression);
    for (iIndex1 = 0; iIndex1 < ivl_expr_parms(ivl_Expression); ++iIndex1)
    {
      pExpression->m_pSubExpressions.push_back(GenAndProcessExpression(ivl_expr_parm(ivl_Expression, iIndex1) ) );
    }
    break;
  }
  if (!clsIr::m_pExpressions.empty() )
  {
    pExpression->m_UID = clsIr::m_pExpressions.back()->m_UID + 1;
  }
  clsIr::m_pExpressions.push_back(pExpression);
  return pExpression;
}

enum typOperator clsIr::GenOperator(char Opcode, int IsUnary)
{
  if (IsUnary)
  {
    switch (Opcode)
    {
    default:
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    case '&':
      return CMODELGEN_OPERATOR_AND;
      break;
    case '|':
      return CMODELGEN_OPERATOR_OR;
      break;
    case '^':
      return CMODELGEN_OPERATOR_XOR;
      break;
    case 'A': // ~&
      return CMODELGEN_OPERATOR_NAND;
      break;
    case 'N': // ~|
      return CMODELGEN_OPERATOR_NOR;
      break;
    case 'X': // ~^
      return CMODELGEN_OPERATOR_XNOR;
      break;
    case '-':
      return CMODELGEN_OPERATOR_SUB;
      break;
    case '~':
      return CMODELGEN_OPERATOR_NOT;
      break;
    case '!':
      return CMODELGEN_OPERATOR_LOGIC_NOT;
      break;
    case 'D': // pre-decrement
    case 'd': // post-decrement
    case 'I': // pre-increment
    case 'i': // post-increment
      _DebugPrint("Operators \"++\" and \"--\" are only allowed in SystemVerilog.");
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    case 'm': // abs // This is only generated if "abs" is used in Verilog-AMS.
      _DebugPrint("Verilog-AMS is not supported.");
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    case '2': // Cast to bool.
      _DebugPrint("Boolean is not supported.");
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    case 'v': // Real related.
    case 'r': // Real related.
      _DebugPrint("Real value is not supported.");
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    }
  }
  else
  {
    switch (Opcode)
    {
    default:
      _DebugPrint("Unrecognized opcode %c.", Opcode);
      _Assert(0 && "Frontend Error: Unrecoginized opcode is encountered.");
      break;
    case 'G': // >=
      return CMODELGEN_OPERATOR_GE;
      break;
    case '>':
      return CMODELGEN_OPERATOR_GT;
      break;
    case 'L': // <=
      return CMODELGEN_OPERATOR_LE;
      break;
    case '<':
      return CMODELGEN_OPERATOR_LT;
      break;
    case 'E': // ===
      return CMODELGEN_OPERATOR_CASE_EQ;
      break;
    case 'e': // ==
      return CMODELGEN_OPERATOR_EQ;
      break;
    case 'N': // !==
      return CMODELGEN_OPERATOR_CASE_NE;
      break;
    case 'n': // !=
      return CMODELGEN_OPERATOR_NE;
      break;
    case 'l': // <<
      return CMODELGEN_OPERATOR_SHL;
      break;
    case 'r': // >>
      return CMODELGEN_OPERATOR_SHR;
      break;
    case 'R': // >>> signed right shift
      return CMODELGEN_OPERATOR_ASHR;
      break;
    case '+':
      return CMODELGEN_OPERATOR_ADD;
      break;
    case '-':
      return CMODELGEN_OPERATOR_SUB;
      break;
    case '*':
      return CMODELGEN_OPERATOR_MUL;
      break;
    case '/':
      return CMODELGEN_OPERATOR_DIV;
      break;
    case '%':
      return CMODELGEN_OPERATOR_MOD;
      break;
    case 'p': // pow
      return CMODELGEN_OPERATOR_POW;
      break;
    case '&':
      return CMODELGEN_OPERATOR_AND;
      break;
    case '|':
      return CMODELGEN_OPERATOR_OR;
      break;
    case '^':
      return CMODELGEN_OPERATOR_XOR;
      break;
    case 'A': // ~&
      return CMODELGEN_OPERATOR_NAND;
      break;
    case 'O': // ~|
      return CMODELGEN_OPERATOR_NOR;
      break;
    case 'X': // ~^
      return CMODELGEN_OPERATOR_XNOR;
      break;
    case 'a': // &&
      return CMODELGEN_OPERATOR_LOGIC_AND;
      break;
    case 'o': // ||
      return CMODELGEN_OPERATOR_LOGIC_OR;
      break;
    }
  }
  _Assert(0 && "Frontend Error: Internal error.");
  return CMODELGEN_OPERATOR_NONE;
}

void clsIr::ModifySignalTypes()
{
  vector<clsNexus *>::iterator     it_pNexus;
  
  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    _Assert( (*it_pNexus)->m_pSignal && "Frontend Error: A nexus must has a signal.");
    if ( (*it_pNexus)->m_Type == CMODELGEN_SIGNAL_SUPPLY0 ||
         (*it_pNexus)->m_Type == CMODELGEN_SIGNAL_SUPPLY1)
    {
      (*it_pNexus)->m_pSignal->m_Type = (*it_pNexus)->m_Type;
    }
  }
}
