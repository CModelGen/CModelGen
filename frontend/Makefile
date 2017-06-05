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

ifndef CMODELGEN_GIT_HOME
  # Environment variable CMODELGEN_GIT_HOME is not defined.
  $(warning "Warning: environment variable CMODELGEN_GIT_HOME is not defined. Therefore the external build mode is used.")
  include localrule.mk
else
  include $(CMODELGEN_GIT_HOME)/makerule/rule.mk
  BUILD_UTILS             = build_utils
  APP_NAME                = $(FE_NAME)
endif

IVL_SRC_DIR             = $(CMODELGEN_GIT_FRONTEND)/$(IVL_DIR)
IVL_BUILD_DEBUG_DIR     = $(CMODELGEN_BUILD_DIR)/frontend/ivl-build-debug/$(IVL_DIR)
IVL_BUILD_RELEASE_DIR   = $(CMODELGEN_BUILD_DIR)/frontend/ivl-build-release/$(IVL_DIR)
IVL_INSTALL_DEBUG_DIR   = $(CMODELGEN_BUILD_DIR)/frontend/ivl-install-debug/$(IVL_DIR)
IVL_INSTALL_RELEASE_DIR = $(CMODELGEN_BUILD_DIR)/frontend/ivl-install-release/$(IVL_DIR)

.PHONY: all help build clean
.PHONY: $(BUILD_UTILS) build_ivl_debug build_ivl_release

all: build

help:
	@echo "Available options: help, build, clean"

$(IVL_SRC_DIR)/configure:
	cd $(IVL_SRC_DIR); autoconf; cd -

##### Debug

$(IVL_BUILD_DEBUG_DIR)/Makefile: $(IVL_SRC_DIR)/configure
	@if !( [ -d ${IVL_BUILD_DEBUG_DIR} ] ); then mkdir -p ${IVL_BUILD_DEBUG_DIR}; fi
	cd $(IVL_BUILD_DEBUG_DIR); CC="$(CC)" CXX="$(CXX)" CFLAGS="$(DEBUG_CFLAGS) $(ICA_PATCH_CFLAGS)" CXXFLAGS="$(DEBUG_CFLAGS) $(ICA_PATCH_CXXFLAGS)" LDFLAGS="$(DEBUG_FLAGS)" $(IVL_SRC_DIR)/configure --prefix=$(IVL_INSTALL_DEBUG_DIR) --enable-suffix=-$(APP_NAME); cd -

build_ivl_debug: $(IVL_BUILD_DEBUG_DIR)/Makefile
	$(MAKE) -C $(IVL_BUILD_DEBUG_DIR)
	@if !( [ -d ${IVL_INSTALL_DEBUG_DIR} ] ); then mkdir -p ${IVL_INSTALL_DEBUG_DIR}; fi
	$(MAKE) -C $(IVL_BUILD_DEBUG_DIR) install

##### Release

$(IVL_BUILD_RELEASE_DIR)/Makefile: $(IVL_SRC_DIR)/configure
	@if !( [ -d ${IVL_BUILD_RELEASE_DIR} ] ); then mkdir -p ${IVL_BUILD_RELEASE_DIR}; fi
	cd $(IVL_BUILD_RELEASE_DIR); CC="$(CC)" CXX="$(CXX)" CFLAGS="$(RELEASE_CFLAGS) $(ICA_PATCH_CFLAGS)" CXXFLAGS="$(RELEASE_CFLAGS) $(ICA_PATCH_CXXFLAGS)" LDFLAGS="$(RELEASE_FLAGS)" $(IVL_SRC_DIR)/configure --prefix=$(IVL_INSTALL_RELEASE_DIR) --enable-suffix=-$(APP_NAME); cd -

build_ivl_release: $(IVL_BUILD_RELEASE_DIR)/Makefile
	$(MAKE) -C $(IVL_BUILD_RELEASE_DIR)
	@if !( [ -d ${IVL_INSTALL_RELEASE_DIR} ] ); then mkdir -p ${IVL_INSTALL_RELEASE_DIR}; fi
	$(MAKE) -C $(IVL_BUILD_RELEASE_DIR) install

build: $(BUILD_UTILS) build_ivl_debug build_ivl_release

clean:
	rm -rf $(CMODELGEN_BUILD_DIR)/frontend
	rm -rf $(IVL_SRC_DIR)/configure $(IVL_SRC_DIR)/autom4te.cache
