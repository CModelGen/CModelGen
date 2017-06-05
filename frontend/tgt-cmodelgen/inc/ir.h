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

#ifndef IR_H
#define IR_H

#include "common.h"
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

class clsLocation;
class clsDataType;
class clsSignal;
class clsNexus;
class clsLogic;
class clsLpm;
class clsConst;
class clsLValue;
class clsScopeType;
class clsScope;
class clsExpression;
class clsStatement;
class clsBasicBlock;
class clsCondition;
class clsEvent;
class clsProcess;

class clsIr;
class clsAnalysis;
class clsTransformation;
class clsOutput;

class clsOptions;

class clsLocation {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsLocation();
  clsLocation(const char * sFile, unsigned int iLine);
  ~clsLocation();
private:
  const char *            m_sFile;
  unsigned int            m_iLine;
};

class clsDataType {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsDataType();
  ~clsDataType();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  unsigned int            m_iWidth;
  int                     m_IsSigned;
};

class clsSignal {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsSignal(ivl_signal_t ivl_Signal, ivl_nexus_t ivl_Nexus, enum typPort);
  clsSignal(ivl_signal_t ivl_Signal, unsigned int iIndex, ivl_nexus_t ivl_Nexus, enum typPort);
  ~clsSignal();
  void                    Initialize(ivl_signal_t ivl_Signal);
  clsNexus *              GetNexus(unsigned int iIndex);
  unsigned int            GetUID() const {return m_UID;}
private:
  ivl_signal_t            m_ivl_Signal;
  unsigned int            m_UID;
  clsLocation             m_Location;
  enum typVector          m_VectorType;
  enum typSignal          m_Type;
  enum typPort            m_PortType;
  const char *            m_sName;
  unsigned int            m_iIndex;
  unsigned int            m_iSize;
  int                     m_BaseAddress;
  unsigned int            m_AddressSwapped;
  clsScope *              m_pScope;
  clsNexus *              m_pNexus;
  // A scalarized vector results in many signals generated. To link these signals,
  // each of them has a pointer points to the first of their kind. The first scalarized
  // signal has this pointer points to itself.
  clsSignal *             m_pHeadSignal;
  int                     m_Local;
  unsigned int            m_iNumberOfAssign;
  unsigned int            m_iNumberOfNBAssign;
};

class clsNexus {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDebug;
  friend class            clsDump;
  friend class            clsSignal;
public:
  clsNexus(ivl_nexus_t ivl_Nexus);
  ~clsNexus();
  unsigned int            GetUID() const {return m_UID;}
private:
  ivl_nexus_t             m_ivl_Nexus;
  unsigned int            m_UID;
  vector<clsSignal *>     m_pSignals;
  clsSignal *             m_pSignal;
  enum typSignal          m_Type;               // When the type is SUPPLY0 or SUPPLY1, it overwrites the signal's type.
  clsDataType *           m_pDataType;
  vector<clsCondition *>  m_pConditions;        // The conditions this nexus enables.
  vector<clsConst *>      m_pConsts;            // The magic constant devices connected to this nexus.
  vector<clsEvent *>      m_pLogicAndLpmEvents; // These are the logic and lpm events that update this nexus.
};

class clsLogic {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsLogic(ivl_net_logic_t ivl_Logic);
  ~clsLogic();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  enum typLogic           m_Type;
  const char *            m_sName;
  vector<clsNexus *>      m_pNexuses;
  clsLocation             m_Location;
  ivl_net_logic_t         m_ivl_Logic;
};

class clsLpm {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsLpm(ivl_lpm_t ivl_Lpm);
  ~clsLpm();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  enum typLpm             m_Type;
  vector<clsNexus *>      m_pNexuses;
  unsigned int            m_iBase;       // m_Type == CMODELGEN_LPM_PART*
  clsScope *              m_pDefine;     // m_Type == CMODELGEN_LPM_UFUNC
  clsLocation             m_Location;
  ivl_lpm_t               m_ivl_Lpm;
};

class clsConst {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsConst(const char * sBits);
  ~clsConst();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  const char *            m_sBits;
  clsDataType *           m_pDataType;
};

class clsParameter {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsParameter(ivl_parameter_t ivl_Parameter);
  ~clsParameter();
  unsigned int            GetUID() const {return m_UID;}
private:
  ivl_parameter_t         m_ivl_Parameter;
  unsigned int            m_UID;
  const char *            m_sName;
  clsScope *              m_pScope;
  clsExpression *         m_pExpression;
  clsLocation             m_Location;
};

class clsLValue {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsLValue(ivl_lval_t ivl_LValue);
  ~clsLValue();
private:
  ivl_lval_t              m_ivl_LValue;
  clsSignal *             m_pSignal;
  unsigned int            m_iWidth;
  clsExpression *         m_pOffsetExpression;
  clsExpression *         m_pIndexExpression;  // Only when the signal is a virtual vector.
};

class clsScopeType {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsScopeType(const char * sTypeName);
  ~clsScopeType();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  vector<clsScope *>      m_pFriendScopes;
  const char *            m_sName;
  typScope                m_Type;
  clsScope *              m_pPrototypeScope;
};

class clsScope {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsScope(ivl_scope_t ivl_Scope);
  ~clsScope();
  unsigned int            GetUID() const {return m_UID;}
private:
  ivl_scope_t             m_ivl_Scope;
  unsigned int            m_UID;
  clsLocation             m_Location;
  clsScope *              m_pParentScope;
  clsScope *              m_pRootScope;
  vector<clsScope *>      m_pChildrenScopes;
  vector<clsSignal *>     m_pSignals;      // m_pArguments are included.
  vector<clsSignal *>     m_pArguments;    // Only if the scope is CMODELGEN_SCOPE_FUNCTION.
  vector<clsParameter *>  m_pParameters;
  vector<clsCondition *>  m_pConditions;   // Only if the scope is a root.
  vector<clsEvent *>      m_pEvents;       // Only if the scope is a root.
  clsBasicBlock *         m_pBasicBlock;   // Only if the scope is CMODELGEN_SCOPE_FUNCTION or CMODELGEN_SCOPE_TASK.
  const char *            m_sScopeName;
  clsScopeType *          m_pScopeType;
  int                     m_IsRoot;
  int                     m_IsAutomatic;   // Only if the scope is CMODELGEN_SCOPE_FUNCTION or CMODELGEN_SCOPE_TASK.
};

class clsExpression {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsExpression(ivl_expr_t ivl_Expression);
  ~clsExpression();
  unsigned int            GetUID() const {return m_UID;}
private:
  ivl_expr_t              m_ivl_Expression;
  unsigned int            m_UID;
  clsLocation             m_Location;
  enum typExpression      m_Type;
  clsDataType *           m_pDataType;
  clsParameter *          m_pParameter;     // Optional
  vector<clsExpression *> m_pSubExpressions;
  clsSignal *             m_pSignal;        // Only for CMODELGEN_EXPRESSION_SIGNAL || CMODELGEN_EXPRESSION_ARRAY.
  clsConst *              m_pConst;         // Only for CMODELGEN_EXPRESSION_NUMBER.
  enum typOperator        m_OperatorType;   // Only for CMODELGEN_EXPRESSION_BINARY.
  clsScope *              m_pScope;         // Only for CMODELGEN_EXPRESSION_FUNCTION.
  const char *            m_sString;        // Only for CMODELGEN_EXPRESSION_STRING.
};

class clsStatement {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsStatement(ivl_statement_t ivl_Statement, clsBasicBlock * pBasicBlock);
  ~clsStatement();
private:
  ivl_statement_t         m_ivl_Statement;
  enum typStatement       m_Type;
  clsLocation             m_Location;
  clsBasicBlock *         m_pBasicBlock;
  vector<clsLValue *>     m_pLValues;
  vector<clsExpression *> m_pExpressions;
  const char *            m_sStatementName;
  clsScope *              m_pCallScope;   // Only for CMODELGEN_STATEMENT_UTASK and CMODELGEN_STATEMENT_DISABLE.
  clsEvent *              m_pEvent;       // Only for CMODELGEN_STATEMENT_WAIT.
  int                     m_HasCondTrue;  // Only for CMODELGEN_STATEMENT_COND.
  int                     m_HasCondFalse; // Only for CMODELGEN_STATEMENT_COND.
};

class clsBasicBlock {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsBasicBlock();
  ~clsBasicBlock();
  unsigned int            GetUID() const {return m_UID;}
private:
  vector<clsStatement *>  m_pStatements;
  vector<clsBasicBlock *> m_pSuccessors;
  unsigned int            m_UID;
  clsEvent *              m_pEvent;
  clsScope *              m_pScope;
  clsScope *              m_pBlockScope;
  clsScope *              m_pBlockEndScope;
};

class clsCondition {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsCondition();
  ~clsCondition();
  unsigned int            GetUID() const {return m_UID;}
private:
  unsigned int            m_UID;
  vector<clsEvent *>      m_pSensitiveEvents;
  enum typCond            m_Type;
  clsNexus *              m_pNexus;
};

class clsEvent {
  friend class            clsIr;
  friend class            clsAnalysis;
  friend class            clsTransformation;
  friend class            clsOutput;
  friend class            clsDump;
  friend class            clsDebug;
public:
  clsEvent();
  ~clsEvent();
  unsigned int            GetUID() const {return m_UID;}
private:
  vector<clsCondition *>  m_pConditions;
  clsScope *              m_pScope;
  enum typEvent           m_Type;
  unsigned int            m_UID;
  clsBasicBlock *         m_pBasicBlock; // m_Type == CMODELGEN_EVENT_EVALUATION || CMODELGEN_EVENT_SUBEVENT
  vector<clsEvent *>      m_pSubEvents;  // m_Type == CMODELGEN_EVENT_EVALUATION
  clsLValue *             m_pLValue;     // m_Type == CMODELGEN_EVENT_NB_UPDATE
  clsLogic *              m_pLogic;      // m_Type == CMODELGEN_EVENT_LOGIC
  clsLpm *                m_pLpm;        // m_Type == CMODELGEN_EVENT_LPM
};

class clsIr {
public:
  static int              IrGenScope(ivl_scope_t ivl_Scope, void * pArgs);
  static int              IrPreGenLogicEvents(ivl_scope_t ivl_Scope, void * pArgs);
  static int              IrGenProcess(ivl_process_t ivl_Process, void * pArgs);
  static void             IrGenScopeBasicBlocks();
  static void             IrGenScopeBasicBlock(clsScope * pScope);
  static void             IrGenEvents();
  static void             IrGenConstNexus();
  static void             IrClean();
  static clsDataType *    GenDataType(unsigned int iWidth, int IsSigned);
  static void             ModifySignalTypes();
private:
  static clsConst *       GenConst(const char * sBits, clsDataType * pDataType);
  static clsScopeType *   GenScopeType(clsScope * pScope, const char * sTypeName);
  static clsBasicBlock *  GenBasicBlock(ivl_statement_t ivl_Statement, clsScope * pScope, clsEvent * pEvent);
  static int              AddToBasicBlock(ivl_statement_t ivl_Statement, clsBasicBlock ** ppBasicBlock);
  static clsCondition *   GenCondition(enum typCond Type, clsNexus * pNexus);
  static void             GenEventConditions(clsEvent * pEvent, ivl_statement_t ivl_Statement);
  static void             GenUpdateEvents(clsBasicBlock * pBasicBlock);
  static void             ProcessStatement(clsStatement * pStatement);
  static clsExpression *  GenAndProcessExpression(ivl_expr_t ivl_Expression);
  static enum typOperator GenOperator(char Opcode, int IsUnary);
public:
  static vector<clsDataType *>   m_pDataTypes;
  static vector<clsConst *>      m_pConsts;
  static vector<clsNexus *>      m_pNexuses;
  static vector<clsParameter *>  m_pParameters;
  static vector<clsSignal *>     m_pSignals;
  static vector<clsScope *>      m_pScopes;
  static vector<clsScope *>      m_pRootScopes;
  static vector<clsScopeType *>  m_pScopeTypes;
  static vector<clsEvent *>      m_pEvents;
  static vector<clsCondition *>  m_pConditions;
  static vector<clsExpression *> m_pExpressions;
  static vector<clsBasicBlock *> m_pBasicBlocks;
  static vector<clsLogic *>      m_pLogics;
  static vector<clsLpm *>        m_pLpms;
};

class clsAnalysis {
public:
  static void             SignalAliasAnalysis();
  static void             EventDependenceAnalysis();
  static clsScope *       FindScope(ivl_scope_t ivl_Scope);
  static clsNexus *       FindNexus(ivl_nexus_t ivl_Nexus);
  static clsParameter *   FindParameter(ivl_parameter_t ivl_Parameter);
  static clsSignal *      FindSignal(ivl_signal_t ivl_Signal, unsigned int iIndex);
  static void             RegisterConditionsAndEvents();
private:
  static int              m_flagInnerGraphAcyclic;
  static int              m_flagOuterGraphAcyclic;
  static clsScope *       FindScopeRecursively(clsScope * pScope, ivl_scope_t ivl_Scope);
  static clsSignal *      FindSignalFromScope(ivl_signal_t ivl_Signal, unsigned int iIndex, clsScope * pScope);
  static void             EventProduceConsumeAnalysis();
  static void             AnalyzeBasicBlock(clsBasicBlock * pBasicBlock, clsEvent * pEvent);
  static void             AnalyzeStatement(clsStatement * pStatement, clsBasicBlock * pBasicBlock, clsEvent * pEvent);
  static void             AnalyzeExpression(clsExpression * pExpression, clsEvent * pEvent);
  static void             AnalyzeLValue(clsLValue * pLValue, clsEvent * pEvent);
  static void             AnalyzeNBLValue(clsLValue * pLValue, clsEvent * pEvent);
};

class clsDump {
  friend class            clsDebug;
public:
  static void             SetDumpMode(int isTextMode);
  static void             Dump(const char * sFileName);
private:
  // dump.cpp
  static int              Dump();
  static int              DumpLine(const char * sFormat, int onlyCalculateSize);
  static int              DumpLine(const char * sFormat, int iInteger, int onlyCalculateSize);
  static int              DumpLine(const char * sFormat, const char * sString, int onlyCalculateSize);
  static int              DumpByte(const char * sFormat, char iByte, int onlyCalculateSize);
  // dump-string.cpp
  static int              DumpString(const char * sAnnotation, const char * sString, int onlyCalculateSize);
  // dump-section.cpp
  static int              DumpSectionHeader(int iSectionID, int iSectionSize, int onlyCalculateSize);
  // dump-location.cpp
  static int              DumpLocation(clsLocation Location, int onlyCalculateSize);
  // dump-datatype.cpp
  static int              DumpDataType(clsDataType * pDataType, int onlyCalculateSize);
  // dump-scope.cpp
  static int              DumpScope(clsScope * pScope, int onlyCalculateSize);
  static int              DumpScopeType(clsScopeType * pScopeType, int onlyCalculateSize);
  // dump-signal.cpp
  static int              DumpSignal(clsSignal * pSignal, int onlyCalculateSize);
  static int              DumpNexus(clsNexus * pNexus, int onlyCalculateSize);
  static int              DumpParameter(clsParameter * pParameter, int onlyCalculateSize);
  // dump-logic.cpp
  static int              DumpLogic(clsLogic * pLogic, int onlyCalculateSize);
  // dump-lpm.cpp
  static int              DumpLpm(clsLpm * pLpm, int onlyCalculateSize);
  // dump-const.cpp
  static int              DumpConst(clsConst * pConst, int onlyCalculateSize);
  // dump-expression.cpp
  static int              DumpExpression(clsExpression * pExpression, int onlyCalculateSize);
  // dump-statement.cpp
  static int              DumpLValue(clsLValue * pLValue, int onlyCalculateSize);
  static int              DumpStatement(clsStatement * pStatement, int onlyCalculateSize);
  static int              DumpBasicBlock(clsBasicBlock * pBasicBlock, int onlyCalculateSize);
  // dump-event.cpp
  static int              DumpEvent(clsEvent * pEvent, int onlyCalculateSize);
  // dump-condition.cpp
  static int              DumpCondition(clsCondition * pCondition, int onlyCalculateSize);
  static int              m_isTextMode;
  static int              m_isDebugMode;
  static FILE *           m_pFile;
  static const char *     m_sFileName;
};

class clsOptions {
public:
  static int              DumpMode;
};

#endif // IR_H
