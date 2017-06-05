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

//////////////////// class clsLocation ////////////////////
clsLocation::clsLocation()
{
  m_sFile = NULL;
  m_iLine = 0;
}

clsLocation::clsLocation(const char * sFile, unsigned int iLine)
{
  m_sFile = sFile;
  m_iLine = iLine;
}

clsLocation::~clsLocation()
{
}

//////////////////// class clsDataType ////////////////////
clsDataType::clsDataType()
{
  m_iWidth           = 0;
  m_IsSigned         = 0;
  m_UID               = 0;
  if (!clsIr::m_pDataTypes.empty() )
  {
    m_UID = clsIr::m_pDataTypes.back()->m_UID + 1;
  }
  clsIr::m_pDataTypes.push_back(this);
}

clsDataType::~clsDataType()
{
}

//////////////////// class clsSignal ////////////////////
clsSignal::clsSignal(ivl_signal_t ivl_Signal, ivl_nexus_t ivl_Nexus, enum typPort PortType)
{
  vector<clsNexus *>::iterator it_pNexus;

  Initialize(ivl_Signal);

  m_iIndex   = 0;
  m_PortType = PortType;

  if (NULL != ivl_Nexus)
  {
    m_VectorType = CMODELGEN_VECTOR_SCALAR;

    for (it_pNexus  = clsIr::m_pNexuses.begin();
         it_pNexus != clsIr::m_pNexuses.end();
         ++it_pNexus)
    {
      if ( (*it_pNexus)->m_ivl_Nexus == ivl_Nexus)
      {
        m_pNexus = *it_pNexus;
        m_pNexus->m_pSignals.push_back(this);
        return;
      }
    }
    m_pNexus = new clsNexus(ivl_Nexus);
    m_pNexus->m_pSignals.push_back(this);
  }
  else
  {
    m_VectorType = CMODELGEN_VECTOR_VIRTUAL_VECTOR;
    m_pNexus = new clsNexus(NULL);
    m_pNexus->m_pSignals.push_back(this);
  }
}

clsSignal::clsSignal(ivl_signal_t ivl_Signal, unsigned int iIndex, ivl_nexus_t ivl_Nexus, enum typPort PortType)
{
  vector<clsNexus *>::iterator it_pNexus;
  
  Initialize(ivl_Signal);
  
  m_iIndex     = iIndex;
  m_PortType   = PortType;
  m_VectorType = CMODELGEN_VECTOR_VECTOR;

  _Assert(NULL != ivl_Nexus && "Frontend Error: Vector signal cannot be initialized without a nexus.");

  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    if ( (*it_pNexus)->m_ivl_Nexus == ivl_Nexus)
    {
      m_pNexus = *it_pNexus;
      m_pNexus->m_pSignals.push_back(this);
      return;
    }
  }
  m_pNexus = new clsNexus(ivl_Nexus);
  m_pNexus->m_pSignals.push_back(this);
}

void clsSignal::Initialize(ivl_signal_t ivl_Signal)
{
  m_ivl_Signal        = ivl_Signal;
  m_UID               = 0;

  if (!clsIr::m_pSignals.empty() )
  {
    m_UID = clsIr::m_pSignals.back()->m_UID + 1;
  }
  clsIr::m_pSignals.push_back(this);
  
  m_pScope            = NULL;
  m_pHeadSignal       = this;
  m_iNumberOfAssign   = 0;
  m_iNumberOfNBAssign = 0;
  m_sName             = ivl_signal_basename(ivl_Signal);
  m_iSize             = ivl_signal_array_count(ivl_Signal);
  m_BaseAddress       = ivl_signal_array_base(ivl_Signal);
  m_AddressSwapped    = ivl_signal_array_addr_swapped(ivl_Signal);
  if (ivl_signal_file(ivl_Signal) )
  {
    m_Location = clsLocation(ivl_signal_file(ivl_Signal), ivl_signal_lineno(ivl_Signal) );
  }
  
  switch (ivl_signal_data_type(ivl_Signal) )
  {
  default:
    _Assert(0 && "Frontend Error: Unknown data type");
    break;
  case IVL_VT_REAL:
    ERROR_RealNumber(ivl_signal_file(ivl_Signal), ivl_signal_lineno(ivl_Signal) );
    break;
  case IVL_VT_BOOL:
  case IVL_VT_LOGIC:
  #ifdef CMODELGEN_USES_0_10
  case IVL_VT_STRING:
  #endif
    break;
  }
  
  switch (ivl_signal_type(ivl_Signal) )
  {
  default:
    _Assert(0 && "Frontend Error: Unknown signal type.");
    break;
  case IVL_SIT_REG:
    m_Type = CMODELGEN_SIGNAL_REG;
    break;
  #ifdef CMODELGEN_USES_0_10
  case IVL_SIT_UWIRE:
  #endif
  case IVL_SIT_TRI:
    m_Type = CMODELGEN_SIGNAL_TRI;
    break;
  case IVL_SIT_TRI0:
    ERROR_Tri0Tri1(ivl_signal_file(ivl_Signal), ivl_signal_lineno(ivl_Signal) );
    m_Type = CMODELGEN_SIGNAL_TRI0;
    break;
  case IVL_SIT_TRI1:
    ERROR_Tri0Tri1(ivl_signal_file(ivl_Signal), ivl_signal_lineno(ivl_Signal) );
    m_Type = CMODELGEN_SIGNAL_TRI1;
    break;
  case IVL_SIT_TRIAND:
    m_Type = CMODELGEN_SIGNAL_TRIAND;
    break;
  case IVL_SIT_TRIOR:
    m_Type = CMODELGEN_SIGNAL_TRIOR;
    break;
  }
  
  if (ivl_signal_local(ivl_Signal) )
  {
    m_Local = true;
  }
  else
  {
    m_Local = false;
  }

  if (ivl_signal_npath(ivl_Signal) )
  {
    _Assert(0 && "Frontend Error: Signals with delay paths are not supported.");
  }
}

clsSignal::~clsSignal()
{
}

//////////////////// class clsNexus ////////////////////
clsNexus::clsNexus(ivl_nexus_t ivl_Nexus)
{
  m_ivl_Nexus       = ivl_Nexus;
  m_pSignal         = NULL;
  m_pDataType       = NULL;
  m_Type            = CMODELGEN_SIGNAL_NONE;
  m_UID             = 0;
  if (!clsIr::m_pNexuses.empty() )
  {
    m_UID = clsIr::m_pNexuses.back()->m_UID + 1;
  }
  clsIr::m_pNexuses.push_back(this);
}

clsNexus::~clsNexus()
{
  m_pSignals.clear();
}

//////////////////// class clsLogic ////////////////////
clsLogic::clsLogic(ivl_net_logic_t ivl_Logic)
{
  m_ivl_Logic = ivl_Logic;
  m_UID       = 0;
  m_sName     = NULL;
  #ifdef CMODELGEN_USES_0_10
  // This code is only available in 0.10.x
  if (ivl_logic_file(ivl_Logic) )
  {
    m_Location = clsLocation(ivl_logic_file(ivl_Logic), ivl_logic_lineno(ivl_Logic) );
  }
  #endif
}

clsLogic::~clsLogic()
{
}

//////////////////// class clsLpm ////////////////////
clsLpm::clsLpm(ivl_lpm_t ivl_Lpm)
{
  m_ivl_Lpm = ivl_Lpm;
  m_UID     = 0;
  m_iBase   = 0;
  m_pDefine = NULL;
  if (ivl_lpm_file(ivl_Lpm) )
  {
    m_Location = clsLocation(ivl_lpm_file(ivl_Lpm), ivl_lpm_lineno(ivl_Lpm) );
  }
}

clsLpm::~clsLpm()
{
}

//////////////////// class clsConst ////////////////////
clsConst::clsConst(const char * sBits)
{
  m_sBits           = sBits;
  m_pDataType       = NULL;
  m_UID             = 0;
  if (!clsIr::m_pConsts.empty() )
  {
    m_UID = clsIr::m_pConsts.back()->m_UID + 1;
  }
  clsIr::m_pConsts.push_back(this);
}

clsConst::~clsConst()
{
}

//////////////////// class clsParameter ////////////////////
clsParameter::clsParameter(ivl_parameter_t ivl_Parameter)
{
  m_ivl_Parameter = ivl_Parameter;
  m_sName         = ivl_parameter_basename(ivl_Parameter);
  m_pScope        = NULL;
  m_pExpression   = NULL;
  if (ivl_parameter_file(ivl_Parameter) )
  {
    m_Location = clsLocation(ivl_parameter_file(ivl_Parameter), ivl_parameter_lineno(ivl_Parameter) );
  }
  m_UID             = 0;
  if (!clsIr::m_pParameters.empty() )
  {
    m_UID = clsIr::m_pParameters.back()->m_UID + 1;
  }
  clsIr::m_pParameters.push_back(this);
}

clsParameter::~clsParameter()
{
}

//////////////////// class clsLValue ////////////////////
clsLValue::clsLValue(ivl_lval_t ivl_LValue)
{
  m_ivl_LValue        = ivl_LValue;
  m_iWidth            = 0;
  m_pOffsetExpression = NULL;
  m_pIndexExpression  = NULL;
  m_pSignal           = NULL;
}

clsLValue::~clsLValue()
{
}

//////////////////// class clsScopeType ////////////////////
clsScopeType::clsScopeType(const char * sTypeName)
{
  m_sName           = sTypeName;
  m_Type            = CMODELGEN_SCOPE_NONE;
  m_pPrototypeScope = NULL;
  m_UID             = 0;

  if (!clsIr::m_pScopeTypes.empty() )
  {
    m_UID = clsIr::m_pScopeTypes.back()->m_UID + 1;
  }
  clsIr::m_pScopeTypes.push_back(this);
}

clsScopeType::~clsScopeType()
{
}

//////////////////// class clsScope ////////////////////
clsScope::clsScope(ivl_scope_t ivl_Scope)
{
  m_ivl_Scope    = ivl_Scope;
  m_pParentScope = NULL;
  m_pRootScope   = NULL;
  m_sScopeName   = NULL;
  m_pBasicBlock  = NULL;
  m_IsRoot       = false;
  m_IsAutomatic  = false;
  m_UID          = 0;
  m_pScopeType   = NULL;

  if (!clsIr::m_pScopes.empty() )
  {
    m_UID = clsIr::m_pScopes.back()->m_UID + 1;
  }
  clsIr::m_pScopes.push_back(this);
}

clsScope::~clsScope()
{
  vector<clsScope *>::iterator  it_pScope;
  vector<clsSignal *>::iterator it_pSignal;

  for (it_pScope  = m_pChildrenScopes.begin();
       it_pScope != m_pChildrenScopes.end();
       ++it_pScope)
  {
    delete *it_pScope;
  }
  m_pChildrenScopes.clear();

  for (it_pSignal  = m_pSignals.begin();
       it_pSignal != m_pSignals.end();
       ++it_pSignal)
  {
    delete *it_pSignal;
  }
  m_pSignals.clear();

  m_pConditions.clear();
}

//////////////////// class clsExpression ////////////////////
clsExpression::clsExpression(ivl_expr_t ivl_Expression)
{
  m_ivl_Expression = ivl_Expression;
  m_UID            = 0;
  m_pDataType      = NULL;
  m_pParameter     = NULL;
  m_Type           = CMODELGEN_EXPRESSION_NONE;
  m_pSignal        = NULL;
  m_pConst         = NULL;
  m_OperatorType   = CMODELGEN_OPERATOR_NONE;
  m_pScope         = NULL;
  m_sString        = NULL;
  
  if (ivl_Expression && ivl_expr_file(ivl_Expression) )
  {
    m_Location = clsLocation(ivl_expr_file(ivl_Expression), ivl_expr_lineno(ivl_Expression) );
  }
}

clsExpression::~clsExpression()
{
}

//////////////////// class clsStatement ////////////////////
clsStatement::clsStatement(ivl_statement_t ivl_Statement, clsBasicBlock * pBasicBlock)
{
  m_ivl_Statement    = ivl_Statement;
  m_pBasicBlock      = pBasicBlock;
  m_pEvent           = NULL;
  m_pCallScope       = NULL;
  m_sStatementName   = NULL;
  if (ivl_stmt_file(ivl_Statement) )
  {
    m_Location = clsLocation(ivl_stmt_file(ivl_Statement), ivl_stmt_lineno(ivl_Statement) );
  }
  if (ivl_statement_type(ivl_Statement) == IVL_ST_CONDIT)
  {
    m_HasCondTrue    = (ivl_stmt_cond_true(ivl_Statement) ) == NULL ? 0 : 1;
    m_HasCondFalse   = (ivl_stmt_cond_false(ivl_Statement) ) == NULL ? 0 : 1;
  }
  else
  {
    m_HasCondTrue    = 0;
    m_HasCondFalse   = 0;
  }
}

clsStatement::~clsStatement()
{
  vector<clsLValue *>::iterator  it_pLValue;
  for (it_pLValue  = m_pLValues.begin();
       it_pLValue != m_pLValues.end();
       ++it_pLValue)
  {
    if (*it_pLValue)
    {
      delete *it_pLValue;
    }
  }
}

//////////////////// class clsBasicBlock ////////////////////
clsBasicBlock::clsBasicBlock()
{
  m_UID            = 0;
  m_pEvent         = NULL;
  m_pScope         = NULL;
  m_pBlockScope    = NULL;
  m_pBlockEndScope = NULL;
}

clsBasicBlock::~clsBasicBlock()
{
}

//////////////////// class clsCondition ////////////////////
clsCondition::clsCondition()
{
  m_Type            = CMODELGEN_COND_ANY;
  m_pNexus          = NULL;
  m_UID             = 0;
  if (!clsIr::m_pConditions.empty() )
  {
    m_UID = clsIr::m_pConditions.back()->m_UID + 1;
  }
  clsIr::m_pConditions.push_back(this);
}

clsCondition::~clsCondition()
{
  m_pSensitiveEvents.clear();
}

//////////////////// class clsEvent ////////////////////
clsEvent::clsEvent()
{
  m_pScope      = NULL;
  m_Type        = CMODELGEN_EVENT_NONE;
  m_UID         = 0;
  m_pBasicBlock = NULL;
  m_pLValue     = NULL;
  m_pLogic      = NULL;
  m_pLpm        = NULL;
  if (!clsIr::m_pEvents.empty() )
  {
    m_UID = clsIr::m_pEvents.back()->m_UID + 1;
  }
  clsIr::m_pEvents.push_back(this);
}

clsEvent::~clsEvent()
{
  m_pConditions.clear();
}

//////////////////// class clsOptions ////////////////////

// The proper initialization values are assigned in FrontendMain.
int              clsOptions::DumpMode                     = 0;
