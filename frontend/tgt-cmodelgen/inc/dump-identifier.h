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
#ifndef DUMP_IDENTIFIER_H
#define DUMP_IDENTIFIER_H

#define DUMP_ID_VERSION                     0x00000001

#define DUMP_ID_FILE_MAGIC_CODE             0x002AB702
#define DUMP_ID_FILE_END_MAGIC_CODE         0xFF2AB702

#define DUMP_ID_SECTION_SCOPE               0x00FFFF00
#define DUMP_ID_SECTION_SCOPE_TYPE          0x00FFFF01
#define DUMP_ID_SECTION_DATA_TYPE           0x00FFFF02
#define DUMP_ID_SECTION_SIGNAL              0x00FFFF03
#define DUMP_ID_SECTION_NEXUS               0x00FFFF04
#define DUMP_ID_SECTION_LOGIC               0x00FFFF05
#define DUMP_ID_SECTION_LPM                 0x00FFFF06
#define DUMP_ID_SECTION_CONST               0x00FFFF07
#define DUMP_ID_SECTION_PARAMETER           0x00FFFF08
#define DUMP_ID_SECTION_EXPRESSION          0x00FFFF09
#define DUMP_ID_SECTION_EVENT               0x00FFFF0A
#define DUMP_ID_SECTION_CONDITION           0x00FFFF0B
#define DUMP_ID_SECTION_BASICBLOCK          0x00FFFF0C

#endif // DUMP_IDENTIFIER_H
