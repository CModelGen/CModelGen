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
#include "dump-identifier.h"

int          clsDump::m_isTextMode  = 0;
int          clsDump::m_isDebugMode = 0;
FILE *       clsDump::m_pFile       = stderr;
const char * clsDump::m_sFileName   = NULL;

void         clsDump::SetDumpMode(int isTextMode)
{
  m_isTextMode = isTextMode;
}

void         clsDump::Dump(const char * sFileName)
{
  char *                              sOutFile;
  FILE *                              pBackupFile;
  unsigned int                        BackupIndent;
  int                                 iDumpSize;

  _Assert(sFileName && "Frontend Error: Output file is not specified.");
  sOutFile = (char *) malloc(sizeof(char) * (strlen(sFileName) + 11) );
  m_sFileName = sFileName;

  strcpy(sOutFile, sFileName);
  if (m_isTextMode)
  {
    m_pFile = fopen(sOutFile, "w+");
  }
  else
  {
    m_pFile = fopen(sOutFile, "wb+");
  }
  if (m_pFile == NULL)
  {
    ERROR_FileCreationFailed(sOutFile);
    sOutFile = NULL;
    exit(1);
  }
  // This file should be unlinked so that it is deleted after this process
  // and the driver process that have already opened it end.
  // It has to be unlinked in the second (or later) process that opens it,
  // otherwise it would be deleted directly.
  unlink(sOutFile);

  pBackupFile = pOutFile;
  BackupIndent = gIndent;

  pOutFile = m_pFile;
  gIndent = 0;

  iDumpSize = Dump();

  if (!m_isTextMode)
  {
    if (iDumpSize != ftell(pOutFile) )
    {
      _DebugPrint("Expected dump file size is %u, actual dump size is %ld", iDumpSize, ftell(pOutFile) );
    }
    _Assert(iDumpSize == ftell(pOutFile) && "Frontend Error: Unexpected dump file size.");
  }
  fclose(pOutFile);
  gIndent = BackupIndent;
  pOutFile = pBackupFile;
}

int          clsDump::Dump()
{
  vector<clsScope *>::iterator      it_pScope;
  vector<clsScopeType *>::iterator  it_pScopeType;
  vector<clsDataType *>::iterator   it_pDataType;
  vector<clsSignal *>::iterator     it_pSignal;
  vector<clsNexus *>::iterator      it_pNexus;
  vector<clsLogic *>::iterator      it_pLogic;
  vector<clsLpm *>::iterator        it_pLpm;
  vector<clsConst *>::iterator      it_pConst;
  vector<clsEvent *>::iterator      it_pEvent;
  vector<clsParameter *>::iterator  it_pParameter;
  vector<clsCondition *>::iterator  it_pCondition;
  vector<clsExpression *>::iterator it_pExpression;
  vector<clsBasicBlock *>::iterator it_pBasicBlock;
  int                               iSectionSize;
  int                               iTotalSize;

  iTotalSize = 0;

  // Dump file header.
  iSectionSize = 0;
  iSectionSize += DumpLine("\tFile Magic Code %u\n", DUMP_ID_FILE_MAGIC_CODE, 0);
  iSectionSize += DumpLine("\tVersion %u\n", DUMP_ID_VERSION, 0);
  iTotalSize += iSectionSize;
  // Dump scope section.
  iSectionSize = 0;
  for (it_pScope  = clsIr::m_pScopes.begin();
       it_pScope != clsIr::m_pScopes.end();
       ++it_pScope)
  {
    iSectionSize += DumpScope(*it_pScope, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_SCOPE, iSectionSize, 0);
  for (it_pScope  = clsIr::m_pScopes.begin();
       it_pScope != clsIr::m_pScopes.end();
       ++it_pScope)
  {
    DumpScope(*it_pScope, 0);
  }
  iTotalSize += iSectionSize;
  // Dump scope type section.
  iSectionSize = 0;
  for (it_pScopeType  = clsIr::m_pScopeTypes.begin();
       it_pScopeType != clsIr::m_pScopeTypes.end();
       ++it_pScopeType)
  {
    iSectionSize += DumpScopeType(*it_pScopeType, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_SCOPE_TYPE, iSectionSize, 0);
  for (it_pScopeType  = clsIr::m_pScopeTypes.begin();
       it_pScopeType != clsIr::m_pScopeTypes.end();
       ++it_pScopeType)
  {
    DumpScopeType(*it_pScopeType, 0);
  }
  iTotalSize += iSectionSize;
  // Dump data type section.
  iSectionSize = 0;
  for (it_pDataType  = clsIr::m_pDataTypes.begin();
       it_pDataType != clsIr::m_pDataTypes.end();
       ++it_pDataType)
  {
    iSectionSize += DumpDataType(*it_pDataType, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_DATA_TYPE, iSectionSize, 0);
  for (it_pDataType  = clsIr::m_pDataTypes.begin();
       it_pDataType != clsIr::m_pDataTypes.end();
       ++it_pDataType)
  {
    DumpDataType(*it_pDataType, 0);
  }
  iTotalSize += iSectionSize;
  // Dump signal section.
  iSectionSize = 0;
  for (it_pSignal  = clsIr::m_pSignals.begin();
       it_pSignal != clsIr::m_pSignals.end();
       ++it_pSignal)
  {
    iSectionSize += DumpSignal(*it_pSignal, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_SIGNAL, iSectionSize, 0);
  for (it_pSignal  = clsIr::m_pSignals.begin();
       it_pSignal != clsIr::m_pSignals.end();
       ++it_pSignal)
  {
    DumpSignal(*it_pSignal, 0);
  }
  iTotalSize += iSectionSize;
  // Dump nexus section.
  iSectionSize = 0;
  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    iSectionSize += DumpNexus(*it_pNexus, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_NEXUS, iSectionSize, 0);
  for (it_pNexus  = clsIr::m_pNexuses.begin();
       it_pNexus != clsIr::m_pNexuses.end();
       ++it_pNexus)
  {
    DumpNexus(*it_pNexus, 0);
  }
  iTotalSize += iSectionSize;
  // Dump logic section.
  iSectionSize = 0;
  for (it_pLogic  = clsIr::m_pLogics.begin();
       it_pLogic != clsIr::m_pLogics.end();
       ++it_pLogic)
  {
    iSectionSize += DumpLogic(*it_pLogic, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_LOGIC, iSectionSize, 0);
  for (it_pLogic  = clsIr::m_pLogics.begin();
       it_pLogic != clsIr::m_pLogics.end();
       ++it_pLogic)
  {
    DumpLogic(*it_pLogic, 0);
  }
  iTotalSize += iSectionSize;
  // Dump lpm section.
  iSectionSize = 0;
  for (it_pLpm  = clsIr::m_pLpms.begin();
       it_pLpm != clsIr::m_pLpms.end();
       ++it_pLpm)
  {
    iSectionSize += DumpLpm(*it_pLpm, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_LPM, iSectionSize, 0);
  for (it_pLpm  = clsIr::m_pLpms.begin();
       it_pLpm != clsIr::m_pLpms.end();
       ++it_pLpm)
  {
    DumpLpm(*it_pLpm, 0);
  }
  iTotalSize += iSectionSize;
  // Dump const section.
  iSectionSize = 0;
  for (it_pConst  = clsIr::m_pConsts.begin();
       it_pConst != clsIr::m_pConsts.end();
       ++it_pConst)
  {
    iSectionSize += DumpConst(*it_pConst, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_CONST, iSectionSize, 0);
  for (it_pConst  = clsIr::m_pConsts.begin();
       it_pConst != clsIr::m_pConsts.end();
       ++it_pConst)
  {
    DumpConst(*it_pConst, 0);
  }
  iTotalSize += iSectionSize;
  // Dump parameter section.
  iSectionSize = 0;
  for (it_pParameter  = clsIr::m_pParameters.begin();
       it_pParameter != clsIr::m_pParameters.end();
       ++it_pParameter)
  {
    iSectionSize += DumpParameter(*it_pParameter, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_PARAMETER, iSectionSize, 0);
  for (it_pParameter  = clsIr::m_pParameters.begin();
       it_pParameter != clsIr::m_pParameters.end();
       ++it_pParameter)
  {
    DumpParameter(*it_pParameter, 0);
  }
  iTotalSize += iSectionSize;
  // Dump expression section.
  iSectionSize = 0;
  for (it_pExpression  = clsIr::m_pExpressions.begin();
       it_pExpression != clsIr::m_pExpressions.end();
       ++it_pExpression)
  {
    iSectionSize += DumpExpression(*it_pExpression, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_EXPRESSION, iSectionSize, 0);
  for (it_pExpression  = clsIr::m_pExpressions.begin();
       it_pExpression != clsIr::m_pExpressions.end();
       ++it_pExpression)
  {
    DumpExpression(*it_pExpression, 0);
  }
  iTotalSize += iSectionSize;
  // Dump event section.
  iSectionSize = 0;
  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    iSectionSize += DumpEvent(*it_pEvent, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_EVENT, iSectionSize, 0);
  for (it_pEvent  = clsIr::m_pEvents.begin();
       it_pEvent != clsIr::m_pEvents.end();
       ++it_pEvent)
  {
    DumpEvent(*it_pEvent, 0);
  }
  iTotalSize += iSectionSize;
  // Dump condition section.
  iSectionSize = 0;
  for (it_pCondition  = clsIr::m_pConditions.begin();
       it_pCondition != clsIr::m_pConditions.end();
       ++it_pCondition)
  {
    iSectionSize += DumpCondition(*it_pCondition, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_CONDITION, iSectionSize, 0);
  for (it_pCondition  = clsIr::m_pConditions.begin();
       it_pCondition != clsIr::m_pConditions.end();
       ++it_pCondition)
  {
    DumpCondition(*it_pCondition, 0);
  }
  iTotalSize += iSectionSize;
  // Dump basicblock section.
  iSectionSize = 0;
  for (it_pBasicBlock  = clsIr::m_pBasicBlocks.begin();
       it_pBasicBlock != clsIr::m_pBasicBlocks.end();
       ++it_pBasicBlock)
  {
    iSectionSize += DumpBasicBlock(*it_pBasicBlock, 1);
  }
  iSectionSize += DumpSectionHeader(DUMP_ID_SECTION_BASICBLOCK, iSectionSize, 0);
  for (it_pBasicBlock  = clsIr::m_pBasicBlocks.begin();
       it_pBasicBlock != clsIr::m_pBasicBlocks.end();
       ++it_pBasicBlock)
  {
    DumpBasicBlock(*it_pBasicBlock, 0);
  }
  iTotalSize += iSectionSize;
  // Dump file ending.
  iSectionSize = 0;
  iSectionSize += DumpLine("\tFile Size %u\n", iTotalSize, 1);
  iSectionSize += DumpLine("\tFile End Magic Code %u\n", DUMP_ID_FILE_END_MAGIC_CODE, 1);
  iTotalSize += iSectionSize;
  DumpLine("\tFile Size %u\n", iTotalSize, 0);
  DumpLine("\tFile End Magic Code %u\n", DUMP_ID_FILE_END_MAGIC_CODE, 0);
  
  return iTotalSize;
}

int          clsDump::DumpLine(const char * sFormat, int onlyCalculateSize)
{
  if (!onlyCalculateSize)
  {
    if (m_isTextMode)
    {
      _WriteOutputLine("%s", sFormat);
    }
    else
    {
      if (m_isDebugMode)
      {
        fprintf(stderr, "%s", sFormat);
      }
    }
  }
  return 0;
}

int          clsDump::DumpLine(const char * sFormat, int iInteger, int onlyCalculateSize)
{
  if (!onlyCalculateSize)
  {
    if (m_isTextMode)
    {
      _WriteOutputLine(sFormat, iInteger);
    }
    else
    {
      if (m_isDebugMode)
      {
        fprintf(stderr, sFormat, iInteger);
      }
      fwrite(&iInteger, sizeof(int), 1, pOutFile);
    }
  }
  return sizeof(int);
}

int          clsDump::DumpLine(const char * sFormat, const char * sString, int onlyCalculateSize)
{
  _Assert(sString && "Frontend Error: NULL pointer in dumping string.");
  if (!onlyCalculateSize)
  {
    if (m_isTextMode)
    {
      _WriteOutputLine(sFormat, sString);
    }
    else
    {
      if (m_isDebugMode)
      {
        fprintf(stderr, sFormat, sString);
      }
      fwrite(sString, 1, strlen(sString) + 1, pOutFile);
    }
  }
  return strlen(sString) + 1;
}

int          clsDump::DumpByte(const char * sFormat, char iByte, int onlyCalculateSize)
{
  if (!onlyCalculateSize)
  {
    if (m_isTextMode)
    {
      _WriteOutputLine(sFormat, iByte);
    }
    else
    {
      if (m_isDebugMode)
      {
        fprintf(stderr, sFormat, iByte);
      }
      fwrite(&iByte, sizeof(char), 1, pOutFile);
    }
  }
  return sizeof(char);
}
