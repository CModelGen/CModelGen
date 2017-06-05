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
#ifndef ERRORCODE_H
#define ERRORCODE_H

// Error codes for the frontend.
#define ERROR_CODE_FRONTEND_MAGIC_CODE           0x00000001
#define ERROR_CODE_FRONTEND_MAGIC_END_CODE       0x00000002
#define ERROR_CODE_FRONTEND_SEEK_FAILED          0x00000003
#define ERROR_CODE_FRONTEND_WRONG_FILE_SIZE      0x00000004
#define ERROR_CODE_FRONTEND_WRONG_SECTION_HEADER 0x00000005
#define ERROR_CODE_FRONTEND_WRONG_SECTION_SIZE   0x00000006
#define ERROR_CODE_FRONTEND_WRONG_SCOPE          0x00000007
#define ERROR_CODE_FRONTEND_WRONG_LOCATION       0x00000008
#define ERROR_CODE_FRONTEND_WRONG_STRING         0x00000009
#define ERROR_CODE_FRONTEND_WRONG_SCOPE_TYPE     0x0000000A
#define ERROR_CODE_FRONTEND_WRONG_DATA_TYPE      0x0000000B
#define ERROR_CODE_FRONTEND_WRONG_SIGNAL         0x0000000C
#define ERROR_CODE_FRONTEND_WRONG_NEXUS          0x0000000D
#define ERROR_CODE_FRONTEND_WRONG_LOGIC          0x0000000E
#define ERROR_CODE_FRONTEND_WRONG_LPM            0x0000000F
#define ERROR_CODE_FRONTEND_WRONG_CONST          0x00000010
#define ERROR_CODE_FRONTEND_WRONG_PARAMETER      0x00000011
#define ERROR_CODE_FRONTEND_WRONG_EXPRESSION     0x00000012
#define ERROR_CODE_FRONTEND_WRONG_LVALUE         0x00000013
#define ERROR_CODE_FRONTEND_WRONG_EVENT          0x00000014
#define ERROR_CODE_FRONTEND_WRONG_CONDITION      0x00000015
#define ERROR_CODE_FRONTEND_WRONG_STATEMENT      0x00000016
#define ERROR_CODE_FRONTEND_WRONG_BASICBLOCK     0x00000017

// Warning codes.
#define WARNING_CODE_IGNORED                     0x00000001
#define WARNING_CODE_REPLACED                    0x00000002

#endif // ERRORCODE_H
