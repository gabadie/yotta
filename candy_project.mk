# ------------------------------------------------------------------------------ Yotta library's headers directory

# ------------------------------------------------------------ default configuration
ifeq ($(filter debug nightly,$(config)),)
    override config=release
endif

PROJECT_CFLAGS := -Wall -Wextra -m64 -std=gnu99
PROJECT_LDFLAGS := -lpthread

# ------------------------------------------------------------ release configuration
ifeq ($(config),release)
    PROJECT_CFLAGS += -O3 -Werror -mmmx -mavx
endif

# ------------------------------------------------------------ nightly configuration
ifeq ($(config),nightly)
    PROJECT_CFLAGS += -g
endif

# ------------------------------------------------------------ debug configuration
ifeq ($(config),debug)
    PROJECT_CFLAGS += -g -DYOTTA_DEBUG
endif


$(call trash_configs, debug nightly release)
$(call hook_precommit_configs, debug release)


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
