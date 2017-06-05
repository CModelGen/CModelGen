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

unsigned int glb_Warning = 0;

#if (defined CMODELGEN_LINUX) && (defined CMODELGEN_DEBUG)
static void SignalHandler(int iSig)
{
  // Trace depth.
  void *       pTrace[16];
  unsigned int iTraceSize = 0;
  unsigned int iIndex;
  char **      pStrings;

  // Load a backtrace of the stack.
  iTraceSize = backtrace(pTrace, 16);
  pStrings   = backtrace_symbols(pTrace, iTraceSize);
  if (iSig == SIGSEGV)
  {
    _DebugPrint("Segmentation fault!");
  }
  else if (iSig == SIGABRT)
  {
    _DebugPrint("Aborted!");
  }
  _DebugPrint("Obtained %u stack frames.", iTraceSize);

  for(iIndex = 0; iIndex < iTraceSize - 2; ++iIndex) 
  {
    _DebugPrint("%s:",pStrings[iIndex]);
  }
  exit(-1);
}
#endif

static unsigned int PassWarningMask(const char * sMask)
{
  unsigned int iIndex;
  unsigned int iRetVal;
  unsigned int iTemp;
  
  iRetVal = 0;
  if (strlen(sMask) != strlen("0xFFFFFFFF") ||
      sMask[0] != '0'                       ||
      sMask[1] != 'x')
  {
    ERROR_WarningMaskIllegal(sMask);
  }
  for (iIndex = 0; iIndex < 8; ++iIndex)
  {
    iTemp = 0;
    switch (sMask[iIndex + 2])
    {
    default:
      ERROR_WarningMaskIllegal(sMask);
      break;
    case 'F':
      iTemp++;
    case 'E':
      iTemp++;
    case 'D':
      iTemp++;
    case 'C':
      iTemp++;
    case 'B':
      iTemp++;
    case 'A':
      iTemp++;
    case '9':
      iTemp++;
    case '8':
      iTemp++;
    case '7':
      iTemp++;
    case '6':
      iTemp++;
    case '5':
      iTemp++;
    case '4':
      iTemp++;
    case '3':
      iTemp++;
    case '2':
      iTemp++;
    case '1':
      iTemp++;
    case '0':
      iRetVal += iTemp << ( (7 - iIndex) * 4);
      break;
    }
  }
  return iRetVal;
}

ivl_nexus_t cmodelgen_ivl_lpm_q(ivl_lpm_t net,
#ifdef CMODELGEN_USES_0_10
  unsigned idx __attribute__((unused))
#else
  unsigned idx
#endif
  )
{
  #ifdef CMODELGEN_USES_0_10
  return ivl_lpm_q(net);
  #else
  return ivl_lpm_q(net, idx);
  #endif
}

void FrontendMain(ivl_design_t * p_ivl_Design)
{
  ivl_scope_t * p_ivl_RootScopes;
  unsigned int  iNumberOfRoots;
  unsigned int  iIndex;
  
#ifdef CMODELGEN_LINUX
  _Debug(signal(SIGSEGV, SignalHandler) );
  _Debug(signal(SIGABRT, SignalHandler) );
#endif
  
  // Process options.
  glb_Warning = 0xFFFFFFFF;
  clsOptions::DumpMode = 2;
  
  if (ivl_design_flag(*p_ivl_Design, "-o") == NULL ||
      0 == strcmp("", ivl_design_flag(*p_ivl_Design, "-o") ) )
  {
    ERROR_IcarusNoOutputFile();
  }
  if (ivl_design_flag(*p_ivl_Design, "wmask") != NULL &&
      0 != strcmp("", ivl_design_flag(*p_ivl_Design, "wmask") ) )
  {
    glb_Warning = PassWarningMask(ivl_design_flag(*p_ivl_Design, "wmask") );
  }
  
  // IR generation and analysis.
  ivl_design_roots(*p_ivl_Design, &p_ivl_RootScopes, &iNumberOfRoots);
  for(iIndex = 0; iIndex < iNumberOfRoots; ++iIndex)
  {
    clsIr::IrGenScope(p_ivl_RootScopes[iIndex], NULL);
    clsIr::IrPreGenLogicEvents(p_ivl_RootScopes[iIndex], NULL);
  }
  clsAnalysis::SignalAliasAnalysis();
  clsIr::IrGenConstNexus();
  clsIr::IrGenScopeBasicBlocks();
  ivl_design_process(*p_ivl_Design, clsIr::IrGenProcess, NULL);
  clsIr::IrGenEvents();
  clsAnalysis::RegisterConditionsAndEvents();
  clsAnalysis::EventDependenceAnalysis();
  clsIr::ModifySignalTypes();
  
  //clsDebug::DebugDumpScopes();
  //clsDebug::DebugDumpEvents();
    
  // Output.
  if (clsOptions::DumpMode == 1)
  {
    clsDump::SetDumpMode(1);
    clsDump::Dump(ivl_design_flag(*p_ivl_Design, "-o") );
  }
  else if (clsOptions::DumpMode == 2)
  {
    clsDump::SetDumpMode(0);
    clsDump::Dump(ivl_design_flag(*p_ivl_Design, "-o") );
  }
  
  // Clean up.
  clsIr::IrClean();
  
  return;
}
