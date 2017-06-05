#
#      Copyright (c) 2016 CModelGen Project
#              ALL RIGHTS RESERVED
#
#    This source code is free software; you can redistribute it
#    and/or modify it in source code form under the terms of the GNU
#    General Public License as published by the Free Software
#    Foundation; either version 2 of the License, or (at your option)
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
IVL_DIR                 = iverilog-working
BUILD_UTILS             =
APP_NAME                = cmodelgen
CC                      = gcc
CXX                     = g++
OS                      = $(shell uname -o)
CMODELGEN_GIT_FRONTEND  = $(shell pwd)
CMODELGEN_BUILD_DIR     = $(CMODELGEN_GIT_FRONTEND)/build
ifeq ($(OS),Msys)
  DEBUG_CFLAGS          = -g -DCMODELGEN_DEBUG
  ICA_PATCH_CFLAGS      = -DCMODELGEN -static-libgcc
  ICA_PATCH_CXXFLAGS    = -DCMODELGEN -static-libstdc++ -static-libgcc
else
  DEBUG_CFLAGS          = -g -DCMODELGEN_DEBUG -rdynamic
  ICA_PATCH_CFLAGS      = -D__inline= -DCMODELGEN
  ICA_PATCH_CXXFLAGS    = -D__inline= -DCMODELGEN
endif
RELEASE_CFLAGS          =
COMMON_DIR              = $(VPATH)/../common
GENHEADER_DIR           = $(VPATH)/..
ERRORMESSAGE_H          = $(GENHEADER_DIR)/errormessage.h
