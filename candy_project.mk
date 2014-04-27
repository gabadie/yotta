#
#       This file is part of the Yotta Distributed Computing Library Project.
#       Yotta Distributed Computing Library, named from the International
#       System of Units (SI) denoting a factor of 10^24, is a C library
#       designed for network-based distributed computing.
#       Yotta provides features to the developer in order to execute
#       simultaneously time-consuming algorithms across severals
#       clustered computers.
#
#       Co-founders:
#          Guillaume Abadie
#          Thierry Cantenot
#
#       Yotta Distributed Computing Library's official code repository is
#       available at <http://github.com/gabadie/yotta>.
#
#   GNU GPL v3 license:
#       Yotta is a free C library: you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation, either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#   github.com:
#       github.com is NEITHER property of the Yotta Distributed Computing
#       Library Project NOR any of its Co-founders'.  Please see
#       <http://github.com/site/terms>.
#

# ------------------------------------------------------------------------------ Yotta library's headers directory

# ------------------------------------------------------------ configurations
PROJECT_GCC_ENV := $(call env_preference, gcc-4.9 gcc-4.8 gcc-4.7 gcc-4.6 gcc)
PROJECT_DEFAULT_CC := $(call env_preference, clang $(PROJECT_GCC_ENV))

PROJECT_RELEASES := $(addprefix release-,$(call env_which,clang $(PROJECT_GCC_ENV)))
PROJECT_CONFIGS := $(addsuffix -$(PROJECT_DEFAULT_CC),debug nightly) $(PROJECT_RELEASES)

$(call trash_configs, $(PROJECT_CONFIGS))
$(call hook_precommit_configs, debug-$(PROJECT_DEFAULT_CC) $(PROJECT_RELEASES))


# ------------------------------------------------------------ default configuration
ifneq ($(filter debug nightly,$(config)),)
    override config:=$(config)-$(PROJECT_DEFAULT_CC)
endif

ifeq ($(filter $(PROJECT_CONFIGS),$(config)),)
    override config:=$(firstword $(PROJECT_RELEASES))
endif


# ------------------------------------------------------------ gcc/clang configuration
override CC:=$(PROJECT_DEFAULT_CC)

ifeq ($(filter-out release-gcc%,$(config)),)
    override CC:=$(PROJECT_GCC_ENV)
endif

ifeq ($(filter-out release-clang%,$(config)),)
    override CC:=clang
endif

override LD:=$(CC)

# ------------------------------------------------------------ default parameters
PROJECT_CFLAGS := -Wall -Wextra -m64 -std=gnu99
PROJECT_LDFLAGS := -lpthread

# ------------------------------------------------------------ release parameters
ifeq ($(filter-out release_%,$(config)),)
    PROJECT_CFLAGS += -O3 -Werror -mmmx -mavx
endif

# ------------------------------------------------------------ nightly parameters
ifeq ($(config),nightly)
    PROJECT_CFLAGS += -g
endif

# ------------------------------------------------------------ debug parameters
ifeq ($(config),debug)
    PROJECT_CFLAGS += -g -DYOTTA_DEBUG
endif


# ------------------------------------------------------------------------------ Yotta library's headers directory

LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

LIB_HEADERS := $(call bin_header_deps,./src/yotta.h)

$(LIB_HEADERS_TARGET): $(LIB_HEADERS)
$(LIB_HEADERS_TARGET): CPFLAGS += $(LIB_HEADERS)
$(LIB_HEADERS_TARGET): CPROOTDIR = src/


# ------------------------------------------------------------------------------ Yotta library's binaries

LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

LIB_OBJECT_BINARIES := $(call bin_object_files,$(call filelist,./src/yotta.flist))

# ------------------------------------------------------------ compilation/link configuration
$(LIB_BINARIES_TARGET): $(LIB_OBJECT_BINARIES)
$(LIB_BINARIES_TARGET): CFLAGS += $(PROJECT_CFLAGS)
$(LIB_BINARIES_TARGET): ARFLAGS += $(LIB_OBJECT_BINARIES)


# ------------------------------------------------------------------------------ Yotta slave application

SLAVE_PRODUCT := $(call product_create,PYEXEC,yotta_slave)
SLAVE_TARGET := $(call product_set_target,$(SLAVE_PRODUCT),src_slave/main.py)
$(call product_public,$(SLAVE_PRODUCT))

SLAVE_PYTHON_FILES := $(call rwildcard,src_slave/,*.py)

# ------------------------------------------------------------ dependencies
$(SLAVE_TARGET): $(filter-out $(SLAVE_TARGET),$(SLAVE_PYTHON_FILES))


# ------------------------------------------------------------------------------ Yotta library's tests

TEST_LIB_FILES := $(call filelist,tests/testlib.flist)
TEST_LIB_C_FILES := $(filter %.c,$(TEST_LIB_FILES))
TEST_LIB_SCRIPTS := $(filter-out %.c,$(TEST_LIB_FILES))

# ------------------------------------------------------------ C test targets
TEST_LIB_TARGETS := $(call test_bins,$(TEST_LIB_C_FILES))

# ------------------------------------------------------------ C configuration
$(TEST_LIB_TARGETS): CDEPS += $(LIB_HEADERS_TARGET)
$(TEST_LIB_TARGETS): CFLAGS += $(PROJECT_CFLAGS)
$(TEST_LIB_TARGETS): CFLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) $(PROJECT_CFLAGS)

# ------------------------------------------------------------ link configuration
$(TEST_LIB_TARGETS): $(LIB_BINARIES_TARGET)
$(TEST_LIB_TARGETS): LDFLAGS += $(LIB_BINARIES_TARGET)
$(TEST_LIB_TARGETS): LDFLAGS += $(PROJECT_LDFLAGS)

# ------------------------------------------------------------ test scripts
TEST_SCRIPT_TARGETS := $(call test_scripts,$(TEST_LIB_SCRIPTS))

$(TEST_SCRIPT_TARGETS): $(LIB_BINARIES_TARGET)
$(TEST_SCRIPT_TARGETS): TESTFLAGS = $(BUILD_DIR)


# ------------------------------------------------------------------------------ Yotta deamon's tests

TEST_DAEMON_FILES := $(call filelist,src_daemon/test_daemon.flist)
TEST_DAEMON_TARGETS := $(call test_scripts,$(TEST_DAEMON_FILES),py.test)


# ------------------------------------------------------------------------------ Yotta demos

DEMOS := $(call filelist,demos/demos.flist)

$(foreach DEMO_PATH, $(DEMOS), \
	$(eval DEMO_PRODUCT := $(call product_create,BINEXEC,$(notdir $(DEMO_PATH)))) \
	$(eval DEMO_TARGET := $(call product_target,$(DEMO_PRODUCT))) \
	$(call product_public,$(DEMO_PRODUCT)) \
	$(eval DEMO_FILES := $(call filelist,$(DEMO_PATH)/$(notdir $(DEMO_PATH)).flist)) \
	$(eval DEMO_BINARIES := $(call bin_object_files, $(DEMO_FILES))) \
	$(eval $(DEMO_BINARIES): $(LIB_HEADERS_TARGET)) \
	$(eval $(DEMO_BINARIES): CFLAGS += $(PROJECT_CFLAGS)) \
	$(eval $(DEMO_BINARIES): CFLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) $(PROJECT_CFLAGS)) \
	$(eval $(DEMO_TARGET): $(LIB_BINARIES_TARGET)) \
	$(eval $(DEMO_TARGET): $(DEMO_BINARIES)) \
	$(eval $(DEMO_TARGET): LDFLAGS += $(DEMO_BINARIES)) \
	$(eval $(DEMO_TARGET): LDFLAGS += $(LIB_BINARIES_TARGET)) \
	$(eval $(DEMO_TARGET): LDFLAGS += $(PROJECT_LDFLAGS)) \
)
