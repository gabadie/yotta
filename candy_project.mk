
# ------------------------------------------------------------------------------ Yotta library's headers directory

# ------------------------------------------------------------ default configuration
ifneq ($(config),debug)
    config=release
endif

PROJECT_CFLAGS := -Wall -Wextra
PROJECT_LDFLAGS :=

# ------------------------------------------------------------ release configuration
ifeq ($(config),release)
    PROJECT_CFLAGS += -O3 -Werror
endif

# ------------------------------------------------------------ debug configuration
ifeq ($(config),debug)
    PROJECT_CFLAGS += -g -DYOTTA_DEBUG
endif


# ------------------------------------------------------------------------------ Yotta library's headers directory

LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

$(LIB_HEADERS_TARGET): $(call bin_header_deps,./src/yotta.h)


# ------------------------------------------------------------------------------ Yotta library's binaries

LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

LIB_OBJECT_BINARIES := $(call bin_object_files,$(call filelist,./src/yotta.flist))

# ------------------------------------------------------------ compilation/link configuration
$(LIB_BINARIES_TARGET): $(LIB_OBJECT_BINARIES)
$(LIB_BINARIES_TARGET): CFLAGS += $(PROJECT_CFLAGS)
$(LIB_BINARIES_TARGET): LDFLAGS += $(LIB_OBJECT_BINARIES)


# ------------------------------------------------------------------------------ Yotta slave application

SLAVE_PRODUCT := $(call product_create,PYEXEC,yotta_slave)
SLAVE_TARGET := $(call product_set_target,$(SLAVE_PRODUCT),src_slave/main.py)
$(call product_public,$(SLAVE_PRODUCT))

SLAVE_PYTHON_FILES := $(call rwildcard,src_slave/,*.py)

# ------------------------------------------------------------ dependencies
$(SLAVE_TARGET): $(filter-out $(SLAVE_TARGET),$(SLAVE_PYTHON_FILES))


# ------------------------------------------------------------------------------ Yotta library's tests

TEST_LIB_C_FILES := $(call filelist,tests/testlib.flist)

$(foreach TEST,$(TEST_LIB_C_FILES), \
    $(eval TEST_LIB_NAME := $(call product_create,BINEXEC,$(notdir $(TEST:.c=)))) \
    $(eval TEST_LIB_TARGET := $(call product_target,$(TEST_LIB_NAME))) \
    $(eval $(TEST_LIB_TARGET): $(call bin_object_files,$(TEST))) \
    $(eval $(TEST_LIB_TARGET): LDFLAGS += $(call bin_object_files,$(TEST))) \
    $(eval TEST_LIB_TARGETS += $(TEST_LIB_TARGET)) \
    $(eval $($(call test_product,$(TEST_LIB_NAME)))) \
)

# ------------------------------------------------------------ C configuration
$(TEST_LIB_TARGETS): CDEPS += $(LIB_HEADERS_TARGET)
$(TEST_LIB_TARGETS): CFLAGS += $(PROJECT_CFLAGS)
$(TEST_LIB_TARGETS): CFLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) $(PROJECT_CFLAGS)

# ------------------------------------------------------------ link configuration
$(TEST_LIB_TARGETS): $(LIB_BINARIES_TARGET)
$(TEST_LIB_TARGETS): LDFLAGS += $(PROJECT_LDFLAGS)
$(TEST_LIB_TARGETS): LDFLAGS += $(LIB_BINARIES_TARGET)

